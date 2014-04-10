/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_WRITER_SERIESDB_HPP__
#define __GDCMIO_WRITER_SERIESDB_HPP__

#include <fwTools/ProgressAdviser.hpp>

#include <fwMedData/Series.hpp>
#include <fwMedData/SeriesDB.hpp>
#include <fwData/location/Folder.hpp>

#include <fwDataIO/writer/GenericObjectWriter.hpp>

#include "gdcmIO/config.hpp"

namespace gdcmIO
{

namespace writer
{

/**
 * @class   SeriesDB
 * @brief   This class manages patient writing, in DICOM file format.
 */
class SeriesDB: public ::fwDataIO::writer::GenericObjectWriter< ::fwMedData::SeriesDB >,
        public ::fwData::location::enableFolder< ::fwDataIO::writer::IObjectWriter >,
        public ::fwTools::ProgressAdviser
{

public:

    fwCoreClassDefinitionsWithFactoryMacro(
            (SeriesDB)( ::fwDataIO::writer::GenericObjectWriter< ::fwMedData::SeriesDB >),
            (()), ::fwDataIO::writer::factory::New< SeriesDB >);

    /// Constructor
    GDCMIO_API SeriesDB(::fwDataIO::writer::IObjectWriter::Key key);

    /// Destructor
    GDCMIO_API ~SeriesDB();

    /**
     * @brief Manage writing tools to save all patients.
     * It launches DicomGlobalManager for each patient.
     */
    GDCMIO_API void write();

    /**
     * Override
     * @brief Do nothing
     */
    GDCMIO_API std::string extension();

protected:
    /**
     * @brief Function used to sort Series
     * @param[in] a First Series
     * @param[in] b Second Series
     */
    static bool seriesComparator(SPTR(::fwMedData::Series) a, SPTR(::fwMedData::Series) b);
};

} // namespace writer

} // namespace gdcmIO

#endif // __GDCMIO_WRITER_SERIESDB_HPP__
