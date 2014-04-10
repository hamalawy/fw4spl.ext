/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/make_shared.hpp>

#include <gdcmImageWriter.h>

#include <fwCore/spyLog.hpp>

#include <fwData/Image.hpp>
#include <fwMedData/Equipment.hpp>
#include <fwMedData/Patient.hpp>
#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/Study.hpp>

#include "gdcmIO/helper/DicomData.hpp"
#include "gdcmIO/writer/helper/FileWriter.hpp"
#include "gdcmIO/writer/ie/Patient.hpp"
#include "gdcmIO/writer/ie/Study.hpp"
#include "gdcmIO/writer/ie/Equipment.hpp"
#include "gdcmIO/writer/ie/Series.hpp"
#include "gdcmIO/writer/ie/Image.hpp"
#include "gdcmIO/writer/ie/FrameOfReference.hpp"

#include "gdcmIO/writer/iod/CTMRImageIOD.hpp"

namespace gdcmIO
{
namespace writer
{
namespace iod
{

//------------------------------------------------------------------------------

CTMRImageIOD::CTMRImageIOD(SPTR(::gdcmIO::container::DicomInstance) instance, ::boost::filesystem::path folderPath):
        ::gdcmIO::writer::iod::InformationObjectDefinition(instance, folderPath)
{
}

//------------------------------------------------------------------------------

CTMRImageIOD::~CTMRImageIOD()
{
}

//------------------------------------------------------------------------------

void CTMRImageIOD::write(::fwMedData::Series::sptr series)
{
    // Retrieve image series
    ::fwMedData::ImageSeries::sptr imageSeries = ::fwMedData::ImageSeries::dynamicCast(series);
    SLM_ASSERT("Image series should not be null.", imageSeries);

    // Retrieve image
    ::fwData::Image::sptr image = imageSeries->getImage();

    // Create writer
    SPTR(::gdcm::ImageWriter) writer = ::boost::make_shared< ::gdcm::ImageWriter >();

    // Create Information Entity helpers
    ::gdcmIO::writer::ie::Patient patientIE(writer, m_instance, series->getPatient());
    ::gdcmIO::writer::ie::Study studyIE(writer, m_instance, series->getStudy());
    ::gdcmIO::writer::ie::Series seriesIE(writer, m_instance, series);
    ::gdcmIO::writer::ie::FrameOfReference frameOfReferenceIE(writer, m_instance, series);
    ::gdcmIO::writer::ie::Equipment equipmentIE(writer, m_instance, series->getEquipment());
    ::gdcmIO::writer::ie::Image imageIE(writer, m_instance, imageSeries->getImage());

    // Write Patient Module - PS 3.3 C.7.1.1
    patientIE.writePatientModule();

    // Write General Study Module - PS 3.3 C.7.2.1
    studyIE.writeGeneralStudyModule();

    // Write Patient Study Module - PS 3.3 C.7.2.2
    studyIE.writePatientStudyModule();

    // Write General Series Module - PS 3.3 C.7.3.1
    seriesIE.writeGeneralSeriesModule();

    // Write Frame of Reference Module - PS 3.3 C.7.4.1
    frameOfReferenceIE.writeFrameOfReferenceModule();

    // Write General Equipment Module - PS 3.3 C.7.5.1
    equipmentIE.writeGeneralEquipmentModule();

    // Write General Image Module - PS 3.3 C.7.6.1
    imageIE.writeGeneralImageModule();

    // Write Image Plane Module - PS 3.3 C.7.6.2
    imageIE.writeImagePlaneModule();

    // Write Image Pixel Module - PS 3.3 C.7.6.3
    imageIE.writeImagePixelModule();

    if(m_instance->getCRefSOPClassUID() == ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::CTImageStorage))
    {
        // Write CT Image Module - PS 3.3 C.8.2.1
        imageIE.writeCTImageModule();
    }
    else if(m_instance->getCRefSOPClassUID() == ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::MRImageStorage))
    {
        // Write MR Image Module - PS 3.3 C.8.3.1
        imageIE.writeMRImageModule();
    }

    // Write VOI LUT Module - PS 3.3 C.11.2
    imageIE.writeVOILUTModule();

    // Write SOP Common Module - PS 3.3 C.12.1
    imageIE.writeSOPCommonModule();


    // Copy dataset to avoid writing conflict with GDCM
    const ::gdcm::DataSet datasetCopy = writer->GetFile().GetDataSet();

    // Compute number of frames
    unsigned int nbFrames = (m_instance->getIsMultiFiles())?(image->getSize()[2]):1;

    // Write specific tags according to frame number
    for(unsigned int i = 0; i < nbFrames; ++i)
    {
        // Reset dataset
        writer->GetFile().SetDataSet(datasetCopy);

        // Write SOP Common Module specific tags - PS 3.3 C.12.1
        imageIE.writeSOPCommonModuleSpecificTags(i);

        // Write General Image Module specific tags - PS 3.3 C.7.6.1
        imageIE.writeGeneralImageModuleSpecificTags(i);

        // Write Image Plane Module specific tags - PS 3.3 C.7.6.2
        imageIE.writeImagePlaneModuleSpecificTags(i);

        // Write Image Pixel Module specific tags - PS 3.3 C.7.6.3
        imageIE.writeImagePixelModuleSpecificTags(i);

        // Write file
        std::stringstream ss;
        ss << m_folderPath.string() << "/im" << std::setfill('0') << std::setw(5) << i;
        ::gdcmIO::writer::helper::FileWriter::write(ss.str(), writer);

    }


}

} // namespace iod
} // namespace writer
} // namespace gdcmIO
