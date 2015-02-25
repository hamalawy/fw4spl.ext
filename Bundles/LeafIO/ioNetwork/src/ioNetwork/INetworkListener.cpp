/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include "ioNetwork/INetworkListener.hpp"

#include <fwServices/Base.hpp>

#include <boost/lexical_cast.hpp>

namespace ioNetwork
{
const ::fwCom::Signals::SignalKeyType INetworkListener::s_CLIENT_CONNECTED_SIGNAL    = "clientConnected";
const ::fwCom::Signals::SignalKeyType INetworkListener::s_CLIENT_DISCONNECTED_SIGNAL = "clientDisconnected";
const ::fwCom::Signals::SignalKeyType INetworkListener::s_RECEIVED_OBJECT_SIGNAL     = "receivedObj";

//-----------------------------------------------------------------------------

INetworkListener::INetworkListener()
{
    m_sigClientConnected    = ClientConnectedSignalType::New();
    m_sigReceivedObject     = ReceivedObjectSignalType::New();
    m_sigClientDisconnected = ClientDisconnectSignalType::New();

    ::fwCom::HasSignals::m_signals (s_CLIENT_CONNECTED_SIGNAL, m_sigClientConnected);
    ::fwCom::HasSignals::m_signals (s_CLIENT_DISCONNECTED_SIGNAL, m_sigClientDisconnected);
    ::fwCom::HasSignals::m_signals (s_RECEIVED_OBJECT_SIGNAL, m_sigReceivedObject);
}

//-----------------------------------------------------------------------------

INetworkListener::~INetworkListener()
{
}

//-----------------------------------------------------------------------------

void INetworkListener::configuring() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void INetworkListener::starting() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void INetworkListener::stopping() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void INetworkListener::updating() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void INetworkListener::notifyObjectUpdated()
{
    ::ioNetwork::ObjectUpdateNotifier::notifyUpdate(this->getSptr());
}

//-----------------------------------------------------------------------------

void INetworkListener::swapping() throw (::fwTools::Failed)
{
    // Classic default approach to update service when oject change
    this->stopping();
    this->starting();
}

//-----------------------------------------------------------------------------

} // namespace ioIGTL


