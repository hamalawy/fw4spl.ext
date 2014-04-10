/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOEXT_DCMTK_READER_RGBLOOKUP_IMAGERGBLOOKUPLAZYREADER_HPP__
#define __FWDICOMIOEXT_DCMTK_READER_RGBLOOKUP_IMAGERGBLOOKUPLAZYREADER_HPP__

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include <fwDicomData/DicomSeries.hpp>
#include <fwTools/Type.hpp>

#include "fwDicomIOExt/dcmtk/helper/Codec.hpp"

#include "fwDicomIOExt/config.hpp"

namespace fwDicomIOExt
{
namespace dcmtk
{
namespace reader
{
namespace rgblookup
{

/**
 * @class ImageRGBLookupLazyReader
 * @brief This class is used to read the buffer of a DICOM image in LAZY mode when a pixel lookup must be performed.
 * @date  2014.
 */
class FWDICOMIOEXT_CLASS_API ImageRGBLookupLazyReader
{
public:

    /**
     * @brief Create an instance buffer according to the image type.
     * The template T is used to determine color format
     * @param[in] rows Number of rows
     * @param[in] columns Number of columns
     * @param[in] instance Path to the instance
     * @param[in] redLookup Red lookup table
     * @param[in] greenLookup Red lookup table
     * @param[in] blueLookup Red lookup table
     * @param[in] pixelValueBitsAllocated Pixel value bits allocated
     */
    template< typename T>
    FWDICOMIOEXT_API static void* createInstanceBuffer(unsigned int rows, unsigned int columns,
            const ::boost::filesystem::path& instance, const T* redLookup , const T* greenLookup,
             const T* blueLookup, unsigned short pixelValueBitsAllocated)
    {
        if(pixelValueBitsAllocated == 16)
        {
            return ::fwDicomIOExt::dcmtk::reader::rgblookup::ImageRGBLookupLazyReader::createInstanceBuffer<T, Uint16>(
                    rows, columns, instance, redLookup, greenLookup, blueLookup);
        }
        else
        {
            return ::fwDicomIOExt::dcmtk::reader::rgblookup::ImageRGBLookupLazyReader::createInstanceBuffer<T, Uint8>(
                    rows, columns, instance, redLookup, greenLookup, blueLookup);
        }
    }

    /**
     * @brief Create an instance buffer according to the image type.
     * The template T is used to determine color format
     * The template U is used to determine pixel value format
     * @param[in] rows Number of rows
     * @param[in] columns Number of columns
     * @param[in] instance Path to the instance
     * @param[in] redLookup Red lookup table
     * @param[in] greenLookup Red lookup table
     * @param[in] blueLookup Red lookup table
     */
    template< typename T, typename U >
    FWDICOMIOEXT_API static T* createInstanceBuffer(unsigned int rows, unsigned int columns,
            const ::boost::filesystem::path& instance, const T* redLookup , const T* greenLookup,
             const T* blueLookup)
    {
        DcmFileFormat fileFormat;
        OFCondition status;
        DcmDataset* dataset;

        // Register codecs
        ::fwDicomIOExt::dcmtk::helper::Codec::registerCodecs();

        // Create temporary buffer
        T* tempoBuffer = new T[rows * columns * 3]; // 3 colors (RGB)

        // Read instance
        const std::string filename = instance.string();
        status = fileFormat.loadFile(filename.c_str());
        FW_RAISE_IF("Unable to read the file: \""+filename+"\"", status.bad());
        dataset = fileFormat.getDataset();

        // Decompress data set if compressed
        dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);

        // Read pixels
        const U* pixelData;
        if(sizeof(U) == 1)
        {
            const Uint8* pdata;
            dataset->findAndGetUint8Array(DCM_PixelData, pdata);
            pixelData = (U*)pdata;
        }
        else
        {
            const Uint16* pdata;
            dataset->findAndGetUint16Array(DCM_PixelData, pdata);
            pixelData = (U*)pdata;
        }

        OSLM_WARN_IF("Unable to read pixel data.", !pixelData);

        for (unsigned int x = 0; x < columns; ++x)
        {
            for (unsigned int y = 0; y < rows; ++y)
            {
                unsigned int position = y + (x * rows);
                U value = pixelData[position];
                tempoBuffer[position*3] = (redLookup[value]/(double)0xffff)*256;
                tempoBuffer[position*3+1] = (greenLookup[value]/(double)0xffff)*256;
                tempoBuffer[position*3+2] = (blueLookup[value]/(double)0xffff)*256;
            }
        }


        // Clean up codecs
        ::fwDicomIOExt::dcmtk::helper::Codec::cleanup();

        return tempoBuffer;
    }


};

} //rgblookup
} //reader
} //dcmtk
} //fwDicomIOExt


#endif /* __FWDICOMIOEXT_DCMTK_READER_RGBLOOKUP_IMAGERGBLOOKUPLAZYREADER_HPP__ */
