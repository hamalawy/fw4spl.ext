/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include "ioZMQ/SZeroMQListener.hpp"
#include "ioZMQ/ZeroMQConfigurationParser.hpp"
#include "ioZMQ/Patterns.hpp"

#include <fwData/Object.hpp>
#include <fwServices/Base.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <boost/function.hpp>
#include <sstream>

fwServicesRegisterMacro (::ioNetwork::INetworkListener, ::ioZMQ::SZeroMQListener, ::fwData::Object);


namespace ioZMQ
{

const ::fwCom::Slots::SlotKeyType SZeroMQListener::s_UPDATE_CONFIGURATION_SLOT = "updateConfiguration";

//-----------------------------------------------------------------------------

SZeroMQListener::SZeroMQListener() :
    ::ioNetwork::INetworkListener()
{
    m_updateConfigurationSlot = ::fwCom::newSlot (&SZeroMQListener::updateConfiguration, this);
    ::fwCom::HasSlots::m_slots (SZeroMQListener::s_UPDATE_CONFIGURATION_SLOT, m_updateConfigurationSlot );

    ::fwCom::HasSlots::m_slots.setWorker (m_associatedWorker);
}

//-----------------------------------------------------------------------------

SZeroMQListener::~SZeroMQListener()
{
}

//-----------------------------------------------------------------------------

void SZeroMQListener::setHost(std::string const &host, ::boost::uint16_t const port) throw (::fwTools::Failed)
{
    std::stringstream stream;

    stream << "tcp://" << host << ":" << port;
    m_hostStr = stream.str();
}

//-----------------------------------------------------------------------------

void SZeroMQListener::configuring() throw (::fwTools::Failed)
{
    ZeroMQConfigurationParser parser(m_configuration);

    parser.parse(Patterns::getSupportedReaderPatterns());
    m_hostStr     = parser.getHostname();
    m_sockMode    = parser.getSocketMode();
    m_patternMode = parser.getPatternMode();
}

//-----------------------------------------------------------------------------

void SZeroMQListener::runReceiver()
{
    ::fwGui::dialog::MessageDialog msgDialog;
    ::fwData::Object::sptr obj;

    try
    {
        m_socket->start (m_hostStr);
    }
    catch (std::exception &err)
    {
        msgDialog.showMessageDialog ("Error", "Cannot start the listener : " + std::string(err.what()));
        return;
    }
    m_socket->setReceiveTimeout(1000);
    obj = this->getObject();
    fwServicesNotifyMacro(this->getLightID(), m_sigClientConnected, ());
    while (m_socket->isStarted())
    {
        try
        {
            if (m_socket->receiveObject(obj))
            {
                this->notifyObjectUpdated();
            }
        }
        catch (::fwCore::Exception &ex)
        {
            SLM_WARN("Error while receiving object from network : " + std::string(ex.what()));
        }
        catch (std::exception &ex)
        {
            SLM_ERROR("Fatal error : " + std::string(ex.what()));
            break;
        }
    }
    m_socket->stop();
    fwServicesNotifyMacro(this->getLightID(), m_sigClientDisconnected, ());
}

//-----------------------------------------------------------------------------

void SZeroMQListener::updateConfiguration(::zmqNetwork::Socket::PatternMode const pattern,
                                          ::zmqNetwork::Socket::SocketMode const sockMode,
                                          std::string const &host)
{
    m_sockMode    = sockMode;
    m_patternMode = pattern;
    m_hostStr     = host;
}

//-----------------------------------------------------------------------------

void SZeroMQListener::starting() throw (::fwTools::Failed)
{
    ::boost::function<void() > task = ::boost::bind (&SZeroMQListener::runReceiver, this);

    m_socket        = ::zmqNetwork::Socket::sptr(new ::zmqNetwork::Socket(m_sockMode, m_patternMode));
    m_receiveWorker = ::fwThread::Worker::New();
    m_receiveWorker->post(task);
}

//-----------------------------------------------------------------------------

void SZeroMQListener::stopping() throw (::fwTools::Failed)
{
    m_socket->terminate();
    m_receiveWorker->stop();
}

//-----------------------------------------------------------------------------

} // namespace ioZMQ


