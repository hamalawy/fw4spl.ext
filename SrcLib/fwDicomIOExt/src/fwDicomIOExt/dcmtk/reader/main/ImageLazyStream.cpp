/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "fwDicomIOExt/dcmtk/reader/main/ImageLazyReader.hpp"
#include "fwDicomIOExt/dcmtk/reader/main/ImageLazyStream.hpp"

namespace fwDicomIOExt
{
namespace dcmtk
{
namespace reader
{
namespace main
{

//------------------------------------------------------------------------------

bool filesStillExist( const ::fwDicomData::DicomSeries::sptr dicomSeries )
{
    ::boost::filesystem::path filePath;
    bool allFilesExists = true;

    for(::fwDicomData::DicomSeries::DicomPathContainerType::const_iterator itPath =
            dicomSeries->getLocalDicomPaths().begin();
            allFilesExists && ( itPath != dicomSeries->getLocalDicomPaths().end() );
            ++itPath )
    {
        filePath = itPath->second;
        allFilesExists &= ::boost::filesystem::exists(filePath);
    }
    return allFilesExists;
}

//------------------------------------------------------------------------------

ImageLazySource::ImageLazySource( ImageLazyInformation::sptr dcmInfo ) :
    m_dcmInfo ( dcmInfo )
{
    SLM_ASSERT( "ImageLazySource needs at least one dicom file to read an image.",
            !dcmInfo->m_dicomSeries->getLocalDicomPaths().empty());

    m_frameSize = m_dcmInfo->m_rows*m_dcmInfo->m_columns*m_dcmInfo->m_imageType.sizeOf();
    m_currentFrame = 0;
    m_currentPosition = 0;
    m_currentPath = m_dcmInfo->m_dicomSeries->getLocalDicomPaths().begin();

    m_frame = static_cast<char*>(::fwDicomIOExt::dcmtk::reader::main::ImageLazyReader::createInstanceBuffer(m_dcmInfo->m_rows,
                        m_dcmInfo->m_columns, m_currentPath->second, m_dcmInfo->m_rescaleSlope,
                        m_dcmInfo->m_rescaleIntercept, m_dcmInfo->m_pixelRepresentation, m_dcmInfo->m_imageType));
}

//------------------------------------------------------------------------------

std::streamsize ImageLazySource::read(char* s, std::streamsize n)
{
    std::streamsize result = -1;

    // Be sure that all files exist when reading the first frame
    if(m_currentFrame == 0 && m_currentPosition == 0)
    {
        const bool exist = filesStillExist(m_dcmInfo->m_dicomSeries);
        OSLM_ERROR_IF( "Error while reading dicom files : " <<
                m_dcmInfo->m_dicomSeries->getLocalDicomPaths().begin()->second.string() << " ...", !exist );
    }

    // Load new frame
    if(m_currentPosition + n > m_frameSize)
    {
        // Copy remaining bytes
        const size_t remainingBytes = m_frameSize - m_currentPosition;
        const size_t extraBytes = n - remainingBytes;

        if(remainingBytes > 0)
        {
            std::copy( m_frame + m_currentPosition, m_frame + m_currentPosition + remainingBytes, s);
            result = remainingBytes;
        }

        // Change frame
        delete m_frame;
        ++m_currentFrame;
        ++m_currentPath;
        m_currentPosition = 0;

        // If there is more frame
        if(m_currentPath != m_dcmInfo->m_dicomSeries->getLocalDicomPaths().end())
        {
            // Copy extra bytes from the new frame
            if(extraBytes > 0)
            {
                m_frame = static_cast<char*>(::fwDicomIOExt::dcmtk::reader::main::ImageLazyReader::createInstanceBuffer(
                                    m_dcmInfo->m_rows, m_dcmInfo->m_columns, m_currentPath->second,
                                    m_dcmInfo->m_rescaleSlope, m_dcmInfo->m_rescaleIntercept,
                                    m_dcmInfo->m_pixelRepresentation, m_dcmInfo->m_imageType));

                std::copy( m_frame + m_currentPosition, m_frame + m_currentPosition + extraBytes, s);
                m_currentPosition+= extraBytes;
                result = remainingBytes + extraBytes;
            }
        }
        else
        {
            OSLM_TRACE("Reading process over.");
        }

    }
    // Copy bytes from the loaded frame
    else
    {
        std::copy( m_frame + m_currentPosition, m_frame + m_currentPosition + n, s);
        m_currentPosition += n;
        result = n;

        if(m_currentPath->second == m_dcmInfo->m_dicomSeries->getLocalDicomPaths().rbegin()->second &&
                m_currentPosition == m_frameSize)
        {
            SLM_TRACE("Reading process over.");
            delete m_frame;
        }
    }

    return result;

}


//------------------------------------------------------------------------------

ImageLazyStream::ImageLazyStream( ImageLazyInformation::sptr dcmInfo ) : m_dcmInfo ( dcmInfo )
{}

//------------------------------------------------------------------------------

SPTR(std::istream) ImageLazyStream::get()
{
    SPTR(::boost::iostreams::stream<ImageLazySource>) is
            = ::boost::make_shared< ::boost::iostreams::stream<ImageLazySource> >( m_dcmInfo );
    return is;
}

//------------------------------------------------------------------------------

} //main
} //reader
} //dcmtk
} //fwDicomIOExt
