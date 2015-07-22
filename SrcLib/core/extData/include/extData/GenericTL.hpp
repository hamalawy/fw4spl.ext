/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __EXTDATA_GENERICTL_HPP__
#define __EXTDATA_GENERICTL_HPP__

#include "extData/timeline/GenericObject.hpp"
#include "extData/BufferTL.hpp"
#include "extData/config.hpp"

namespace extData
{
/**
 * @class   GenericTL
 * @brief   This class defines a timeline of template objects. All objects have the same maximum number of elements,
 *          nevertheless it is not mandatory to provide all elements.
 */
template < class BUFFER_TYPE >
class GenericTL : public BufferTL
{

public:
    fwCoreNonInstanciableClassDefinitionsMacro( (GenericTL<BUFFER_TYPE>)(::fwData::Object) );

    typedef ::extData::timeline::GenericObject< BUFFER_TYPE > BufferType;
    /**
     * @brief Constructor
     * @param key Private construction key
     */
    GenericTL( ::fwData::Object::Key key );

    /// Destructor
    virtual ~GenericTL();

    /// Defines deep copy
    virtual void cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType &cache);

    /**
     * @brief Returns the closest buffer to the given timestamp
     * @param timestamp timestamp used to find the closest buffer
     * @param direction direction to find the closest buffer (PAST, FUTURE, BOTH)
     */
    CSPTR(BufferType) getClosestBuffer( ::fwCore::HiResClock::HiResClockType timestamp,
                                        DirectionType direction = BOTH) const;

    /// Returns the buffer matching the specified timestamp, returns NULL if object is not found
    CSPTR(BufferType) getBuffer(::fwCore::HiResClock::HiResClockType timestamp) const;

    /// Initializes the size of the pool buffer.
    virtual void initPoolSize(unsigned int maxElementNum);

    /**
     * @brief Returns a new ::extData::timeline::Object with the given timestamp.
     * @note This buffer memory is managed by the pool.
     * @warning This buffer is not registered in the timeline. You must call pushObject() to register it.
     */
    SPTR(::extData::timeline::Object) createObject(::fwCore::HiResClock::HiResClockType timestamp);

    /**
     * @brief Returns a new BufferType with the given timestamp.
     * @note This buffer memory is managed by the pool.
     * @warning This buffer is not registered in the timeline. You must call pushObject() to register it.
     */
    SPTR(BufferType) createBuffer(::fwCore::HiResClock::HiResClockType timestamp);

    /// Check if the type of an object is compatible with this timeline
    virtual bool isObjectValid(const CSPTR(::extData::timeline::Object) &obj) const;

    /// Get/set the maximum number of objects inside a single buffer
    unsigned int getMaxElementNum() const;

protected:
    /// maximum number of elements inside a single buffer
    unsigned int m_maxElementNum;

}; // class GenericTL

} // namespace extData

#endif // __EXTDATA_GENERICTL_HPP__
