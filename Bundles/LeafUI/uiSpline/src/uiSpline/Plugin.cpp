/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include "uiSpline/Plugin.hpp"

namespace uiSpline
{

//-----------------------------------------------------------------------------

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::uiSpline::Plugin");

//-----------------------------------------------------------------------------

Plugin::~Plugin() throw()
{}

//-----------------------------------------------------------------------------

void Plugin::start() throw( ::fwRuntime::RuntimeException )
{}

//-----------------------------------------------------------------------------

void Plugin::stop() throw()
{}

//-----------------------------------------------------------------------------

} // namespace uiSpline

