/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IONETWORK_INETWORKSENDER_HPP__
#define __IONETWORK_INETWORKSENDER_HPP__

#include "ioNetwork/config.hpp"

#include <fwData/Object.hpp>
#include <fwServices/helper/SigSlotConnection.hpp>
#include <fwServices/IController.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>
#include <fwCom/Signal.hpp>

#include <map>
#include <boost/type.hpp>

namespace ioNetwork
{

/**
 * @class INetworkSender
 *
 * @brief Abstract class for network senders(you need to inherit from this class if you want implement a new network
 * sender).
 *
 * @note The inherited service will automatically be connected to its object. This bypass the "autoConnect" because of
 * the OSLM_FATAL_IF when service receives a message.
 */
class IONETWORK_CLASS_API INetworkSender : public ::fwServices::IController
{

public:

    fwCoreServiceClassDefinitionsMacro ( (INetworkSender) (::fwServices::IController));

    /**
     * @brief Send data slot can be used if you want to connect a custom signal to this slot
     */
    IONETWORK_API static const ::fwCom::Slots::SlotKeyType s_SEND_DATA_SLOT;

    /**
     * @typedef SendDataSlotType
     *
     * @brief SendDataSlotType slot type stored that you can connect in your plugin.xml
     */
    typedef ::fwCom::Slot<void (::fwData::Object::sptr) > SendDataSlotType;

    /**
     * @brief Server started signal is emitted when the server is started
     */
    IONETWORK_API static const ::fwCom::Signals::SignalKeyType s_SERVER_STARTED_SIGNAL;

    /**
     * @typedef ServerStartedSignalType
     *
     * @brief ServerStartedSignalType is stored and emitted when the sender is started
     */
    typedef ::fwCom::Signal< void () > ServerStartedSignalType;

    /**
     * @brief Server stopped signal is emitted when the sender is stopped
     */
    IONETWORK_API static const ::fwCom::Signals::SignalKeyType s_SERVER_STOPPED_SIGNAL;

    /**
     * @typedef ServerStoppedSignalType
     *
     * @brief ServerStoppedSignalType is stored and emitted when the sender is stopped
     */
    typedef ::fwCom::Signal< void () > ServerStoppedSignalType;


    /// Constructor
    IONETWORK_API INetworkSender();

    /// Destructor
    IONETWORK_API virtual ~INetworkSender();

    /**
     * @brief method to set a port to listen
     *
     * @param[in] port new port to listen
     */
    IONETWORK_API virtual void setPort(::boost::uint16_t const port) throw (::fwTools::Failed) = 0;

protected:

    /// Overrides
    IONETWORK_API virtual void configuring() throw (::fwTools::Failed);

    /// Make connection like autoConnect enable message reception
    IONETWORK_API virtual void starting() throw (::fwTools::Failed);

    /// Disconnect connection to bypass the OSLM_FATAL_IF when service receives a message
    IONETWORK_API virtual void stopping() throw (::fwTools::Failed);

    /// Sends the object
    IONETWORK_API virtual void updating() throw (::fwTools::Failed);

    /// called when object is updated and call sendData method
    IONETWORK_API virtual void receiving (::fwServices::ObjectMsg::csptr _msg) throw (::fwTools::Failed);

    /// Calls stopping and starting
    IONETWORK_API virtual void swapping() throw (::fwTools::Failed);

    /**
     * @brief SLOT: called to send obj
     *
     * @param[in] obj the obj to send
     */
    IONETWORK_API void sendData(::fwData::Object::sptr obj);

    /**
     * @brief Sends the obj
     */
    virtual void sendObject(const ::fwData::Object::sptr &obj) = 0;

    /// Signal when server started
    ServerStartedSignalType::sptr m_sigServerStarted;

    /// Signal when server stopped
    ServerStoppedSignalType::sptr m_sigServerStopped;

private:

    /// Slot used to send data
    SendDataSlotType::sptr m_sendDataSlot;

    /// connection between obj signal and service slots
    ::fwServices::helper::SigSlotConnection::sptr m_connections;
};

} // namespace ioNetwork

#endif /*__IONETWORK_INETWORKSENDER_HPP__*/

