/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
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
#include <fwGui/Cursor.hpp>

#include <io/IWriter.hpp>

#include <fwDicomData/DicomSeries.hpp>
#include <fwData/location/Folder.hpp>

#include <fwDicomIOExt/gdcm/DicomSeriesWriter.hpp>

#include "ioDicomExt/gdcm/SDicomSeriesWriter.hpp"


namespace ioDicomExt
{

namespace gdcm
{

fwServicesRegisterMacro( ::io::IWriter , ::ioDicomExt::gdcm::SDicomSeriesWriter , ::fwDicomData::DicomSeries ) ;

//------------------------------------------------------------------------------

SDicomSeriesWriter::SDicomSeriesWriter() throw()
{}

//------------------------------------------------------------------------------

SDicomSeriesWriter::~SDicomSeriesWriter() throw()
{}

//------------------------------------------------------------------------------

void SDicomSeriesWriter::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Choose a directory for DICOM images");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr  result;
    result= ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result)
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

void SDicomSeriesWriter::starting() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void SDicomSeriesWriter::stopping() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void SDicomSeriesWriter::updating() throw(::fwTools::Failed)
{
    if( this->hasLocationDefined() )
    {
        // Retrieve dataStruct associated with this service
        ::fwDicomData::DicomSeries::sptr series = this->getObject< ::fwDicomData::DicomSeries >();
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

        if (series->getModality() == "OT")
        {
            ::fwGui::dialog::MessageDialog dialog;
            dialog.setMessage("Series modality is '" + series->getModality() + "' some information can be lost."
                              "\nDo you want to continue ?");
            dialog.setTitle("Series modality.");
            dialog.setIcon(::fwGui::dialog::MessageDialog::QUESTION);
            dialog.addButton( ::fwGui::dialog::MessageDialog::YES_NO );
            ::fwGui::dialog::MessageDialog::Buttons button = dialog.show();

            if(button == ::fwGui::dialog::MessageDialog::NO)
            {
                return;
            }
        }
        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);
        this->saveDicomSeries(folder, series);
        cursor.setDefaultCursor();
    }
}


//------------------------------------------------------------------------------

void SDicomSeriesWriter::saveDicomSeries( const ::boost::filesystem::path folder,
        ::fwDicomData::DicomSeries::sptr series ) const
{
    ::fwDicomIOExt::gdcm::DicomSeriesWriter::sptr writer = ::fwDicomIOExt::gdcm::DicomSeriesWriter::New();

    writer->setObject(series);
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

::io::IOPathType SDicomSeriesWriter::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

} // namespace gdcm
} // namespace ioDicomExt
