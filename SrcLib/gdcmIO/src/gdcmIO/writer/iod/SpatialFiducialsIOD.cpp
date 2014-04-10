/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/make_shared.hpp>

#include <gdcmWriter.h>

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
#include "gdcmIO/writer/ie/SpatialFiducials.hpp"

#include "gdcmIO/writer/iod/SpatialFiducialsIOD.hpp"

namespace gdcmIO
{
namespace writer
{
namespace iod
{

//------------------------------------------------------------------------------

SpatialFiducialsIOD::SpatialFiducialsIOD(
        SPTR(::gdcmIO::container::DicomInstance) instance, ::boost::filesystem::path folderPath):
        ::gdcmIO::writer::iod::InformationObjectDefinition(instance, folderPath)
{
}

//------------------------------------------------------------------------------

SpatialFiducialsIOD::~SpatialFiducialsIOD()
{
}

//------------------------------------------------------------------------------

void SpatialFiducialsIOD::write(::fwMedData::Series::sptr series)
{
    // Retrieve image series
    ::fwMedData::ImageSeries::sptr imageSeries = ::fwMedData::ImageSeries::dynamicCast(series);
    SLM_ASSERT("Image series should not be null.", imageSeries);

    // Retrieve image
    ::fwData::Image::sptr image = imageSeries->getImage();

    // Create writer
    SPTR(::gdcm::Writer) writer = ::boost::make_shared< ::gdcm::Writer >();

    // Create Information Entity helpers
    ::gdcmIO::writer::ie::Patient patientIE(writer, m_instance, series->getPatient());
    ::gdcmIO::writer::ie::Study studyIE(writer, m_instance, series->getStudy());
    ::gdcmIO::writer::ie::Series seriesIE(writer, m_instance, series);
    ::gdcmIO::writer::ie::Equipment equipmentIE(writer, m_instance, series->getEquipment());
    ::gdcmIO::writer::ie::SpatialFiducials spatialFiducialsIE(writer, m_instance, image);

    // Write Patient Module - PS 3.3 C.7.1.1
    patientIE.writePatientModule();

    // Write General Study Module - PS 3.3 C.7.2.1
    studyIE.writeGeneralStudyModule();

    // Write Patient Study Module - PS 3.3 C.7.2.2
    studyIE.writePatientStudyModule();

    // Write General Series Module - PS 3.3 C.7.3.1
    seriesIE.writeGeneralSeriesModule();

    // Write General Series Module - PS 3.3 C.21.1
    seriesIE.writeSpatialFiducialsSeriesModule();

    // Write General Equipment Module - PS 3.3 C.7.5.1
    equipmentIE.writeGeneralEquipmentModule();

    // Write Spatial Fiducials Module - PS 3.3 C.21.2
    spatialFiducialsIE.writeSpatialFiducialsModule();

    // Write Common Instance Reference Module - PS 3.3 C.12.2
    spatialFiducialsIE.writeCommonInstanceReferenceModule();

    // Write SOP Common Module - PS 3.3 C.12.1
    spatialFiducialsIE.writeSOPCommonModule();

    // Write document
    ::gdcmIO::writer::helper::FileWriter::write(m_folderPath.string() + "/imSF", writer);

}

} // namespace iod
} // namespace writer
} // namespace gdcmIO
