/* ***** BEGIN LICENSE BLOCK *****
* FW4SPL - Copyright (C) IRCAD, 2009-2013.
* Distributed under the terms of the GNU Lesser General Public License (LGPL) as
* published by the Free Software Foundation.
* ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include <fwServices/macros.hpp>
#include <fwData/Acquisition.hpp>

#include <io/IWriter.hpp>
#include <io/ioTypes.hpp>

#include "ioMidas/SAcquisitionWriter.hpp"
#include "ioMidas/ExportDialog.hpp"

namespace ioMidas
{

fwServicesRegisterMacro( ::io::IWriter , ::ioMidas::SAcquisitionWriter , ::fwData::Acquisition );

//------------------------------------------------------------------------------

SAcquisitionWriter::SAcquisitionWriter() throw()
{}

//------------------------------------------------------------------------------

SAcquisitionWriter::~SAcquisitionWriter() throw()
{}

//------------------------------------------------------------------------------

void SAcquisitionWriter::configuring() throw(::fwTools::Failed)
{
    ::io::IWriter::configuring();

    typedef SPTR(::fwRuntime::ConfigurationElement) ConfigurationElement;
    typedef std::vector < ConfigurationElement >    ConfigurationElementContainer;

    ConfigurationElementContainer version = m_configuration->find("serverVersion");
    SLM_ASSERT("The <serverVersion> element can be set at most once.", version.size() <= 1 );

    std::string versionStr;
    if (version.size() == 1)
    {
        versionStr = version.at(0)->getValue();
    }
    else
    {
        versionStr = "3.2.6";
    }

    m_config = ::midasIO::configurationFactory::New(versionStr);
    SLM_ASSERT("Failed to create Midas configuration using factory with key '" << versionStr << "'", m_config);

    ConfigurationElementContainer url = m_configuration->find("url");
    SLM_ASSERT("The <url> element can be set once.", url.size() == 1 );
    if (url.size() == 1)
    {
        m_config->setUrl(url.at(0)->getValue());
    }

    ConfigurationElementContainer appName = m_configuration->find("appName");
    SLM_ASSERT("The <appName> element can be set at most once.", appName.size() <= 1 );
    if (appName.size() == 1)
    {
        m_config->setAppName(appName.at(0)->getValue());
    }

    ConfigurationElementContainer license = m_configuration->find("license");
    SLM_ASSERT("The <license> element can be set at most once.", license.size() <= 1 );
    if (license.size() == 1)
    {
        const int lic = ::boost::lexical_cast< int >(license.at(0)->getValue());
        const bool licFound
            = ::midasIO::IConfiguration::s_LICENSES.find(lic) != ::midasIO::IConfiguration::s_LICENSES.end();

        if(licFound)
        {
            m_config->setDefaultLicense(lic);
        }

        OSLM_ASSERT("<license> attribute value '" << lic << "' is not allowed", licFound);
    }


    ConfigurationElementContainer privacy = m_configuration->find("privacy");
    SLM_ASSERT("The <privacy> element can be set at most once.", privacy.size() <= 1 );
    if (privacy.size() == 1)
    {
        const std::string& privacyStr = privacy.at(0)->getValue();
        if(privacyStr == "Public")
        {
            m_config->setPrivacy(::midasIO::IConfiguration::PUBLIC);
        }
        else if(privacyStr == "Parent")
        {
            m_config->setPrivacy(::midasIO::IConfiguration::PARENT);
        }
        else
        {
            m_config->setPrivacy(::midasIO::IConfiguration::PRIVATE);
        }
    }

    ConfigurationElementContainer rootFolder = m_configuration->find("rootFolder");
    SLM_ASSERT("The <rootFolder> element can be set at most once.", rootFolder.size() <= 1 );
    if (rootFolder.size() == 1)
    {
        m_config->setRootFolderName(rootFolder.at(0)->getValue());
    }
}

//------------------------------------------------------------------------------

void SAcquisitionWriter::configureWithIHM()
{
    ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition>();
    SLM_ASSERT("Failed to retrieve selected acquisition", acq);

    ::ioMidas::ExportDialog dialog(m_config, acq);
    dialog.exec();
}

//------------------------------------------------------------------------------

void SAcquisitionWriter::starting() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void SAcquisitionWriter::updating() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void SAcquisitionWriter::stopping() throw(::fwTools::Failed)
{}

//------------------------------------------------------------------------------

void SAcquisitionWriter::info(std::ostream &_sstream )
{
    _sstream << "SAcquisitionWriter::info";
}

//------------------------------------------------------------------------------

::io::IOPathType SAcquisitionWriter::getIOPathType() const
{
    return ::io::FILES;
}

//------------------------------------------------------------------------------

std::vector< std::string > SAcquisitionWriter::getSupportedExtensions()
{
    std::vector< std::string > extensions ;
    return extensions ;
}

//------------------------------------------------------------------------------

std::string SAcquisitionWriter::getSelectorDialogTitle()
{
    return "Export acquisition to Midas";
}

//------------------------------------------------------------------------------

} // namespace ioMidas

