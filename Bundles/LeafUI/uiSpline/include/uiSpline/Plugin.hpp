/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

 
#ifndef __UISPLINE_PLUGIN_HPP__
#define __UISPLINE_PLUGIN_HPP__

#include <fwRuntime/Plugin.hpp>

#include "uiSpline/config.hpp"

namespace uiSpline
{

/**
 * @brief   This class is started when the bundles is loaded.
 * @class   Plugin
 */
struct UISPLINE_CLASS_API Plugin : public ::fwRuntime::Plugin
{

    /// Destructor
    UISPLINE_API ~Plugin() throw();

    /// Overrides
    UISPLINE_API void start() throw(::fwRuntime::RuntimeException);

    /// Overrides
    UISPLINE_API void stop() throw();

};


} // namespace uiSpline

#endif // __UISPLINE_PLUGIN_HPP__

