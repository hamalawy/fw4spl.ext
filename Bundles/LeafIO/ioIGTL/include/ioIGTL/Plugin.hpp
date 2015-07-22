/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOIGTL_PLUGIN_HPP__
#define __IOIGTL_PLUGIN_HPP__

#include <fwRuntime/Plugin.hpp>

#include "ioIGTL/config.hpp"

namespace ioIGTL
{

/**
 * @brief   This class is started when the bundles is loaded
 * @class   Plugin
 */
struct IOIGTL_CLASS_API Plugin : public ::fwRuntime::Plugin
{

    /// Destructor
    IOIGTL_API ~Plugin() throw();

    /// Overrides
    IOIGTL_API void start() throw (::fwRuntime::RuntimeException);

    /// Overrides
    IOIGTL_API void stop() throw();

};

} // namespace ioIGTL

#endif // __IOIGTL_PLUGIN_HPP__
