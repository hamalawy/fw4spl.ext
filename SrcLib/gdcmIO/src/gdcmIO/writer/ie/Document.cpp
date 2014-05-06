/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <sstream>

#include <boost/algorithm/string/split.hpp>

#include <gdcmUIDGenerator.h>

#include <fwComEd/Dictionary.hpp>
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>
#include <fwMedData/Series.hpp>
#include <fwMedData/types.hpp>
#include <fwTools/dateAndTime.hpp>

#include "gdcmIO/helper/DicomData.hpp"
#include "gdcmIO/helper/StructuredReport.hpp"   // For graphviz dump
#include "gdcmIO/writer/tid/MeasurementReport.hpp"

#include "gdcmIO/writer/ie/Document.hpp"

namespace gdcmIO
{
namespace writer
{
namespace ie
{

//------------------------------------------------------------------------------

Document::Document(SPTR(::gdcm::Writer) writer,
        SPTR(::gdcmIO::container::DicomInstance) instance,
        ::fwData::Image::sptr image,
         bool use3DSR):
        ::gdcmIO::writer::ie::InformationEntity< ::fwData::Image >(writer, instance, image), m_use3DSR(use3DSR)
{
}

//------------------------------------------------------------------------------

Document::~Document()
{
}

//------------------------------------------------------------------------------

void Document::writeSRDocumentGeneralModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    ::boost::posix_time::ptime ptime = boost::posix_time::second_clock::local_time();

    // Instance Number - Type 1
    ::gdcmIO::helper::DicomData::setTagValue< int, 0x0020, 0x0013 >(0, dataset);

    // Content Date - Type 1 - FIXME: Keep series date ?
    std::string date = ::fwTools::getDate(ptime);
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0023 >(date, dataset);

    // Content Time - Type 1 - FIXME: Keep series time ?
    std::string time =  ::fwTools::getTime(ptime);
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0033 >(time, dataset);

    // Performed Procedure Code Sequence (0040,A372) // Type 2 (FIXME: CID 7000 ?)
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > sequence = new ::gdcm::SequenceOfItems();
    sequence->SetLengthToUndefined();
    ::gdcmIO::helper::DicomData::setSQ< 0x0040, 0xa372 >(sequence, dataset);

    // Completion flag - Type 1 - (FIXME: Set to COMPLETE ?)
    ::gdcmIO::helper::DicomData::setTagValue< 0x0040, 0xa491 >("PARTIAL", dataset);

    // Verification Flag - Type 1 - (FIXME: Set to VERIFIED ?)
    ::gdcmIO::helper::DicomData::setTagValue< 0x0040, 0xa493 >("UNVERIFIED", dataset);

    // Pertinent Other Evidence Sequence - Type 1C
    this->writePertinentOtherEvidenceSequence();

}

//------------------------------------------------------------------------------

void Document::writePertinentOtherEvidenceSequence()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Pertinent Other Evidence Sequence - Type 1C
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > sequence = new ::gdcm::SequenceOfItems();
    sequence->SetLengthToUndefined();
    ::gdcmIO::helper::DicomData::setSQ< 0x0040, 0xa385 >(sequence, dataset);

    // Add new study to sequence
    ::gdcm::Item studyItem;
    studyItem.SetVLToUndefined();
    ::gdcm::DataSet &studyItemDataset = studyItem.GetNestedDataSet();
    ::gdcmIO::helper::DicomData::setTagValue< 0x0020, 0x000D >(m_instance->getStudyInstanceUID(), studyItemDataset);

    // Referenced Series Sequence - Type 1
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > seriesSequence = new ::gdcm::SequenceOfItems();
    seriesSequence->SetLengthToUndefined();
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1115 >(seriesSequence, studyItemDataset);

    // Add new series to sequence
    ::gdcm::Item seriesItem;
    seriesItem.SetVLToUndefined();
    ::gdcm::DataSet &seriesItemDataset = seriesItem.GetNestedDataSet();
    ::gdcmIO::helper::DicomData::setTagValue< 0x0020, 0x000E >(m_instance->getSeriesInstanceUID(), seriesItemDataset);

    // Referenced SOP Sequence - Type 1
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > sopSequence = new ::gdcm::SequenceOfItems();
    sopSequence->SetLengthToUndefined();
    ::gdcmIO::helper::DicomData::setSQ< 0x0008, 0x1199 >(sopSequence, seriesItemDataset);

    BOOST_FOREACH(const std::string& instanceUID, m_instance->getCRefSOPInstanceUIDContainer())
    {
        // Add new instance to sequence
        ::gdcm::Item instanceItem;
        instanceItem.SetVLToUndefined();
        ::gdcm::DataSet &instanceItemDataset = instanceItem.GetNestedDataSet();

        // Referenced SOP Class UID - Type 1
        ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1150 >(m_instance->getSOPClassUID(), instanceItemDataset);

        // Referenced SOP Instance UID - Type 1
        ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x1155 >(instanceUID, instanceItemDataset);

        sopSequence->AddItem(instanceItem);
    }

    seriesSequence->AddItem(seriesItem);
    sequence->AddItem(studyItem);
}

//------------------------------------------------------------------------------

void Document::writeSRDocumentContentModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // Create SR Root node
    ::gdcmIO::writer::tid::MeasurementReport report(m_writer, m_instance, m_object);
    SPTR(::gdcmIO::container::sr::DicomSRNode) root = report.createRootNode(m_use3DSR);
    root->write(dataset);

    // Dump SR
    //::gdcmIO::helper::StructuredReport::dumpSR(root);
}

//------------------------------------------------------------------------------

void Document::writeSOPCommonModule()
{
    // Retrieve dataset
    ::gdcm::DataSet &dataset = m_writer->GetFile().GetDataSet();

    // SOP Class UID
    std::string sopClassUID;
    if(!m_use3DSR)
    {
        sopClassUID = ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::ComprehensiveSR);
    }
    else
    {
        sopClassUID = "1.2.840.10008.5.1.4.1.1.88.34";  // FIXME Remove hard coded string
//        sopClassUID = ::gdcm::MediaStorage::GetMSString(::gdcm::MediaStorage::Comprehensive3DSR);
    }
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0016 >(sopClassUID, dataset);

    // SOP Instance UID
    ::gdcm::UIDGenerator uidGenerator;
    std::string sopInstanceUID = uidGenerator.Generate();
    ::gdcmIO::helper::DicomData::setTagValue< 0x0008, 0x0018 >(sopInstanceUID, dataset);
}


} // namespace ie
} // namespace writer
} // namespace gdcmIO
