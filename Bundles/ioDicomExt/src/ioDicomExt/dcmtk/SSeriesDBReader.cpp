/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/bind.hpp>

#include <fwCore/base.hpp>

#include <fwData/String.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/registry/ServiceConfig.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <fwGui/dialog/ProgressDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/Cursor.hpp>

#include <io/IReader.hpp>

#include <fwComEd/SeriesDBMsg.hpp>

#include <fwMedData/SeriesDB.hpp>

#include <fwDicomIOExt/dcmtk/SeriesDBReader.hpp>

#include "ioDicomExt/dcmtk/SSeriesDBReader.hpp"


namespace ioDicomExt
{

namespace dcmtk
{

fwServicesRegisterMacro( ::io::IReader , ::ioDicomExt::dcmtk::SSeriesDBReader , ::fwMedData::SeriesDB ) ;

//------------------------------------------------------------------------------

SSeriesDBReader::SSeriesDBReader() throw()
{}

//------------------------------------------------------------------------------

SSeriesDBReader::~SSeriesDBReader() throw()
{}

//------------------------------------------------------------------------------

void SSeriesDBReader::configureWithIHM()
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

    // Select filter
    if(!m_filterSelectorSrvConfig.empty())
    {
        // Get the config
        ::fwRuntime::ConfigurationElement::csptr filterSelectorConfig;
        filterSelectorConfig = ::fwServices::registry::ServiceConfig::getDefault()->getServiceConfig(
                m_filterSelectorSrvConfig , "::ioDicomExt::dcmtk::editor::SFilterSelectorDialog");

        SLM_ASSERT("Sorry, there is no service configuration "
                       + m_filterSelectorSrvConfig
                       + " for ::ioDicomExt::dcmtk::editor::SFilterSelectorDialog", filterSelectorConfig);

        // Init and execute the service
        ::fwServices::IService::sptr filterSelectorSrv;
        ::fwData::String::sptr key = ::fwData::String::New();
        filterSelectorSrv = ::fwServices::add(key,
                                          "::gui::editor::IDialogEditor",
                                          "::ioDicomExt::dcmtk::editor::SFilterSelectorDialog");
        filterSelectorSrv->setConfiguration( ::fwRuntime::ConfigurationElement::constCast(filterSelectorConfig) ) ;
        filterSelectorSrv->configure() ;
        filterSelectorSrv->start();
        filterSelectorSrv->update();
        filterSelectorSrv->stop();
        ::fwServices::OSR::unregisterService( filterSelectorSrv );

        m_filterType = key->getValue();

    }
}

//------------------------------------------------------------------------------

void SSeriesDBReader::configuring() throw (fwTools::Failed)
{
    ::io::IReader::configuring();

    // Use filter selector
    ::fwRuntime::ConfigurationElement::sptr selectorConfig =
            m_configuration->findConfigurationElement("FilterSelectorSrvConfig");
    if(selectorConfig)
    {
        SLM_ASSERT("Missing 'name' attribute", selectorConfig->hasAttribute("name"));
        m_filterSelectorSrvConfig = selectorConfig->getAttributeValue("name");
    }

    // Set filter
    ::fwRuntime::ConfigurationElement::sptr config = m_configuration->findConfigurationElement("config");
    if(config)
    {
        m_filterType = config->getAttributeValue("filterType");
    }
}

//------------------------------------------------------------------------------

void SSeriesDBReader::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SSeriesDBReader::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SSeriesDBReader::info(std::ostream &_sstream )
{
    _sstream << "SSeriesDBReader::info" ;
}

//------------------------------------------------------------------------------

SSeriesDBReader::ExtensionsType SSeriesDBReader::getSupportedExtensions()
{
    ExtensionsType extensions ;
    return extensions ;
}

//------------------------------------------------------------------------------

std::string SSeriesDBReader::getSelectorDialogTitle()
{
    return "Choose a directory with DICOM images";
}


//------------------------------------------------------------------------------

::fwMedData::SeriesDB::sptr SSeriesDBReader::createSeriesDB(const ::boost::filesystem::path& dicomDir)
{
    SLM_TRACE_FUNC();
    ::fwDicomIOExt::dcmtk::SeriesDBReader::sptr myLoader = ::fwDicomIOExt::dcmtk::SeriesDBReader::New();
    ::fwMedData::SeriesDB::sptr dummy = ::fwMedData::SeriesDB::New();
    myLoader->setObject(dummy);
    myLoader->setFolder(dicomDir);
    myLoader->setDicomFilterType(m_filterType);

    if(myLoader->isDicomDirAvailable())
    {
        ::fwGui::dialog::MessageDialog messageBox;
        messageBox.setTitle("Dicomdir file");
        messageBox.setMessage( "There is a dicomdir file in the root folder. "
                "Would you like to use it for the reading process ?" );
        messageBox.setIcon(::fwGui::dialog::IMessageDialog::QUESTION);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::YES_NO);
        ::fwGui::dialog::IMessageDialog::Buttons button = messageBox.show();

        myLoader->setDicomdirActivated(button == ::fwGui::dialog::IMessageDialog::YES);
    }

    try
    {
        ::fwGui::dialog::ProgressDialog progressMeterGUI("Loading Dicom Image");
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

void SSeriesDBReader::updating() throw(::fwTools::Failed)
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
                    "Image Reader","This file can not be read. Retry with another file reader.",
                    ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

void SSeriesDBReader::notificationOfDBUpdate()
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

::io::IOPathType SSeriesDBReader::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

} // namespace dcmtk
} // namespace ioDicomExt
