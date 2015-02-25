/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IGTLNETWORK_INETWORK_HPP__
#define __IGTLNETWORK_INETWORK_HPP__

#include "igtlNetwork/config.hpp"

#include <fwCore/Exception.hpp>
#include <fwData/Object.hpp>
#include <fwTools/macros.hpp>

#include <igtlProtocol/DataConverter.hpp>

#include <boost/type.hpp>
#include <igtl/igtlSocket.h>
#include <igtl/igtlMessageHeader.h>
#include <string>

namespace igtlNetwork
{

/**
 * @class INetwork
 *
 * @brief a interface for client and server classes
 */
class IGTLNETWORK_CLASS_API INetwork
{
public:
    /**
     * @brief default constructor
     */
    IGTLNETWORK_API INetwork();


    /**
     * @brief Destructor if a connection is opened the destructor close it
     */
    IGTLNETWORK_API virtual ~INetwork();

    /**
     * @brief generic method to receive object the type of the object is determined by response header
     * this method call the correct receiver for the type found in response header
     *
     * @return a smart pointer of fwData object
     */
    IGTLNETWORK_API bool receiveObject(::fwData::Object::sptr dest) throw (::fwCore::Exception);

    /**
     * @brief generic method to send a object the type of object is determined by classname
     *        this method call the correct sender method. If the client is not connected you receive
     *        a sigpipe signal
     * @param[in] dest object to send
     */
    IGTLNETWORK_API bool sendObject (::fwData::Object::sptr dest) throw (::fwCore::Exception);

protected:

    /**
     * @brief receive header
     * @return header
     */

    ::igtl::MessageHeader::Pointer receiveHeader();

    /** @brief receive body pack
     *
     *  @param[in] header msg header
     *
     *  @return Message
     */
    ::igtl::MessageBase::Pointer receiveBody (::igtl::MessageHeader::Pointer header) throw (::fwCore::Exception);

    /// client socket
    ::igtl::Socket::Pointer m_socket;

    /// DataConverter instance to convert fwData::Object
    /// to igtl::MessageBase and igtl::MessageBase to fwData::Object
    ::igtlProtocol::DataConverter::sptr m_dataConverter;
};

} // namespace igtlNetwork

#endif // __IGTLNETWORK_INETWORK_HPP__

