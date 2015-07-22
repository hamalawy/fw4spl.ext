/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>           // for ProgessHandler
#include <boost/lambda/lambda.hpp>  // for ProgessHandler
#include <fwCore/base.hpp>

#include <fwTools/Stringizer.hpp>

#include <fwMedData/Series.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwDataIO/writer/registry/macros.hpp>

#include "gdcmIO/writer/SeriesDB.hpp"

fwDataIOWriterRegisterMacro(::gdcmIO::writer::SeriesDB);

namespace gdcmIO
{

namespace writer
{

//------------------------------------------------------------------------------

SeriesDB::SeriesDB(::fwDataIO::writer::IObjectWriter::Key key) :
    ::fwData::location::enableFolder< ::fwDataIO::writer::IObjectWriter >(this),
    m_fiducialsExportMode(::gdcmIO::writer::Series::SPATIAL_FIDUCIALS)
{
}

//------------------------------------------------------------------------------

SeriesDB::~SeriesDB()
{
}

//------------------------------------------------------------------------------

void SeriesDB::write()
{
//    // Disable GDCM Warnings
//    ::gdcm::Trace::SetWarning(false);

    ::fwMedData::SeriesDB::sptr seriesDB = this->getConcreteObject();
    SLM_ASSERT("SeriesDB not instanced", seriesDB);

    ::gdcmIO::writer::Series::sptr writer = ::gdcmIO::writer::Series::New();
    writer->setFiducialsExportMode(m_fiducialsExportMode);

    // Copy and sort container in order to write ImageSeries before ModelSeries
    ::fwMedData::SeriesDB::ContainerType seriesContainer = seriesDB->getContainer();
    std::sort (seriesContainer.begin(), seriesContainer.end(), SeriesDB::seriesComparator);

    // Write all patients
    BOOST_FOREACH( ::fwMedData::Series::sptr series, seriesContainer)
    {
        // Create a new directory
        const ::boost::filesystem::path& seriesPath = this->getFolder() / series->getInstanceUID();
        ::boost::filesystem::create_directory(seriesPath);
        writer->setObject(series);
        writer->setFolder(seriesPath);

        // Forward event progress to its parents
        ::fwTools::ProgressAdviser::ProgessHandler handler =
            ::boost::bind( &Series::notifyProgress, this, ::boost::lambda::_1, ::boost::lambda::_2);
        writer->addHandler(handler);

        // Write a series
        writer->write();
    }
}

//------------------------------------------------------------------------------

std::string SeriesDB::extension()
{
    return std::string("");
}

//------------------------------------------------------------------------------

bool SeriesDB::seriesComparator(SPTR(::fwMedData::Series)a, SPTR(::fwMedData::Series)b)
{
    ::fwMedData::ModelSeries::sptr ma = ::fwMedData::ModelSeries::dynamicCast(a);
    ::fwMedData::ModelSeries::sptr mb = ::fwMedData::ModelSeries::dynamicCast(b);
    return (mb && !ma);
}

} // namespace writer

} // namespace gdcmIO
