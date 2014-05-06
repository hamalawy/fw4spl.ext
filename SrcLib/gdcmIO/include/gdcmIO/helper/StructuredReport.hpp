/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_HELPER_STRUCTUREDREPORT_HPP__
#define __GDCMIO_HELPER_STRUCTUREDREPORT_HPP__

#include <gdcmDataSet.h>
#include <gdcmSequenceOfItems.h>

#include "gdcmIO/helper/DicomData.hpp"
#include "gdcmIO/container/sr/DicomSRNode.hpp"
#include "gdcmIO/container/sr/DicomSRContainerNode.hpp"

namespace gdcmIO
{

namespace helper
{

/**
 * @class StructuredReport
 * @brief This class contains helpers for DICOM Structured Reporting (SR).
 */
class GDCMIO_CLASS_API StructuredReport
{
public:

    /**
     * @brief Read a Structured Report
     * @param[in] dataset Dataset from which the SR must be created
     */
    GDCMIO_API static SPTR(::gdcmIO::container::sr::DicomSRContainerNode) readSR(const ::gdcm::DataSet& dataset);

    /**
     * @brief Dump the SR in graphviz format
     * @param[in] root Root node
     * @param[in] out Destination stream
     */
    GDCMIO_API static void dumpSR(const SPTR(::gdcmIO::container::sr::DicomSRNode)& root, std::ostream& out = std::cout);

protected:

    /**
     * @brief Read all sub nodes and add it as children to the parent node
     * @param[in] dataset Dataset from which the sub nodes must been read
     * @param[in] parent Parent node
     */
    static void readSubNodeContainer(const ::gdcm::DataSet& dataset, SPTR(::gdcmIO::container::sr::DicomSRNode) parent);

    /**
     * @brief Read a sub node and add it as a child to the parent node
     * @param[in] dataset Dataset from which the sub node must been read
     * @param[in] parent Parent node
     */
    static void readSubNode(const ::gdcm::DataSet& dataset, SPTR(::gdcmIO::container::sr::DicomSRNode) parent);

    /**
     * @brief Dump an SR node in graphviz format
     * @param[in] node Node
     * @param[in] out Destination stream
     * @param[in] index Node index
     */
    GDCMIO_API static void dumpSRNode(const SPTR(::gdcmIO::container::sr::DicomSRNode)& node, std::ostream& out, int& index);

    /**
     * @brief Read content of a code sequence (eg : Concept Name Code Sequence, ...)
     * @note This method can return a DicomCodedAttribute with empty members.
     * @param[in] dataset   Data set where the code sequence is supposed to be.
     * @tparam GROUP Tag group of the code sequence.
     * @tparam ELEMENT Element group of the code sequence.
     */
    template <uint16_t GROUP, uint16_t ELEMENT>
    static ::gdcmIO::container::DicomCodedAttribute readCodeSequence(const ::gdcm::DataSet & dataset)
    {
        ::gdcmIO::container::DicomCodedAttribute codedAttributes;

        if ( !dataset.FindDataElement(::gdcm::Tag(GROUP,ELEMENT)))
        {
            // Return empty coded attributes
            return codedAttributes;
        }

        ::gdcm::SmartPointer< ::gdcm::SequenceOfItems > sequence =
                dataset.GetDataElement(::gdcm::Tag(GROUP,ELEMENT)).GetValueAsSQ();
        if (sequence->GetNumberOfItems() == 0)  // One Item shall be permitted
        {
            // Return empty coded attributes
            return codedAttributes;
        }

        const ::gdcm::DataSet& itemDataset = sequence->GetItem(1).GetNestedDataSet();

        // Code value - Type 1
        codedAttributes.setCodeValue(DicomData::getTrimmedTagValue<0x0008,0x0100>(itemDataset));

        // Coding Scheme Designator - Type 1
        codedAttributes.setCodingSchemeDesignator(DicomData::getTrimmedTagValue<0x0008,0x0102>(itemDataset));

        // Coding Scheme Version - Type 1C
        codedAttributes.setCodingSchemeVersion(DicomData::getTrimmedTagValue<0x0008,0x0103>(itemDataset));

        // Code Meaning - Type 1
        codedAttributes.setCodeMeaning(DicomData::getTrimmedTagValue<0x0008,0x0104>(itemDataset));

        return codedAttributes;
    }
};

} // namespace helper
} // namespace gdcmIO

#endif /*__GDCMIO_HELPER_STRUCTUREDREPORT_HPP__*/
