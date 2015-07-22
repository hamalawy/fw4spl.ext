/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/SelectorDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <io/IWriter.hpp>

#include <fwData/location/Folder.hpp>
#include <fwMedData/Series.hpp>
#include <fwMedData/SeriesDB.hpp>

#include <fwComEd/helper/SeriesDB.hpp>

#include <gdcmIO/helper/Fiducial.hpp>
#include <gdcmIO/writer/SeriesDB.hpp>

#include "ioGdcm/SSeriesDBWriter.hpp"


namespace ioGdcm
{

fwServicesRegisterMacro( ::io::IWriter, ::ioGdcm::SSeriesDBWriter, ::fwData::Vector );

//------------------------------------------------------------------------------

SSeriesDBWriter::SSeriesDBWriter() throw() :
    m_fiducialsExportMode(::gdcmIO::writer::Series::COMPREHENSIVE_3D_SR)
{
}

//------------------------------------------------------------------------------

SSeriesDBWriter::~SSeriesDBWriter() throw()
{
}

//------------------------------------------------------------------------------

void SSeriesDBWriter::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Choose a directory for DICOM images");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr result;
    result = ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result && this->selectFiducialsExportMode())
    {
        _sDefaultPath = result->getFolder();
        this->setFolder( result->getFolder() );
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void SSeriesDBWriter::starting() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SSeriesDBWriter::stopping() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SSeriesDBWriter::updating() throw(::fwTools::Failed)
{
    if( this->hasLocationDefined() )
    {
        const ::boost::filesystem::path& folder = this->getFolder();
        if(!::boost::filesystem::is_empty(folder))
        {
            ::fwGui::dialog::MessageDialog dialog;
            dialog.setMessage("Folder '"+folder.string()+"' isn't empty, files can be overwritten."
                              "\nDo you want to continue ?");
            dialog.setTitle("Folder not empty.");
            dialog.setIcon(::fwGui::dialog::MessageDialog::QUESTION);
            dialog.addButton( ::fwGui::dialog::MessageDialog::YES_NO );
            ::fwGui::dialog::MessageDialog::Buttons button = dialog.show();

            if(button == ::fwGui::dialog::MessageDialog::NO)
            {
                return;
            }
        }

        // Retrieve dataStruct associated with this service
        ::fwData::Vector::sptr vector = this->getObject< ::fwData::Vector >();

        // Create SeriesDB
        ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();
        ::fwComEd::helper::SeriesDB seriesDBHelper(seriesDB);

        BOOST_FOREACH(::fwData::Object::sptr object, vector->getContainer())
        {
            ::fwMedData::Series::sptr series = ::fwMedData::Series::dynamicCast(object);
            SLM_ASSERT("The container should only contain series.", series);
            seriesDBHelper.add(series);
        }

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);
        this->saveSeriesDB(folder, seriesDB);
        cursor.setDefaultCursor();
    }
}


//------------------------------------------------------------------------------

void SSeriesDBWriter::saveSeriesDB( const ::boost::filesystem::path folder, ::fwMedData::SeriesDB::sptr seriesDB )
{
    ::gdcmIO::writer::SeriesDB::sptr writer = ::gdcmIO::writer::SeriesDB::New();
    writer->setObject(seriesDB);
    writer->setFiducialsExportMode(m_fiducialsExportMode);
    ::fwData::location::Folder::sptr loc = ::fwData::location::Folder::New();
    loc->setFolder(folder);
    writer->setLocation(loc);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Saving series ");
        writer->addHandler( progressMeterGUI );
        writer->write();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during saving : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog(
            "Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
            "Warning", "Warning during saving", ::fwGui::dialog::IMessageDialog::WARNING);
    }
}

//-----------------------------------------------------------------------------

::io::IOPathType SSeriesDBWriter::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

bool SSeriesDBWriter::selectFiducialsExportMode()
{
    // Retrieve dataStruct associated with this service
    ::fwData::Vector::sptr vector = this->getObject< ::fwData::Vector >();

    // Create SeriesDB
    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();
    ::fwComEd::helper::SeriesDB seriesDBHelper(seriesDB);

    BOOST_FOREACH(::fwData::Object::sptr object, vector->getContainer())
    {
        ::fwMedData::Series::sptr series = ::fwMedData::Series::dynamicCast(object);
        SLM_ASSERT("The container should only contain series.", series);
        seriesDBHelper.add(series);
    }

    const bool containsLandmarks   = ::gdcmIO::helper::Fiducial::containsLandmarks(seriesDB);
    const bool containsDistances   = ::gdcmIO::helper::Fiducial::containsDistances(seriesDB);
    const bool contains3DDistances = ::gdcmIO::helper::Fiducial::contains3DDistances(seriesDB);

    if(containsLandmarks || containsDistances)
    {
        static const std::string fiducialIOD          = "Spatial Fiducials";
        static const std::string comprehensiveSRIOD   = "Comprehensive SR";
        static const std::string comprehensive3DSRIOD = "Comprehensive 3D SR";

        std::vector< std::string > exportModes;
        if(!containsDistances)
        {
            exportModes.push_back(fiducialIOD);
        }
        if(!contains3DDistances)
        {
            exportModes.push_back(comprehensiveSRIOD);
        }
        exportModes.push_back(comprehensive3DSRIOD);

        // Create selector
        ::fwGui::dialog::SelectorDialog::sptr selector = ::fwGui::dialog::SelectorDialog::New();

        selector->setTitle("Fiducials export mode");
        selector->setSelections(exportModes);
        const std::string mode             = selector->show();
        const bool modeSelectionIsCanceled = mode.empty();

        if(mode == fiducialIOD)
        {
            m_fiducialsExportMode = ::gdcmIO::writer::Series::SPATIAL_FIDUCIALS;
        }
        else if(mode == comprehensiveSRIOD)
        {
            m_fiducialsExportMode = ::gdcmIO::writer::Series::COMPREHENSIVE_SR;
        }
        else
        {
            m_fiducialsExportMode = ::gdcmIO::writer::Series::COMPREHENSIVE_3D_SR;
        }


        return !modeSelectionIsCanceled;
    }

    return true;

}

} // namespace ioGdcm
