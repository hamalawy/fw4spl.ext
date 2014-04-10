/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/bind.hpp>

#include <fwCore/base.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <io/IReader.hpp>

#include <fwComEd/SeriesDBMsg.hpp>

#include <fwMedData/SeriesDB.hpp>

#include <fwDicomIOExt/gdcm/DicomSeriesDBReader.hpp>

#include "ioDicomExt/gdcm/SDicomSeriesDBReader.hpp"


namespace ioDicomExt
{

namespace gdcm
{

fwServicesRegisterMacro( ::io::IReader , ::ioDicomExt::gdcm::SDicomSeriesDBReader , ::fwMedData::SeriesDB ) ;

//------------------------------------------------------------------------------

SDicomSeriesDBReader::SDicomSeriesDBReader() throw()
{}

//------------------------------------------------------------------------------

SDicomSeriesDBReader::~SDicomSeriesDBReader() throw()
{}

//------------------------------------------------------------------------------

void SDicomSeriesDBReader::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle(this->getSelectorDialogTitle());
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr  result;
    result= ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getFolder();
        this->setFolder( result->getFolder() );
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    }
}

//------------------------------------------------------------------------------

void SDicomSeriesDBReader::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SDicomSeriesDBReader::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SDicomSeriesDBReader::info(std::ostream &_sstream )
{
    _sstream << "SDicomSeriesDBReader::info" ;
}

//------------------------------------------------------------------------------

SDicomSeriesDBReader::ExtensionsType SDicomSeriesDBReader::getSupportedExtensions()
{
    ExtensionsType extensions ;
    return extensions ;
}

//------------------------------------------------------------------------------

std::string SDicomSeriesDBReader::getSelectorDialogTitle()
{
    return "Choose a directory with DICOM images";
}


//------------------------------------------------------------------------------

::fwMedData::SeriesDB::sptr SDicomSeriesDBReader::createSeriesDB(const ::boost::filesystem::path& dicomDir) const
{
    SLM_TRACE_FUNC();
    ::fwDicomIOExt::gdcm::DicomSeriesDBReader::sptr myLoader = ::fwDicomIOExt::gdcm::DicomSeriesDBReader::New();
    ::fwMedData::SeriesDB::sptr dummy = ::fwMedData::SeriesDB::New();
    myLoader->setObject(dummy);
    myLoader->setFolder(dicomDir);

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Loading Dicom Series");
        myLoader->addHandler( progressMeterGUI );
        myLoader->read();
    }
    catch (const std::exception & e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Warning", "Warning during loading", ::fwGui::dialog::IMessageDialog::WARNING);
    }

    return myLoader->getConcreteObject();
}

//------------------------------------------------------------------------------

void SDicomSeriesDBReader::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    if( this->hasLocationDefined() )
    {
        ::fwMedData::SeriesDB::sptr seriesDB = createSeriesDB( this->getFolder() );

        if( seriesDB->size() > 0 )
        {
            // Retrieve dataStruct associated with this service
            ::fwMedData::SeriesDB::sptr associatedSeriesDB = this->getObject< ::fwMedData::SeriesDB >();
            SLM_ASSERT("associated SeriesDB not instanced", associatedSeriesDB);
            associatedSeriesDB->shallowCopy( seriesDB ) ;

            ::fwGui::Cursor cursor;
            cursor.setCursor(::fwGui::ICursor::BUSY);
            this->notificationOfDBUpdate();
            cursor.setDefaultCursor();
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                    "Dicom Series Reader","This file can not be read. Retry with another file reader.", ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

void SDicomSeriesDBReader::notificationOfDBUpdate()
{
    SLM_TRACE_FUNC();
    ::fwMedData::SeriesDB::sptr seriesDB = this->getObject< ::fwMedData::SeriesDB >() ;
    SLM_ASSERT("Unable to get seriesDB", seriesDB);

    ::fwComEd::SeriesDBMsg::sptr msg = ::fwComEd::SeriesDBMsg::New();
    BOOST_FOREACH(const ::fwMedData::Series::sptr& s, seriesDB->getContainer() )
    {
        msg->appendAddedSeries(s);
    }

    ::fwServices::IEditionService::notify(this->getSptr(),  seriesDB, msg);
}

//-----------------------------------------------------------------------------

::io::IOPathType SDicomSeriesDBReader::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

} // namespace gdcm
} // namespace ioDicomExt
