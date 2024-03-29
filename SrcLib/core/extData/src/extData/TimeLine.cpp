/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/base.hpp>

#include "extData/TimeLine.hpp"

namespace extData
{

const ::fwCom::Signals::SignalKeyType TimeLine::s_OBJECT_PUSHED_SIG = "objectPushed";

//------------------------------------------------------------------------------

TimeLine::TimeLine ( ::fwData::Object::Key key )
{
    m_sigObjectPushed = ObjectPushedSignalType::New();
    m_signals( s_OBJECT_PUSHED_SIG,  m_sigObjectPushed);

#ifdef COM_LOG
    ::fwCom::HasSignals::m_signals.setID();
#endif

}

//------------------------------------------------------------------------------

TimeLine::~TimeLine ()
{
}

//------------------------------------------------------------------------------

} // namespace extData


