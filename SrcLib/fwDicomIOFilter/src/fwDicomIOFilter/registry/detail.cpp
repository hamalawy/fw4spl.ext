/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/util/LazyInstantiator.hpp>

#include "fwDicomIOFilter/registry/detail.hpp"


namespace fwDicomIOFilter
{

namespace registry
{

struct FwDicomIOFilterRegistryInstantiatorTag {};

SPTR(Type) get()
{
    typedef ::fwCore::util::LazyInstantiator< Type, FwDicomIOFilterRegistryInstantiatorTag > InstantiatorType;
    return InstantiatorType::getInstance();
}

} // namespace registry

} // namespace fwDicomIOFilter


