/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "ioIGTL/SImageNetworkWriter.hpp"

#include <fwData/Image.hpp>
#include <fwServices/Base.hpp>
#include <fwGui/dialog/InputDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

fwServicesRegisterMacro (::io::IWriter, ::ioIGTL::SImageNetworkWriter, ::fwData::Image);

namespace ioIGTL
{

//-----------------------------------------------------------------------------

SImageNetworkWriter::SImageNetworkWriter()
{
}

//-----------------------------------------------------------------------------

SImageNetworkWriter::~SImageNetworkWriter() throw()
{
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::configuring() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::starting() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::stopping() throw (::fwTools::Failed)
{
    if (m_server.isStarted())
    {
        m_server.stop();
    }
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::configureWithIHM()
{
    std::string portStr;
    ::fwGui::dialog::MessageDialog msgDialog;
    ::fwGui::dialog::InputDialog inputDialog;


    inputDialog.setTitle ("Enter server port");
    portStr = inputDialog.getInput();
    try
    {
        m_port        = ::boost::lexical_cast< ::boost::uint16_t> (portStr);
        m_isValidPort = true;
    }
    catch (boost::bad_lexical_cast &)
    {
        msgDialog.showMessageDialog("Invalid input", "Invalid port");
        m_isValidPort = false;
    }
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::updating() throw (::fwTools::Failed)
{
    ::fwData::Image::sptr img;
    ::fwGui::dialog::MessageDialog msgDialog;
    ::igtlNetwork::Client::sptr newClient;

    if (m_isValidPort)
    {
        img = this->getObject< ::fwData::Image >();
        try
        {
            m_server.start (m_port);
            newClient = m_server.waitForConnection();
            if (newClient != NULL)
            {
                newClient->sendObject (img);
            }
            newClient->disconnect();
        }
        catch (boost::bad_lexical_cast &)
        {
            msgDialog.showMessageDialog ("Bad port", "port is incorrect");
        }
        catch (::fwCore::Exception &err)
        {
            msgDialog.showMessageDialog("Error ", std::string(err.what()));
        }
    }
}

//-----------------------------------------------------------------------------

::io::IOPathType SImageNetworkWriter::getIOPathType() const
{
    return io::TYPE_NOT_DEFINED;
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::receiving (::fwServices::ObjectMsg::csptr _msg) throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SImageNetworkWriter::swapping() throw (::fwTools::Failed)
{
    // Classic default approach to update service when object change
    this->stopping();
    this->starting();
}

//-----------------------------------------------------------------------------

} // namespace ioIGTL


