/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __MIDASIO_FACTORY_NEW_HPP__
#define __MIDASIO_FACTORY_NEW_HPP__

#include <string>

#include <boost/make_shared.hpp>

#include <fwTools/macros.hpp>

#include "midasIO/config.hpp"
#include "midasIO/registry/detail.hpp"


namespace midasIO
{

class IConfiguration;
class IResponseHandler;

namespace configurationFactory
{

template<class CLASSNAME > SPTR( CLASSNAME )  New();

/**
 * @brief Key class used to restrict access to Object construction.
 * See http://www.drdobbs.com/184402053
 */
class Key
{
    template<typename CLASSNAME>
    friend SPTR( CLASSNAME ) midasIO::configurationFactory::New();

    Key(){};
};


    template<class CLASSNAME > SPTR( CLASSNAME )  New()
    {
        SPTR(CLASSNAME) obj = ::boost::make_shared< CLASSNAME >( Key() );
        return obj;
    }

    MIDASIO_API SPTR(::midasIO::IConfiguration) New(
            const ::midasIO::registry::KeyType & classname );
}

namespace responseHandlerFactory
{

template<class CLASSNAME > SPTR( CLASSNAME )  New();

/**
 * @brief Key class used to restrict access to Object construction.
 * See http://www.drdobbs.com/184402053
 */
class Key
{
    template<typename CLASSNAME>
    friend SPTR( CLASSNAME ) midasIO::responseHandlerFactory::New();

    Key(){};
};


    template<class CLASSNAME > SPTR( CLASSNAME )  New()
    {
        SPTR(CLASSNAME) obj = ::boost::make_shared< CLASSNAME >( Key() );
        return obj;
    }

    MIDASIO_API SPTR(::midasIO::IResponseHandler) New(
            const ::midasIO::registry::KeyType & classname );
} // responseHandlerFactory

} // namespace midasIO

#endif /* __MIDASIO_FACTORY_NEW_HPP__ */

