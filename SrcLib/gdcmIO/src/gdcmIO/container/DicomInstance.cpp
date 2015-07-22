/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <gdcmUIDGenerator.h>

#include <fwCore/spyLog.hpp>

#include <fwData/Image.hpp>
#include <fwDicomData/DicomSeries.hpp>
#include <fwMedData/Series.hpp>
#include <fwMedData/Study.hpp>
#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include "gdcmIO/container/DicomInstance.hpp"

namespace gdcmIO
{
namespace container
{

//------------------------------------------------------------------------------

DicomInstance::DicomInstance() :
    m_isMultiFiles(true),
    m_SOPClassUID("")
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

DicomInstance::DicomInstance(SPTR(::fwMedData::Series)series, bool isMultiFiles) :
    m_isMultiFiles(isMultiFiles),
    m_SOPClassUID(""),
    m_studyInstanceUID(series->getStudy()->getInstanceUID()),
    m_seriesInstanceUID(series->getInstanceUID())
{
    // Compute SOPClassUID
    this->computeSOPClassUID(series);

    // Generate SOPInstanceUIDs
    this->generateSOPInstanceUIDs(series);
}

//------------------------------------------------------------------------------

DicomInstance::DicomInstance(SPTR(::fwDicomData::DicomSeries)dicomSeries) :
    m_isMultiFiles(dicomSeries->getLocalDicomPaths().size()>1),
    m_studyInstanceUID(dicomSeries->getStudy()->getInstanceUID()),
    m_seriesInstanceUID(dicomSeries->getInstanceUID())
{
    // Get SOPClassUID
    ::fwDicomData::DicomSeries::SOPClassUIDContainerType sopClassUIDContainer = dicomSeries->getSOPClassUIDs();
    m_SOPClassUID                                                             = sopClassUIDContainer.begin()->c_str();
}

//------------------------------------------------------------------------------

DicomInstance::DicomInstance(const DicomInstance &dicomInstance)
{
    SLM_TRACE_FUNC();
    m_isMultiFiles            = dicomInstance.m_isMultiFiles;
    m_SOPClassUID             = dicomInstance.m_SOPClassUID;
    m_SOPInstanceUIDContainer = dicomInstance.m_SOPInstanceUIDContainer;
}

//------------------------------------------------------------------------------

DicomInstance::~DicomInstance()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void DicomInstance::computeSOPClassUID(::fwMedData::Series::sptr series)
{
    // Retrieve series type
    ::fwMedData::ImageSeries::csptr imageSeries = ::fwMedData::ImageSeries::dynamicConstCast(series);
    ::fwMedData::ModelSeries::csptr modelSeries = ::fwMedData::ModelSeries::dynamicConstCast(series);

    // Create result
    std::string sopClassUID = "";

    if(imageSeries)
    {
        // Retrieve image from series
        ::fwData::Image::csptr image = imageSeries->getImage();

        // Compute instance dimension
        unsigned int dimension = (this->getIsMultiFiles()) ? 2 : image->getNumberOfDimensions();

        // Define SOP Class UID from the modality
        ::gdcm::MediaStorage mediaStorage;
        mediaStorage.GuessFromModality(series->getModality().c_str(), dimension);

        // Identify the SOPClassUID from a guess
        if (mediaStorage != ::gdcm::MediaStorage::MS_END && mediaStorage.GetString() != 0)
        {
            sopClassUID = mediaStorage.GetString();
        }
        // Force SOPClassUID to be CTImageStorage
        else
        {
            sopClassUID = ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::CTImageStorage);
        }
    }
    else if(modelSeries)
    {
        sopClassUID = ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::SurfaceSegmentationStorage);
    }

    // Update instance information
    this->setSOPClassUID(sopClassUID);
}

//------------------------------------------------------------------------------

void DicomInstance::generateSOPInstanceUIDs(::fwMedData::Series::sptr series)
{
    // Retrieve ImageSeries
    ::fwMedData::ImageSeries::csptr imageSeries = ::fwMedData::ImageSeries::dynamicConstCast(series);

    // Compute number of instances
    unsigned int nbInstances = (imageSeries && m_isMultiFiles) ? (imageSeries->getImage()->getSize()[2]) : (1);

    // Create generator
    ::gdcm::UIDGenerator uidGenerator;

    // Generate UIDs
    for(unsigned int i = 0; i < nbInstances; ++i)
    {
        m_SOPInstanceUIDContainer.push_back(uidGenerator.Generate());
    }
}

} //namespace container
} //namespace gdcmIO
