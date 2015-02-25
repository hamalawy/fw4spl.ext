/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "igtlProtocol/DataConverter.hpp"
#include "igtlProtocol/converter/AtomConverter.hpp"
#include "igtlProtocol/converter/ImageConverter.hpp"
#include "igtlProtocol/converter/MatrixConverter.hpp"
#include "igtlProtocol/converter/StringConverter.hpp"
#include "igtlProtocol/converter/LineConverter.hpp"
#include "igtlProtocol/converter/PointListConverter.hpp"
#include "igtlProtocol/converter/MeshConverter.hpp"
#include "igtlProtocol/converter/ScalarConverter.hpp"

#include <fwCore/util/LazyInstantiator.hpp>
#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>

#include <boost/foreach.hpp>
#include <boost/thread/mutex.hpp>

#include <igtl/igtlStatusMessage.h>
#include <igtl/igtlCapabilityMessage.h>


namespace igtlProtocol
{

DataConverter::sptr DataConverter::getInstance()
{
    return ::fwCore::util::LazyInstantiator< DataConverter >::getInstance();
}

//-----------------------------------------------------------------------------

void DataConverter::registerConverter(converter::IConverter::sptr c)
{
    (DataConverter::getInstance())->m_converters.push_back(c);
}

//-----------------------------------------------------------------------------

DataConverter::DataConverter()
{
    m_defaultConverter = ::igtlProtocol::converter::AtomConverter::New();
}

//-----------------------------------------------------------------------------

DataConverter::~DataConverter()
{
}

//-----------------------------------------------------------------------------

::igtl::MessageBase::Pointer DataConverter::fromFwObject(::fwData::Object::sptr src) const
throw (::igtlProtocol::exception::Conversion)
{
    std::string classname = src->getClassname();

    BOOST_FOREACH(converter::IConverter::sptr converter, m_converters)
    {
        if (converter->getFwDataObjectType() == classname)
        {
            ::fwCore::mt::ReadLock lock(src->getMutex());
            return converter->fromFwDataObject(src);
        }
    }
    return m_defaultConverter->fromFwDataObject(src);
}

//-----------------------------------------------------------------------------

void DataConverter::fromIgtlMessage(::igtl::MessageBase::Pointer const &src,
                                    ::fwData::Object::sptr &dest) const throw (::igtlProtocol::exception::Conversion)
{
    std::string const deviceType = src->GetDeviceType();

    if (deviceType == "ATOMS")
    {
        m_defaultConverter->fromIgtlMessage(src, dest);
        return;
    }

    for(converter::IConverter::sptr converter : m_converters)
    {
        if (converter->getIgtlType() == deviceType)
        {
            ::fwCore::mt:: WriteLock lock(dest->getMutex());
            converter->fromIgtlMessage(src, dest);
            return;
        }
    }
    FW_RAISE_EXCEPTION(exception::Conversion("Message type not supported : " + std::string(src->GetDeviceType())));
}

//-----------------------------------------------------------------------------

::igtl::MessageBase::Pointer DataConverter::getStatusMessage(int igtlCode,
                                                             int igtlSubCode,
                                                             std::string const &errMsg) const
{
    ::igtl::StatusMessage::Pointer statusMsg;

    statusMsg = ::igtl::StatusMessage::New();
    statusMsg->SetCode(igtlCode);
    statusMsg->SetErrorName(errMsg.c_str());
    statusMsg->SetSubCode(igtlSubCode);
    return ::igtl::MessageBase::Pointer(statusMsg);
}

//-----------------------------------------------------------------------------

::igtl::MessageBase::Pointer DataConverter::getCapabilitiesMessage() const
{
    ::igtl::CapabilityMessage::Pointer msg;

    msg = ::igtl::CapabilityMessage::New();
    msg->SetNumberOfTypes(static_cast<int>(m_converters.size()));
    for (unsigned int i = 0; i < m_converters.size(); ++i)
    {
        msg->SetType(i, m_converters[i]->getIgtlType().c_str());
    }
    return ::igtl::MessageBase::Pointer(msg);
}

}//namespace igtlProtocol
