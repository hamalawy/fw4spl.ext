/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "zmqNetwork/Socket.hpp"

#include "igtlProtocol/MessageFactory.hpp"

#include <igtl/igtlMessageBase.h>
#include <cstring>


namespace zmqNetwork
{

//------------------------------------------------------------------------------

Socket::Socket(SocketMode socketMode, PatternMode patternMode) :
    m_isStarted(false),
    m_sockMode(socketMode),
    m_patternMode(patternMode),
    m_timeout(1000)
{
    m_dataConverter = ::igtlProtocol::DataConverter::getInstance();
}

//------------------------------------------------------------------------------

Socket::~Socket()
{

}

//------------------------------------------------------------------------------

void Socket::deleteMessage(void *data, void *hint)
{
    igtl::MessageBase::Pointer *ptr = reinterpret_cast<igtl::MessageBase::Pointer*>(hint);

    delete ptr;
}

//------------------------------------------------------------------------------

void Socket::sendObject(::fwData::Object::sptr data) throw (::fwCore::Exception)
{
    ::igtl::MessageBase::Pointer    *igtlMsg;

    igtlMsg = new ::igtl::MessageBase::Pointer(m_dataConverter->fromFwObject(data));
    (*igtlMsg)->SetDeviceName ("Tracker");
    (*igtlMsg)->Pack();

    ::zmq::message_t zmqMsg((*igtlMsg)->GetPackPointer(), (*igtlMsg)->GetPackSize(),
                            Socket::deleteMessage, igtlMsg);

    m_socket->send(zmqMsg);
}

//------------------------------------------------------------------------------

bool Socket::receiveObject(::fwData::Object::sptr obj) throw(::fwCore::Exception)
{
    ::igtl::MessageBase::Pointer msg;
    ::zmq::message_t response;
    ::igtl::MessageHeader::Pointer headerMsg;

    headerMsg = ::igtl::MessageHeader::New();
    headerMsg->InitPack();
    m_socket->recv(&response);
    if (response.size() > Socket::s_HEADER_SIZE)
    {
        std::memcpy(headerMsg->GetPackPointer(), response.data(), headerMsg->GetPackSize());
        if (headerMsg->Unpack() & ::igtl::MessageHeader::UNPACK_HEADER)
        {
            msg = ::igtlProtocol::MessageFactory::create(headerMsg->GetDeviceType());
            msg->SetMessageHeader(headerMsg);
            msg->AllocatePack();
            std::memcpy(msg->GetPackBodyPointer(),
                        (reinterpret_cast<char*>(response.data())) + Socket::s_HEADER_SIZE, msg->GetPackBodySize());
            if (!(msg->Unpack(1) & ::igtl::MessageHeader::UNPACK_BODY))
            {
                throw Exception("Cannot unpack body");
            }
            m_dataConverter->fromIgtlMessage(msg, obj);
            return true;
        }
        else
        {
            throw Exception("Cannot unpack header");
        }
    }
    return false;
}

//------------------------------------------------------------------------------

bool Socket::isStarted() const
{
    return m_isStarted;
}

//------------------------------------------------------------------------------

Socket::SocketMode Socket::getSocketMode() const
{
    return m_sockMode;
}

//------------------------------------------------------------------------------

Socket::PatternMode Socket::getPatternMode() const
{
    return m_patternMode;
}

//------------------------------------------------------------------------------

void Socket::initialize()
{
    m_context = ContextSPtr(new ::zmq::context_t(1));

    m_socket = SocketSPtr(new ::zmq::socket_t(*m_context, static_cast<int>(m_patternMode)));
    if (m_patternMode == Subscribe)
    {
        m_socket->setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    }
}

//------------------------------------------------------------------------------

void Socket::start(std::string const &str)
{
    if (m_context == NULL)
    {
        this->initialize();
    }
    if (m_sockMode == Server)
    {
        m_socket->bind(str.c_str());
    }
    else
    {
        m_socket->connect(str.c_str());
    }
    m_isStarted = true;
}

//------------------------------------------------------------------------------

void Socket::terminate()
{
    m_isStarted = false;
}

//------------------------------------------------------------------------------

void Socket::setReceiveTimeout(int msec)
{
    m_timeout = msec;
    m_socket->setsockopt(ZMQ_RCVTIMEO, &m_timeout, sizeof(m_timeout));
}

//------------------------------------------------------------------------------

void Socket::stop()
{
    m_socket->close();
    m_context->close();
    m_isStarted = false;
}

//------------------------------------------------------------------------------

}//namespace zmqNetwork
