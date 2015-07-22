/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <gdcmImageWriter.h>
#include <gdcmUIDGenerator.h>

#include <fwComEd/helper/Image.hpp>
#include <fwData/Image.hpp>

#include "gdcmIO/helper/DicomData.hpp"
#include "gdcmIO/writer/ie/Image.hpp"

namespace gdcmIO
{
namespace writer
{
namespace ie
{

//------------------------------------------------------------------------------

Image::Image(SPTR(::gdcm::Writer)writer,
             SPTR(::gdcmIO::container::DicomInstance)instance,
             ::fwData::Image::sptr image) :
    ::gdcmIO::writer::ie::InformationEntity< ::fwData::Image >(writer, instance, image)
{
}

//------------------------------------------------------------------------------

Image::~Image()
{
}

//------------------------------------------------------------------------------

void Image::writeGeneralImageModule()
{
    // Nothing to write
}

//------------------------------------------------------------------------------

void Image::writeGeneralImageModuleSpecificTags(unsigned int instanceNumber)
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Instance Number
    ::gdcmIO::helper::DicomData::setTagValue< int, 0x0020, 0x0013 >(instanceNumber, dataset);

}

//------------------------------------------------------------------------------

void Image::writeImagePlaneModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Retrieve GDCM image
    SPTR(::gdcm::ImageWriter) imageWriter = ::boost::static_pointer_cast< ::gdcm::ImageWriter >(m_writer);
    ::gdcm::Image &gdcmImage              = imageWriter->GetImage();

    // Pixel Spacing - Type 1
    // WARNING : some DICOM image have not any spacing (NOT SUPPORTED BY FW4SPL), but stuff like "Pixel Aspect Ratio"
    const unsigned int dimension         = m_object->getNumberOfDimensions();
    const std::vector< double > &spacing = m_object->getSpacing();
    for (unsigned int i = 0; i < dimension; ++i)
    {
        gdcmImage.SetSpacing(i, spacing[i]);
    }
    OSLM_TRACE("Image's spacing : " << spacing[0] << "x" << spacing[1] << "x" << spacing[2]);

    // Slice Thickness - Type 2
    // NOTE: ::gdcm::ImageWriter may replace SliceThickness values according to Image Origins
    ::gdcmIO::helper::DicomData::setTagValue< double, 0x0018, 0x0050 >(spacing[2], dataset);

    // Image Orientation (Patient) - Type 1
    // This tag his handled by gdcm writer

}

//------------------------------------------------------------------------------

void Image::writeImagePlaneModuleSpecificTags(unsigned int instanceNumber)
{
    // Retrieve GDCM image
    SPTR(::gdcm::ImageWriter) imageWriter = ::boost::static_pointer_cast< ::gdcm::ImageWriter >(m_writer);
    ::gdcm::Image &gdcmImage              = imageWriter->GetImage();

    // Image Position (Patient) - Type 1
    const std::vector< double > &origin  = m_object->getOrigin();
    const std::vector< double > &spacing = m_object->getSpacing();
    gdcmImage.SetOrigin(0, origin[0]);
    gdcmImage.SetOrigin(1, origin[1]);
    gdcmImage.SetOrigin(2, origin[2] + spacing[2]*instanceNumber);
}

//------------------------------------------------------------------------------

void Image::writeImagePixelModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Retrieve GDCM image
    ::gdcm::ImageWriter *imageWriter = ::boost::static_pointer_cast< ::gdcm::ImageWriter >(m_writer).get();
    ::gdcm::Image &gdcmImage         = imageWriter->GetImage();

    // Image's photometric interpretation - Type 1
    ::gdcm::PhotometricInterpretation photoInter = ::gdcmIO::helper::DicomData::getPhotometricInterpretation(*m_object);
    gdcmImage.SetPhotometricInterpretation(photoInter);
    OSLM_TRACE("Image's photometric interpretation : " << photoInter);

    // Image's pixel type
    ::gdcm::PixelFormat pixelFormat = ::gdcmIO::helper::DicomData::getPixelType(m_object);
    gdcmImage.SetPixelFormat(pixelFormat);
    OSLM_TRACE("Image's pixel type : " << pixelFormat);

    //Image's number of dimension
    unsigned int dimension = (unsigned int) (m_instance->getIsMultiFiles()) ? 2 : m_object->getNumberOfDimensions();
    gdcmImage.SetNumberOfDimensions(dimension);
    OSLM_TRACE("Image's number of dimensions : " << dimension);

    // Image's dimension
    const ::fwData::Image::SizeType &size = m_object->getSize();
    for (unsigned int i = 0; i < dimension; i++)
    {
        gdcmImage.SetDimension(i, size[i]);
    }
    OSLM_TRACE("Image's dimensions : " << size[0] << "x" << size[1] << "x" << size[2]);

}

//------------------------------------------------------------------------------

void Image::writeImagePixelModuleSpecificTags(unsigned int instanceNumber)
{
    SLM_ASSERT("Wrong instance number.", m_instance->getIsMultiFiles() || instanceNumber == 0);

    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Retrieve GDCM image
    ::gdcm::ImageWriter *imageWriter = ::boost::static_pointer_cast< ::gdcm::ImageWriter >(m_writer).get();
    ::gdcm::Image &gdcmImage         = imageWriter->GetImage();

    // Compute buffer size
    const ::fwData::Image::SizeType &size = m_object->getSize();
    unsigned int bufferLength             = size[0] * size[1] * gdcmImage.GetPixelFormat().GetPixelSize();
    bufferLength = (!m_instance->getIsMultiFiles()) ? (bufferLength*size[2]) : bufferLength;

    // Retrieve image buffer
    ::fwComEd::helper::Image imageHelper(m_object);
    const char *imageBuffer = static_cast< char* >(imageHelper.getBuffer());

    // Pixel Data - Type 1C
    ::gdcm::DataElement pixeldata(::gdcm::Tag(0x7fe0, 0x0010));
    pixeldata.SetByteValue(imageBuffer + instanceNumber * bufferLength, bufferLength);
    gdcmImage.SetDataElement(pixeldata);
}

//------------------------------------------------------------------------------

void Image::writeVOILUTModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    const double windowCenter = m_object->getWindowCenter();
    const double windowWidth  = m_object->getWindowWidth();
    if(windowCenter || windowWidth)
    {
        // Image's windows center
        ::gdcmIO::helper::DicomData::setTagValues< double, 0x0028, 0x1050 >(&windowCenter, 1, dataset);
        OSLM_TRACE("Image's window center : " << windowCenter);

        // Image's windows width
        ::gdcmIO::helper::DicomData::setTagValues< double, 0x0028, 0x1051 >(&windowWidth, 1, dataset);
        OSLM_TRACE("Image's window width : " << windowWidth);
    }

}

//------------------------------------------------------------------------------

void Image::writeSOPCommonModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // SOP Class UID
    ::gdcmIO::helper::DicomData::setTagValue<  0x0008, 0x0016 >(m_instance->getSOPClassUID(), dataset);
    SLM_TRACE("SOP Class UID : " + m_instance->getSOPClassUID());
}

//------------------------------------------------------------------------------

void Image::writeSOPCommonModuleSpecificTags(unsigned int instanceNumber)
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // SOP Instance UID
    const std::string sopInstanceUID = m_instance->getCRefSOPInstanceUIDContainer()[instanceNumber];
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0018 >(sopInstanceUID, dataset);
}

//------------------------------------------------------------------------------

void Image::writeCTImageModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Image Type - Type 1 - FIXME: Fake Value
    const ::gdcm::String< 92, 16 > imageType = "ORIGINAL\\PRIMARY\\AXIAL";
    ::gdcmIO::helper::DicomData::setTagValues< ::gdcm::String< 92, 16 >, 0x0008, 0x0008 >(&imageType, 1, dataset);

    // Acquisition Number - Type 2 - FIXME: Fake Value
    unsigned int acquisitionNumber = 1;
    ::gdcmIO::helper::DicomData::setTagValue< unsigned int, 0x0020, 0x0012 >(acquisitionNumber, dataset);

    // KVP - Type 2 - FIXME: Fake Value
    ::gdcmIO::helper::DicomData::setTagValue< double, 0x0018, 0x0060 >(1, dataset);

    // Rescale Type - Type 1C - FIXME: Fake Value
    ::gdcmIO::helper::DicomData::setTagValue< 0x0028, 0x1054 >("HU", dataset);

}

//------------------------------------------------------------------------------

void Image::writeMRImageModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Image Type - Type 1 - FIXME: Fake Value
    const ::gdcm::String< 92, 16 > imageType = "ORIGINAL\\PRIMARY\\MPR";
    ::gdcmIO::helper::DicomData::setTagValues< ::gdcm::String< 92, 16 >, 0x0008, 0x0008 >(&imageType, 1, dataset);

    // Scanning Sequence - Type 1 - FIXME: Fake Value
    const ::gdcm::String< 92, 16 > scanningSequence = "SE";
    ::gdcmIO::helper::DicomData::setTagValues< ::gdcm::String< 92, 16 >, 0x0018, 0x0020 >(&scanningSequence, 1,
                                                                                          dataset);

    // Sequence Variant - Type 1 - FIXME: Fake Value
    const ::gdcm::String< 92, 16 > sequenceVariant = "NONE";
    ::gdcmIO::helper::DicomData::setTagValues< ::gdcm::String< 92, 16 >, 0x0018, 0x0021 >(&sequenceVariant, 1, dataset);

    // Scan Options - Type 2 - FIXME: Fake Value
    const ::gdcm::String< 92, 16 > scanOption = "";
    ::gdcmIO::helper::DicomData::setTagValues< ::gdcm::String< 92, 16 >, 0x0018, 0x0022 >(&scanOption, 1, dataset);

    // MR Acquisition Type - Type 2 - FIXME: Fake Value
    ::gdcmIO::helper::DicomData::setTagValue< 0x0018, 0x0023 >("3D", dataset);

    // Repetition Time - Type 2C - FIXME: Fake Value
    ::gdcmIO::helper::DicomData::setTagValue< double, 0x0018, 0x0080 >(0, dataset);

    // Echo Time - Type 2 - FIXME: Fake Value
    ::gdcmIO::helper::DicomData::setTagValue< double, 0x0018, 0x0081 >(1, dataset);

    // Echo Train Length - Type 2 - FIXME: Fake Value
    ::gdcmIO::helper::DicomData::setTagValue< unsigned int, 0x0018, 0x0091 >(1, dataset);


}


} // namespace ie
} // namespace writer
} // namespace gdcmIO
