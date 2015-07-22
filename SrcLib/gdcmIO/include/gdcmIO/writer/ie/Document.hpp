/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_WRITER_IE_DOCUMENT_HPP__
#define __GDCMIO_WRITER_IE_DOCUMENT_HPP__

#include <fwData/Image.hpp>

#include "gdcmIO/container/sr/DicomSRNode.hpp"
#include "gdcmIO/writer/ie/InformationEntity.hpp"

namespace gdcmIO
{
namespace writer
{
namespace ie
{

/**
 * @class Document
 * @brief Document Information Entity class
 */
class GDCMIO_CLASS_API Document : public ::gdcmIO::writer::ie::InformationEntity< ::fwData::Image >
{

public:
    /**
     * @brief Constructor
     * @param[in] writer GDCM writer that must be enriched
     * @param[in] instance DICOM instance used to share information between modules
     * @param[in] image Image data
     */
    GDCMIO_API Document(SPTR(::gdcm::Writer)writer,
                        SPTR(::gdcmIO::container::DicomInstance)instance,
                        ::fwData::Image::sptr image,
                        bool use3DSR = false);

    /// Destructor
    GDCMIO_API virtual ~Document();

    /**
     * @brief Write SR Document General Module tags
     * @see PS 3.3 C.17.2
     */
    GDCMIO_API virtual void writeSRDocumentGeneralModule();

    /**
     * @brief Write SR Document Content Module tags
     * @see PS 3.3 C.17.3
     */
    GDCMIO_API virtual void writeSRDocumentContentModule();

    /**
     * @brief Write SOP Common Module tags
     * @see PS 3.3 C.12.1
     */
    GDCMIO_API void writeSOPCommonModule();

protected:

    /**
     * @brief Write Pertinent Other Evidence Sequence (0040,A385)
     */
    void writePertinentOtherEvidenceSequence();

    /// True if we must use 3DSR
    bool m_use3DSR;

};

} // namespace ie
} // namespace writer
} // namespace gdcmIO

#endif /* __GDCMIO_WRITER_IE_DOCUMENT_HPP__ */
