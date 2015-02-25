/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOZMQ__SZEROMQSENDER_HPP__
#define __IOZMQ__SZEROMQSENDER_HPP__

#include "ioZMQ/config.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <ioNetwork/INetworkSender.hpp>
#include <fwTools/Failed.hpp>
#include <zmqNetwork/Socket.hpp>

namespace ioZMQ
{

/**
 * @class SZeroMQSender
 *
 * @brief network sender using ZMQNetwork library
 */
class IOZMQ_CLASS_API SZeroMQSender : public ::ioNetwork::INetworkSender
{
public:

    fwCoreServiceClassDefinitionsMacro ( (SZeroMQSender) (::ioNetwork::INetworkSender));

    /**
     * @brief Slot to update configuration
     */
    IOZMQ_API static const ::fwCom::Slots::SlotKeyType s_UPDATE_CONFIGURATION_SLOT;
    typedef ::fwCom::Slot< void (::zmqNetwork::Socket::PatternMode const,
                                 ::zmqNetwork::Socket::SocketMode const,
                                 std::string const &host) > UpdateConfigurationSlotType;


    /// Constructor
    IOZMQ_API SZeroMQSender();

    /// Destructor
    IOZMQ_API virtual ~SZeroMQSender();

protected:
    /**
     * @brief configure the listener and parse configuration with ZeroMQConfigurationParser
     *
     * @see ZeroMQConfigurationParser for the format of the configuration
     */
    IOZMQ_API virtual void configuring() throw (::fwTools::Failed);

    /**
     * @brief start the socket in a thread
     */
    IOZMQ_API virtual void starting() throw (::fwTools::Failed);

    /**
     * @brief stop the socket
     */
    IOZMQ_API virtual void stopping() throw (::fwTools::Failed);


    /**
     * @brief change the port of the service work only in tcp mode
     */
    IOZMQ_API void setPort(::boost::uint16_t const port) throw (::fwTools::Failed);

    /**
     * @brief slot method to update configuration
     *
     * @param[in] patternMode pattern mode (push, pull,...)
     * @param[in] sockMode socket mode (client or server)
     * @param[in] host new host
     */
    IOZMQ_API void updateConfiguration(::zmqNetwork::Socket::PatternMode const,
                                       ::zmqNetwork::Socket::SocketMode const,
                                       std::string const &host);

private:

    /**
     * @brief method to send data object
     *
     * @see INetworkSender
     */
    void sendObject (const ::fwData::Object::sptr &obj);

    /// Socket instance
    ::zmqNetwork::Socket::sptr m_socket;

    /// PatternMode to use (Push, Pull, Publish,...)
    ::zmqNetwork::Socket::PatternMode m_patternMode;

    /// Socket mode to use (Client, Server)
    ::zmqNetwork::Socket::SocketMode m_sockMode;

    /// update configuration slot
    UpdateConfigurationSlotType::sptr m_updateConfigurationSlot;

    /// host to connect or interface to bind
    std::string m_hostStr;
};



} // namespace ioZMQ

#endif /*__IOZMQ__SZEROMQSENDER_HPP__*/

