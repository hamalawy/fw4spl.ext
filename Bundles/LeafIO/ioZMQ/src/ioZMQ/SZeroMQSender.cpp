/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include "ioZMQ/SZeroMQSender.hpp"
#include "ioZMQ/ZeroMQConfigurationParser.hpp"
#include "ioZMQ/Patterns.hpp"

#include <fwData/Object.hpp>
#include <fwCore/spyLog.hpp>
#include <fwServices/Base.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <sstream>

fwServicesRegisterMacro (::ioNetwork::INetworkSender, ::ioZMQ::SZeroMQSender, ::fwData::Object);


namespace ioZMQ
{

const ::fwCom::Slots::SlotKeyType SZeroMQSender::s_UPDATE_CONFIGURATION_SLOT = "updateConfiguration";

//-----------------------------------------------------------------------------

SZeroMQSender::SZeroMQSender() :
    ::ioNetwork::INetworkSender()
{
    m_updateConfigurationSlot = ::fwCom::newSlot (&SZeroMQSender::updateConfiguration, this);
    ::fwCom::HasSlots::m_slots (SZeroMQSender::s_UPDATE_CONFIGURATION_SLOT, m_updateConfigurationSlot);

    ::fwCom::HasSlots::m_slots.setWorker(m_associatedWorker);
}

//-----------------------------------------------------------------------------

SZeroMQSender::~SZeroMQSender()
{
}

//-----------------------------------------------------------------------------

void SZeroMQSender::configuring() throw (::fwTools::Failed)
{
    ZeroMQConfigurationParser parser (m_configuration);

    parser.parse (Patterns::getSupportedWriterPatterns());
    m_hostStr     = parser.getHostname();
    m_sockMode    = parser.getSocketMode();
    m_patternMode = parser.getPatternMode();
}

//-----------------------------------------------------------------------------

void SZeroMQSender::setPort (boost::uint16_t const port) throw (::fwTools::Failed)
{
    std::string newHost;
    std::stringstream stream;

    if (m_hostStr.substr (0, 3) != "tcp")
    {
        throw ::fwTools::Failed ("Change port is only supported for tcp protocol");
    }
    newHost = m_hostStr.substr (0, m_hostStr.find (":", 4));
    stream << newHost << ":" << port;
    m_hostStr = stream.str();
}

//-----------------------------------------------------------------------------

void SZeroMQSender::starting() throw (::fwTools::Failed)
{
    ::fwGui::dialog::MessageDialog msgDialog;

    try
    {
        ::ioNetwork::INetworkSender::starting();
        m_socket = ::zmqNetwork::Socket::sptr (new ::zmqNetwork::Socket (m_sockMode, m_patternMode));
        m_socket->start (m_hostStr);
        fwServicesNotifyMacro (this->getLightID(), m_sigServerStarted, ());
    }
    catch (std::exception &err)
    {
        msgDialog.showMessageDialog ("Error", "Cannot start the sender : " + std::string (err.what()));
    }
}

//-----------------------------------------------------------------------------

void SZeroMQSender::updateConfiguration (::zmqNetwork::Socket::PatternMode const pattern,
                                         ::zmqNetwork::Socket::SocketMode const sockMode,
                                         std::string const &host)
{
    m_sockMode    = sockMode;
    m_patternMode = pattern;
    m_hostStr     = host;
}

//-----------------------------------------------------------------------------

void SZeroMQSender::stopping() throw (::fwTools::Failed)
{
    m_socket->stop();
    ::ioNetwork::INetworkSender::stopping();
    fwServicesNotifyMacro (this->getLightID(), m_sigServerStopped, ());
}

//-----------------------------------------------------------------------------

void SZeroMQSender::sendObject (const ::fwData::Object::sptr &obj)
{
    m_socket->sendObject (obj);
}

//-----------------------------------------------------------------------------

} // namespace ioZMQ


