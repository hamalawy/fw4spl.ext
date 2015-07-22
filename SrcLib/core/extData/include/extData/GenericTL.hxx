/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __EXTDATA_GENERICTL_HXX__
#define __EXTDATA_GENERICTL_HXX__

#include <boost/make_shared.hpp>
#include <boost/pool/pool.hpp>

#include <fwData/Exception.hpp>

namespace extData
{

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
GenericTL<BUFFER_TYPE>::GenericTL ( ::fwData::Object::Key key ) :
    BufferTL(key),
    m_maxElementNum(~0u)
{
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
GenericTL<BUFFER_TYPE>::~GenericTL ()
{
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
void GenericTL<BUFFER_TYPE>::cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType &cache)
{
    GenericTL::csptr other = GenericTL::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );

    this->fieldDeepCopy( _source );
    this->clearTimeline();
    this->initPoolSize(other->getMaxElementNum());

    ::fwCore::mt::WriteLock writeLock(m_tlMutex);
    ::fwCore::mt::WriteLock readLock(other->m_tlMutex);

    BOOST_FOREACH(TimelineType::value_type elt, other->m_timeline)
    {
        SPTR(BufferType) tlObj = this->createBuffer(elt.first);
        tlObj->deepCopy(*elt.second);
        m_timeline.insert(TimelineType::value_type(elt.first, tlObj));
    }
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
CSPTR(typename GenericTL<BUFFER_TYPE>::BufferType)
GenericTL<BUFFER_TYPE>::getClosestBuffer( ::fwCore::HiResClock::HiResClockType timestamp,
                                          DirectionType direction) const
{
    CSPTR(::extData::timeline::Object) buffer = this->getClosestObject(timestamp, direction);
    return ::boost::dynamic_pointer_cast< const BufferType >(buffer);
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
CSPTR(typename GenericTL<BUFFER_TYPE>::BufferType)
GenericTL<BUFFER_TYPE>::getBuffer(::fwCore::HiResClock::HiResClockType timestamp) const
{
    CSPTR(::extData::timeline::Object) buffer = this->getObject(timestamp);
    return ::boost::dynamic_pointer_cast< const BufferType >(buffer);
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
void GenericTL<BUFFER_TYPE>::initPoolSize(unsigned int maxElementNum)
{
    m_maxElementNum = maxElementNum;
    this->allocPoolSize(sizeof(BUFFER_TYPE) * m_maxElementNum);
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
SPTR(::extData::timeline::Object)
GenericTL<BUFFER_TYPE>::createObject(::fwCore::HiResClock::HiResClockType timestamp)
{
    return this->createBuffer(timestamp);
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
SPTR(typename GenericTL<BUFFER_TYPE>::BufferType)
GenericTL<BUFFER_TYPE>::createBuffer(::fwCore::HiResClock::HiResClockType timestamp)
{
    SPTR(BufferType) obj = ::boost::make_shared< BufferType >(
        m_maxElementNum, timestamp,
        (::extData::timeline::Buffer::BufferDataType) m_pool->malloc(),
        m_pool->get_requested_size(),
        ::boost::bind( &::boost::pool<>::free, m_pool, _1)
        );
    return obj;
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
bool GenericTL<BUFFER_TYPE>::isObjectValid(const CSPTR(::extData::timeline::Object) &obj) const
{
    CSPTR(BufferType) srcObj = ::boost::dynamic_pointer_cast< const BufferType >(obj);
    return srcObj != NULL;
}

//------------------------------------------------------------------------------

template < class BUFFER_TYPE >
unsigned int GenericTL<BUFFER_TYPE>::getMaxElementNum() const
{
    return m_maxElementNum;
}

//------------------------------------------------------------------------------

} // namespace extData


#endif // __EXTDATA_GENERICTL_HXX__
