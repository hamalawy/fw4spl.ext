/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/make_shared.hpp>

#include <fwTools/Object.hpp>
#include <fwRuntime/profile/Profile.hpp>
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwCore/spyLog.hpp>

#include <fwServices/registry/ActiveWorkers.hpp>
#include <fwServices/registry/ServiceFactory.hpp>
#include <fwServices/registry/ObjectService.hpp>


#include "PoC07TimeLine/Plugin.hpp"
#include "PoC07TimeLine/SConsumer.hpp"

namespace PoC07TimeLine
{

//-----------------------------------------------------------------------------

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::PoC07TimeLine::Plugin");

//-----------------------------------------------------------------------------

Plugin::~Plugin() throw()
{
}

//-----------------------------------------------------------------------------

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    SLM_TRACE_FUNC();

    ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();
    ::fwServices::registry::ActiveWorkers::getDefault()
    ->addWorker(::fwServices::registry::ActiveWorkers::s_DEFAULT_WORKER, worker);
}

//-----------------------------------------------------------------------------

void Plugin::stop() throw()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

} // namespace PoC07TimeLine
