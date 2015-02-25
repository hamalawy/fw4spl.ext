/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOZMQ__SATOMNETWORKWRITER_HPP__
#define __IOZMQ__SATOMNETWORKWRITER_HPP__

#include "ioZMQ/config.hpp"

#include <io/IWriter.hpp>
#include <zmqNetwork/Socket.hpp>

namespace ioZMQ
{

/**
 * @class SAtomNetworkWriter
 *
 * @brief writer of atom object using ZMQNetwork library
 */
class IOZMQ_CLASS_API SAtomNetworkWriter : public ::io::IWriter
{

public:

    fwCoreServiceClassDefinitionsMacro ( (SAtomNetworkWriter) (::io::IWriter));


    /// Constructor
    IOZMQ_API SAtomNetworkWriter();

    /// Destructor
    IOZMQ_API virtual ~SAtomNetworkWriter() throw();

    /// Overrides
    IOZMQ_API ::io::IOPathType getIOPathType() const;

    /**
     * @brief configure the host(in zeromq format) to send atom to a client
     *
     * @see http://zeromq.org/intro:read-the-manual
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
    IOZMQ_API virtual void swapping() throw (::fwTools::Failed);

private:

    /// Socket instance
    ::zmqNetwork::Socket::sptr m_socket;

    /// Host in zmq format
    std::string m_host;
};



} // namespace ioZMQ

#endif /*__IOZMQ__SATOMNETWORKWRITER_HPP__*/

