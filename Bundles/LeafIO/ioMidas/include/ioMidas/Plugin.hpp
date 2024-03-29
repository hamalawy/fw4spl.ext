/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOMIDAS_PLUGIN_HPP__
#define __IOMIDAS_PLUGIN_HPP__

#include <fwRuntime/Plugin.hpp>

namespace ioMidas
{

/**
 * @brief   This class is started when the bundles is loaded.
 * @class   Plugin.
 */
struct Plugin : public ::fwRuntime::Plugin
{
    /**
     * @brief   destructor
     */
    ~Plugin() throw();

    // Overrides
    void start() throw(::fwRuntime::RuntimeException);

    // Overrides
    void stop() throw();

};

} // namespace ioMidas

#endif // __IOMIDAS_PLUGIN_HPP__
