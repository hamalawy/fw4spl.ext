/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/foreach.hpp>

#include <gdcmUIDGenerator.h>

#include <fwComEd/Dictionary.hpp>

#include <fwData/Image.hpp>
#include <fwData/Point.hpp>
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>

#include <fwTools/dateAndTime.hpp>

#include "gdcmIO/helper/DicomData.hpp"
#include "gdcmIO/writer/ie/SpatialFiducials.hpp"

namespace gdcmIO
{
namespace writer
{
namespace ie
{

//------------------------------------------------------------------------------

SpatialFiducials::SpatialFiducials(SPTR(::gdcm::Writer) writer,
        SPTR(::gdcmIO::container::DicomInstance) instance,
        ::fwData::Image::sptr image):
        ::gdcmIO::writer::ie::InformationEntity< ::fwData::Image >(writer, instance, image)
{
}

//------------------------------------------------------------------------------

SpatialFiducials::~SpatialFiducials()
{
}

//------------------------------------------------------------------------------

void SpatialFiducials::writeSpatialFiducialsModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    ::boost::posix_time::ptime ptime = boost::posix_time::second_clock::local_time();

    // Content Date - Type 1 - FIXME: Keep series date ?
    std::string date = ::fwTools::getDate(ptime);
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0023 >(date, dataset);

    // Content Time - Type 1 - FIXME: Keep series time ?
    std::string time =  ::fwTools::getTime(ptime);
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0033 >(time, dataset);

    // Instance Number - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< unsigned int, 0x0020, 0x0013 >(0, dataset);

    // Content Label - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< 0x0070, 0x0080 >("SF", dataset);

    // Content Description - Type 2
    ::gdcmIO::helper::DicomData::setTagValue< 0x0070, 0x0081 >("Spatial Fiducials", dataset);

    // Content Creator's Name - Type 2 - TODO: Set fiducial creator name ?
    ::gdcmIO::helper::DicomData::setTagValue< 0x0070, 0x0084 >("Unknown^Unknown", dataset);


    // Fiducial Set Sequence - Type 1
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > fiducialSetSequence = new ::gdcm::SequenceOfItems();
    ::gdcmIO::helper::DicomData::setSQ< 0x0070, 0x031C >(fiducialSetSequence, dataset);

    // Create Fiducial Sequence Item
    ::gdcm::Item fiducialSetItem;
    fiducialSetItem.SetVLToUndefined();
    ::gdcm::DataSet &fiducialSetItemDataset = fiducialSetItem.GetNestedDataSet();

    // Referenced Image Sequence - Type 1C
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > referencedImageSequence = new ::gdcm::SequenceOfItems();
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1140 >(referencedImageSequence, fiducialSetItemDataset);

    // Add all referenced image
    for(int index = 0; index < m_instance->getCRefSOPInstanceUIDContainer().size(); ++index)
    {
        ::gdcm::Item referencedImageItem;
        referencedImageItem.SetVLToUndefined();
        ::gdcm::DataSet &referencedImageItemDataset = referencedImageItem.GetNestedDataSet();

        // Referenced Frame Number - Type 1C
        int frameNumber = index + 1;
        ::gdcmIO::helper::DicomData::setTagValues< int, 0x0008, 0x1160 >(&frameNumber, 1, referencedImageItemDataset);

        // Referenced SOP Class UID - Type 1
        ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1150 >(m_instance->getCRefSOPClassUID(),
                referencedImageItemDataset);

        // Referenced SOP Instance UID - Type 1
        ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1155 >(m_instance->getCRefSOPInstanceUIDContainer()[index],
                referencedImageItemDataset);

        // Add referenced image to sequence
        referencedImageSequence->AddItem(referencedImageItem);

    }


    // Fiducial Sequence - Type 1
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > fiducialSequence = new ::gdcm::SequenceOfItems();
    ::gdcmIO::helper::DicomData::setSQ< 0x0070, 0x031E >(fiducialSequence, fiducialSetItemDataset);

    // Write landmarks
    this->writeLandmarks(fiducialSequence);

    // Add Fiducial Set to sequence
    fiducialSetSequence->AddItem(fiducialSetItem);

}

//------------------------------------------------------------------------------

void SpatialFiducials::writeLandmarks(::gdcm::SmartPointer< ::gdcm::SequenceOfItems > sequence)
throw(::gdcmIO::exception::Failed)
{
    ::fwData::PointList::sptr pointList =
            m_object->getField< ::fwData::PointList >(::fwComEd::Dictionary::m_imageLandmarksId);
    if (pointList)
    {
        unsigned int index = 0;
        BOOST_FOREACH(const ::fwData::Point::sptr& point, pointList->getRefPoints())
        {
            ::gdcm::Item fiducialItem;
            fiducialItem.SetVLToUndefined();
            ::gdcm::DataSet &fiducialItemDataset = fiducialItem.GetNestedDataSet();

            // Fiducial Identifier - Type 1
            std::stringstream ssIdentifier;
            ssIdentifier << "landmark-" << index;
            ::gdcmIO::helper::DicomData::setTagValue< 0x0070, 0x0310 >(ssIdentifier.str(), fiducialItemDataset);

            // Fiducial Description - Type 3
            std::string label = point->getField< ::fwData::String >(::fwComEd::Dictionary::m_labelId)->value();
            ::gdcmIO::helper::DicomData::setTagValue< 0x0070, 0x030F >(label, fiducialItemDataset);

            // Shape Type - Type 1
            ::gdcmIO::helper::DicomData::setTagValue< 0x0070, 0x0306 >("POINT", fiducialItemDataset);

            // Graphic Coordinates Data Sequence - Type 1C
            ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > graphicCoodinatesDataSequence =
                    new ::gdcm::SequenceOfItems();
            ::gdcmIO::helper::DicomData::setSQ< 0x0070, 0x0318 >(graphicCoodinatesDataSequence, fiducialItemDataset);

            ::gdcm::Item graphicDataItem;
            graphicDataItem.SetVLToUndefined();
            ::gdcm::DataSet &graphicDataItemDataset = graphicDataItem.GetNestedDataSet();

            // Graphic Data - Type 1
            float coordinates[2];
            coordinates[0] = point->getCoord()[0];
            coordinates[1] = point->getCoord()[1];
            ::gdcmIO::helper::DicomData::setTagValues< float, 0x0070, 0x0022 >(coordinates, 2, graphicDataItemDataset);
            OSLM_TRACE("WRITE NEW POINT : [" << label << "] " << coordinates[0] << " / " << coordinates[1]);

            // Referenced Image Sequence - Type 1
            ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > referencedImageSequence = new ::gdcm::SequenceOfItems();
            ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1140 >(referencedImageSequence, graphicDataItemDataset);

            int frameNumber = ::gdcmIO::helper::DicomData::convertPointToFrameNumber(m_object, point);
            this->addReferencedImage(frameNumber, referencedImageSequence);

            // Add graphic data to sequence
            graphicCoodinatesDataSequence->AddItem(graphicDataItem);

            // Add Fiducial to sequence
            sequence->AddItem(fiducialItem);

            // Increase index
            ++index;
        }
    }
}

//------------------------------------------------------------------------------

void SpatialFiducials::writeCommonInstanceReferenceModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Referenced Series Sequence - Type 1C
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > referencedSeriesSequence = new ::gdcm::SequenceOfItems();
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1115 >(referencedSeriesSequence, dataset);

    // Create Referenced Series
    ::gdcm::Item referencedSeriesItem;
    referencedSeriesItem.SetVLToUndefined();
    ::gdcm::DataSet &referencedSeriesItemDataset = referencedSeriesItem.GetNestedDataSet();

    // Series Instance UID - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< 0x0020, 0x000E >(m_instance->getSeriesInstanceUID(),
            referencedSeriesItemDataset);

    // Referenced Instance Sequence - Type 1
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > referencedInstanceSequence = new ::gdcm::SequenceOfItems();
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x114A >(referencedInstanceSequence, referencedSeriesItemDataset);

    // Add all referenced image
    for(int index = 0; index < m_instance->getCRefSOPInstanceUIDContainer().size(); ++index)
    {
        ::gdcm::Item referencedInstanceItem;
        referencedInstanceItem.SetVLToUndefined();
        ::gdcm::DataSet &referencedInstanceItemDataset = referencedInstanceItem.GetNestedDataSet();

        // Referenced SOP Class UID - Type 1
        ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1150 >(m_instance->getCRefSOPClassUID(),
                referencedInstanceItemDataset);

        // Referenced SOP Instance UID - Type 1
        ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1155 >(m_instance->getCRefSOPInstanceUIDContainer()[index],
                referencedInstanceItemDataset);

        // Add referenced image to sequence
        referencedInstanceSequence->AddItem(referencedInstanceItem);
    }


    referencedSeriesSequence->AddItem(referencedSeriesItem);

    // Studies Containing Other Referenced Instances Sequence - Type 1C
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > studiesSequence = new ::gdcm::SequenceOfItems();
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1200 >(studiesSequence, dataset);

    ::gdcm::Item studiesItem;
    studiesItem.SetVLToUndefined();
    ::gdcm::DataSet &studiesItemDataset = studiesItem.GetNestedDataSet();

    // Study Instance UID - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< 0x0020, 0x000D >(m_instance->getStudyInstanceUID(), studiesItemDataset);

    // Referenced Series Sequence - Type 1
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1115 >(referencedSeriesSequence, studiesItemDataset);

    studiesSequence->AddItem(studiesItem);
}

//------------------------------------------------------------------------------

void SpatialFiducials::writeSOPCommonModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // SOP Class UID
    std::string sopClassUID = ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::SpacialFiducialsStorage);
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0016 >(sopClassUID, dataset);

    // SOP Instance UID
    ::gdcm::UIDGenerator uidGenerator;
    std::string sopInstanceUID = uidGenerator.Generate();
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0018 >(sopInstanceUID, dataset);
}

//------------------------------------------------------------------------------

void SpatialFiducials::addReferencedImage(int frameNumber,
        ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > referencedImageSequence) throw(::gdcmIO::exception::Failed)
{
    ::gdcm::Item referencedImageItem;
    referencedImageItem.SetVLToUndefined();
    ::gdcm::DataSet &referencedImageItemDataset = referencedImageItem.GetNestedDataSet();

    // Referenced Frame Number - Type 1C
    ::gdcmIO::helper::DicomData::setTagValues< int, 0x0008, 0x1160 >(&frameNumber, 1,
            referencedImageItemDataset);

    // Referenced SOP Class UID - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1150 >(m_instance->getCRefSOPClassUID(),
            referencedImageItemDataset);

    // Referenced SOP Instance UID - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1155 >(
            m_instance->getCRefSOPInstanceUIDContainer()[frameNumber-1], referencedImageItemDataset);

    // Add referenced image to sequence
    referencedImageSequence->AddItem(referencedImageItem);
}

//------------------------------------------------------------------------------

} // namespace ie
} // namespace writer
} // namespace gdcmIO
