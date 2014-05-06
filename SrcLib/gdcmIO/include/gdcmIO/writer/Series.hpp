/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_WRITER_SERIES_HPP__
#define __GDCMIO_WRITER_SERIES_HPP__

#include <fwTools/ProgressAdviser.hpp>

#include <fwDataIO/writer/GenericObjectWriter.hpp>

#include <fwMedData/Series.hpp>
#include <fwMedData/ImageSeries.hpp>

#include <fwData/location/Folder.hpp>

#include "gdcmIO/container/DicomInstance.hpp"
#include "gdcmIO/exception/Failed.hpp"
#include "gdcmIO/config.hpp"

namespace gdcmIO
{
namespace writer
{

/**
 * @class Series
 * @brief This class handles DICOM files writing.
 * It defines needs of data storage and delegates writing to appropriate tools.
 * It manages all related data of one patient.
 */
class GDCMIO_CLASS_API Series: public ::fwDataIO::writer::GenericObjectWriter< ::fwMedData::Series >,
        public ::fwData::location::enableFolder< ::fwDataIO::writer::IObjectWriter >,
        public ::fwTools::ProgressAdviser
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (Series)( ::fwDataIO::writer::GenericObjectWriter< ::fwMedData::Series >),
            (()), ::fwDataIO::writer::factory::New< Series >);

    typedef enum
    {
        SPATIAL_FIDUCIALS = 1,  /*! Export Fiducials using SpatialFiducialsIOD */
        COMPREHENSIVE_SR = 2,   /*! Export Fiducials using ComprehensiveSRIOD */
        COMPREHENSIVE_3D_SR = 3 /*! Export Fiducials using Comprehensive3DSRIOD */
    } FiducialsExportMode;

    typedef std::map< std::string, SPTR(::gdcmIO::container::DicomInstance) > DicomInstanceMapType;

    /// Constructor
    GDCMIO_API Series(::fwDataIO::writer::IObjectWriter::Key key);

    /// Destructor
    GDCMIO_API ~Series();

    /**
     * @brief Load and start appropriate writing tools.
     * In addition, it creates an adapted folder tree where DICOM files will be stored.
     * It manages writing of the following general DICOM modules : patient, study and series
     * @note Currently, it handles image, SR and RT document writing.
     */
    GDCMIO_API void write() throw (::gdcmIO::exception::Failed);

    /**
     * Override
     * @brief Do nothing
     */
    GDCMIO_API std::string extension();

    GDCMIO_API fwGettersSettersDocMacro(FiducialsExportMode, fiducialsExportMode, FiducialsExportMode, Fiducials Export Mode);

private:

    /**
     * @brief Check if there is comment on acquisition.
     * @param[in] series ImageSeries that must be checked.
     */
    bool hasDocumentSR(::fwMedData::ImageSeries::csptr series) const;

    /**
     * @brief Returns the image instance used to create the reconstruction
     * @note This function assume that one and only one image instance has been created prior calling this function.
     * The created image instance must be the one used to generate the reconstruction.
     */
    SPTR(::gdcmIO::container::DicomInstance) getImageInstance();

    /// Dicom Instance map used to keep information between series writing calls
    DicomInstanceMapType m_dicomInstanceMap;

    /// Fiducials Export Mode
    FiducialsExportMode m_fiducialsExportMode;

};

} // namespace writer
} // namespace gdcmIO

#endif // __GDCMIO_WRITER_SERIES_HPP__
