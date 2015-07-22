/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOZMQ_SIMAGENETWORKWRITER_HPP__
#define __IOZMQ_SIMAGENETWORKWRITER_HPP__

#include "ioZMQ/config.hpp"
#include <io/IWriter.hpp>
#include <zmqNetwork/Socket.hpp>

namespace ioZMQ
{
/**
 * @class SImageNetworkWriter
 *
 * @brief writer of image using ZMQNetwork
 */
class IOZMQ_CLASS_API SImageNetworkWriter : public ::io::IWriter
{

public:

    fwCoreServiceClassDefinitionsMacro ( (SImageNetworkWriter) (::io::IWriter));

    /// Constructor
    IOZMQ_API SImageNetworkWriter();

    /// Destructor
    IOZMQ_API virtual ~SImageNetworkWriter() throw();

    /// Overrides
    IOZMQ_API ::io::IOPathType getIOPathType() const;

    /**
     * @brief configure the host(in zeromq format) to send image to a client
     */
    IOZMQ_API virtual void configureWithIHM();

protected:

    /// Overrides
    IOZMQ_API virtual void configuring() throw (::fwTools::Failed);

    /// Overrides
    IOZMQ_API virtual void starting() throw (::fwTools::Failed);

    /// Overrides
    IOZMQ_API virtual void stopping() throw (::fwTools::Failed);

    /// Overrides
    IOZMQ_API virtual void updating() throw (::fwTools::Failed);

    /// Overrides
    IOZMQ_API virtual void receiving (::fwServices::ObjectMsg::csptr _msg) throw (::fwTools::Failed);

    /// Overrides
    IOZMQ_API virtual void swapping() throw (::fwTools::Failed);


private:

    /// Socket instance
    ::zmqNetwork::Socket::sptr m_socket;

    /// Host in zmq format
    std::string m_host;
};



} // namespace ioZMQ

#endif /*__IOZMQ_SIMAGENETWORKWRITER_HPP__*/

