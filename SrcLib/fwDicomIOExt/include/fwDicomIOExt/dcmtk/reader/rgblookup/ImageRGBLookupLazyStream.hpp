/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOEXT_DCMTK_READER_RGBLOOKUP_IMAGERGBLOOKUPLAZYSTREAM_HPP__
#define __FWDICOMIOEXT_DCMTK_READER_RGBLOOKUP_IMAGERGBLOOKUPLAZYSTREAM_HPP__

#include <vector>

#include <boost/iostreams/stream.hpp>

#include <fwCore/base.hpp>
#include <fwDicomData/DicomSeries.hpp>
#include <fwMemory/stream/in/IFactory.hpp>
#include <fwTools/Type.hpp>

#include "fwDicomIOExt/config.hpp"

namespace fwDicomIOExt
{
namespace dcmtk
{
namespace reader
{
namespace rgblookup
{

//------------------------------------------------------------------------------

/**
 * @class ImageRGBLookupLazyInformation
 * @brief Basic class used to store informations in order to read DICOM images
 */
class FWDICOMIOEXT_CLASS_API ImageRGBLookupLazyInformation
{
public:
    typedef SPTR ( ImageRGBLookupLazyInformation ) sptr;

    /// Dicom series used to get the paths of the instances
    ::fwDicomData::DicomSeries::sptr m_dicomSeries;

    /// Number of rows in the image
    unsigned int m_rows;

    /// Number of columns in the image
    unsigned int m_columns;

    /// Depth of the image
    unsigned int m_depth;

    /// Bits allocated
    unsigned short m_bitsAllocated;

    /// Pixel value bits allocated
    unsigned short m_pixelValueBitsAllocated;

    /// Image type
    ::fwTools::Type m_imageType;

};

//------------------------------------------------------------------------------

/**
 * @class ImageRGBLookupLazySource
 * @brief Class used to build a std::istream using the ::boost::iostreams api
 */
class FWDICOMIOEXT_CLASS_API ImageRGBLookupLazySource
{
public:
    typedef char char_type;
    typedef ::boost::iostreams::source_tag category;

    /**
     * @brief Constructor
     * @param[in] dcmInfo DICOM information
     */
    ImageRGBLookupLazySource( ImageRGBLookupLazyInformation::sptr dcmInfo );

    /**
     * @brief Method to read n bytes in dicom buffer and write it in s.
     * @param[in] size Number of bytes
     * @param[out] stream Destination buffer
     */
    std::streamsize read(char* stream, std::streamsize size);

private:
    /// DICOM information
    ImageRGBLookupLazyInformation::sptr m_dcmInfo;

    /// Frame size
    size_t m_frameSize;

    /// Current frame
    int m_currentFrame;

    /// Current position in the frame
    size_t m_currentPosition;

    /// Current frame
    char* m_frame;

    /// Current path
    ::fwDicomData::DicomSeries::DicomPathContainerType::const_iterator m_currentPath;

    /// Red lookup table
    void* m_redLookup;

    /// Green lookup table
    void* m_greenLookup;

    /// Blue lookup table
    void* m_blueLookup;

};

//------------------------------------------------------------------------------

/**
 * @class ImageRGBLookupLazyStream
 * @brief Class used to perform a lazy reading of dicom images with fw4spl system
 */
class FWDICOMIOEXT_CLASS_API ImageRGBLookupLazyStream : public ::fwMemory::stream::in::IFactory
{

public:

    /**
     * @brief Constructor. Builds the ::fwMemory::stream::in::IFactory with few dicom information
     * @param[in] dcmInfo DICOM information
     */
    ImageRGBLookupLazyStream( ImageRGBLookupLazyInformation::sptr dcmInfo );

protected:

    /// Returns the istream on dicom image buffer
    SPTR(std::istream) get();

    /// To conserve dicom information
    ImageRGBLookupLazyInformation::sptr m_dcmInfo;
};

//------------------------------------------------------------------------------

} //rgblookup
} //reader
} //dcmtk
} //fwDicomIOExt

#endif // __FWDICOMIOEXT_DCMTK_READER_RGBLOOKUP_IMAGERGBLOOKUPLAZYSTREAM_HPP__
