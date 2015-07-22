/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <gdcmSequenceOfItems.h>

#include <fwCore/spyLog.hpp>

#include "gdcmIO/helper/DicomData.hpp"
#include "gdcmIO/container/sr/DicomSRNumNode.hpp"

namespace gdcmIO
{
namespace container
{
namespace sr
{

//------------------------------------------------------------------------------

DicomSRNumNode::DicomSRNumNode(const DicomCodedAttribute& codedAttribute,
                               const std::string& relationship, double numValue,
                               const DicomCodedAttribute& measurementUnits) :
    ::gdcmIO::container::sr::DicomSRNode(codedAttribute, "NUM", relationship), m_numValue(numValue),
    m_measurementUnits(measurementUnits)
{
}

//------------------------------------------------------------------------------

DicomSRNumNode::~DicomSRNumNode()
{
}

//------------------------------------------------------------------------------

void DicomSRNumNode::write(::gdcm::DataSet &dataset) const
{
    ::gdcmIO::container::sr::DicomSRNode::write(dataset);

    // Measured Value Sequence - Type 2
    this->writeMeasuredValueSequence(dataset);
}

//------------------------------------------------------------------------------

void DicomSRNumNode::writeMeasuredValueSequence(::gdcm::DataSet &dataset) const
{
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > sequence = new ::gdcm::SequenceOfItems();
    ::gdcm::Item item;
    item.SetVLToUndefined();
    ::gdcm::DataSet& itemDataset = item.GetNestedDataSet();

    // Add numerical value - Type 1
    ::gdcmIO::helper::DicomData::setTagValues< double, 0x0040, 0xa30a >(&m_numValue, 1, itemDataset);

    // Add measured units code sequence - Type 1
    ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > codeSequence =
        this->createConceptNameCodeSequence(itemDataset, m_measurementUnits);
    ::gdcmIO::helper::DicomData::insertSQ<0x0040, 0x08ea>(codeSequence, itemDataset);

    sequence->AddItem(item);
    ::gdcmIO::helper::DicomData::setSQ< 0x0040, 0xa300 >(sequence, dataset);
}

//------------------------------------------------------------------------------

void DicomSRNumNode::print(std::ostream& os) const
{
    DicomSRNode::print(os);
    os << "\\nMeasurement units : [" << m_measurementUnits << "]";
    os << "\\nMeasurement value : [" << m_numValue << "]";
}

//------------------------------------------------------------------------------

} //namespace sr
} //namespace container
} //namespace gdcmIO
