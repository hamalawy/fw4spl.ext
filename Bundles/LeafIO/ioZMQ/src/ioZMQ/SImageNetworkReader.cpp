/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "ioZMQ/SImageNetworkReader.hpp"

#include <fwData/Image.hpp>
#include <fwData/String.hpp>
#include <fwServices/Base.hpp>
#include <fwComEd/ImageMsg.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/InputDialog.hpp>

fwServicesRegisterMacro (::io::IReader, ::ioZMQ::SImageNetworkReader, ::fwData::Image);


namespace ioZMQ
{

//-----------------------------------------------------------------------------

SImageNetworkReader::SImageNetworkReader()
{
}

//-----------------------------------------------------------------------------

SImageNetworkReader::~SImageNetworkReader() throw()
{
}

//-----------------------------------------------------------------------------

void SImageNetworkReader::configuring() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SImageNetworkReader::configureWithIHM()
{
    ::fwGui::dialog::InputDialog inputDialog;

    inputDialog.setTitle("Enter hostname");
    m_host = inputDialog.getInput();
}

//-----------------------------------------------------------------------------

void SImageNetworkReader::starting() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SImageNetworkReader::stopping() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SImageNetworkReader::updating() throw (::fwTools::Failed)
{
    ::fwData::String::sptr request;
    ::fwGui::dialog::MessageDialog msgDialog;
    ::fwData::Object::ObjectModifiedSignalType::sptr sig;
    ::fwComEd::ImageMsg::sptr msg;
    ::fwData::Object::sptr obj = this->getObject();

    try
    {
        request = ::fwData::String::New();
        request->setValue("GET IMAGE");
        m_socket = ::zmqNetwork::Socket::sptr(new ::zmqNetwork::Socket(::zmqNetwork::Socket::Client,
                                                                       ::zmqNetwork::Socket::Request));
        m_socket->start(m_host);
        msg = ::fwComEd::ImageMsg::New();
        m_socket->sendObject(request);
        m_socket->receiveObject(obj);
        m_socket->sendObject(request);
        m_socket->stop();
        ::ioNetwork::ObjectUpdateNotifier::notifyUpdate(this->getSptr());
    }
    catch (std::exception &err)
    {
        if (m_socket->isStarted())
        {
            m_socket->sendObject(request);
        }
        m_socket->stop();
        msgDialog.showMessageDialog("Error", err.what());
    }
}

//-----------------------------------------------------------------------------

::io::IOPathType SImageNetworkReader::getIOPathType() const
{
    return ::io::TYPE_NOT_DEFINED;
}

//-----------------------------------------------------------------------------

void SImageNetworkReader::swapping() throw (::fwTools::Failed)
{
    // Classic default approach to update service when oject change
    this->stopping();
    this->starting();
}

//-----------------------------------------------------------------------------

} // namespace ioZMQ


