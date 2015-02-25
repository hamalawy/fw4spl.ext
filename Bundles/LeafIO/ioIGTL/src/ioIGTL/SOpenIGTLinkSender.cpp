/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>
#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include "ioIGTL/SOpenIGTLinkSender.hpp"

#include <fwData/Object.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/ObjectMsg.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwCore/spyLog.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>

fwServicesRegisterMacro (::ioNetwork::INetworkSender, ::ioIGTL::SOpenIGTLinkSender, ::fwData::Object);

namespace ioIGTL
{

const ::fwCom::Slots::SlotKeyType SOpenIGTLinkSender::s_UPDATE_CONFIGURATION_SLOT = "updateConfiguration";
const ::fwCom::Slots::SlotKeyType SOpenIGTLinkSender::s_START_SENDING_SLOT        = "startSending";
const ::fwCom::Slots::SlotKeyType SOpenIGTLinkSender::s_STOP_SENDING_SLOT         = "stopSending";

//-----------------------------------------------------------------------------

SOpenIGTLinkSender::SOpenIGTLinkSender() :
    ioNetwork::INetworkSender(),
    m_port(4242),
    m_isSending(false)
{
    m_updateConfigurationSlot = ::fwCom::newSlot (&SOpenIGTLinkSender::updateConfiguration, this);
    m_startSendingSlot        = ::fwCom::newSlot (&SOpenIGTLinkSender::startSending, this);
    m_stopSendingSlot         = ::fwCom::newSlot (&SOpenIGTLinkSender::stopSending, this);
    ::fwCom::HasSlots::m_slots (SOpenIGTLinkSender::s_UPDATE_CONFIGURATION_SLOT, m_updateConfigurationSlot )
        (SOpenIGTLinkSender::s_START_SENDING_SLOT, m_startSendingSlot )
        (SOpenIGTLinkSender::s_STOP_SENDING_SLOT, m_stopSendingSlot );

    ::fwCom::HasSlots::m_slots.setWorker (m_associatedWorker);
    m_server = ::igtlNetwork::Server::sptr (new ::igtlNetwork::Server());
}

//-----------------------------------------------------------------------------

SOpenIGTLinkSender::~SOpenIGTLinkSender()
{
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::configuring() throw (::fwTools::Failed)
{

    if (m_configuration != NULL && m_configuration->findConfigurationElement ("port"))
    {
        m_port = ::boost::lexical_cast< ::boost::uint16_t > (
            m_configuration->findConfigurationElement ("port")->getValue());
    }
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::updateConfiguration(::boost::uint16_t const port)
{
    m_port = port;
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::setPort(boost::uint16_t const port) throw (::fwTools::Failed)
{
    this->updateConfiguration(port);
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::starting() throw (::fwTools::Failed)
{
    ::ioNetwork::INetworkSender::starting();
    m_serverWorker = ::fwThread::Worker::New();
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::stopping() throw (::fwTools::Failed)
{
    this->::ioNetwork::INetworkSender::stopping();
    this->stopSending();
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::updating() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::startSending()
{
    if(!m_isSending)
    {
        ::boost::function<void() > task = ::boost::bind (&::igtlNetwork::Server::runServer, m_server);
        try
        {
            m_server->start (m_port);
            m_serverWorker->post(task)
            fwServicesNotifyMacro(this->getLightID(), m_sigServerStarted, ());
            m_isSending = true;
        }
        catch (::fwCore::Exception &e)
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog ("Error", "Cannot start the server: " +
                                                               std::string(e.what()),
                                                               ::fwGui::dialog::IMessageDialog::CRITICAL);
        }
    }
}

//-----------------------------------------------------------------------------

void SOpenIGTLinkSender::stopSending()
{
    if(m_isSending)
    {
        ::fwGui::dialog::MessageDialog msgDialog;

        try
        {
            m_server->stop();
            m_serverWorker->stop();
            fwServicesNotifyMacro(this->getLightID(), m_sigServerStopped, ());
            m_isSending = false;
        }
        catch (::fwCore::Exception &e)
        {
            msgDialog.showMessageDialog ("Error", e.what());
        }
    }
}

//-----------------------------------------------------------------------------
void SOpenIGTLinkSender::sendObject(const ::fwData::Object::sptr &obj)
{
    m_server->broadcast(obj);
}

//-----------------------------------------------------------------------------

} // namespace ioIGTL


