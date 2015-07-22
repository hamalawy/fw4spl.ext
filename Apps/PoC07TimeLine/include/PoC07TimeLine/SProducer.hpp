/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __POC07TIMELINE_SPRODUCER_HPP__
#define __POC07TIMELINE_SPRODUCER_HPP__

#include <fwServices/IService.hpp>

#include "PoC07TimeLine/config.hpp"

namespace PoC07TimeLine
{

/**
 * @class   SProducer
 * @brief   Service that produce messages and store them into a timeline.
 */
class POC07TIMELINE_CLASS_API SProducer : public ::fwServices::IService
{
public:

    fwCoreServiceClassDefinitionsMacro ( (SProducer)(::fwServices::IService) );

    POC07TIMELINE_API SProducer() throw();
    POC07TIMELINE_API virtual ~SProducer() throw();

protected:

    /// Starts the timer.
    virtual void starting() throw(::fwTools::Failed);

    /// Stops the timer.
    virtual void stopping() throw(::fwTools::Failed);

    /// Does nothing.
    virtual void swapping() throw ( ::fwTools::Failed );

    /// Does nothing.
    virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );

    /// Called by the timer to send messages periodically
    virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief Configures the service.
     *
     * @verbatim
       <service type="::fwServices::IService" impl="::PoC07TimeLine::SProducer" autoConnect="yes">
           <message>Hello</message>
           <id>0</id>
           <period>1000</period>
       </service>
       @endverbatim
     * - \b message : message to be sent.
     * - \b id : id of the sender.
     * - \b period : time between two messages, in milliseconds.
     * - \b timelineSize (optional): maximum number of messages inside the timeline.
     */
    virtual void configuring() throw(fwTools::Failed);

private:

    /// Timer used to send messages periodically
    SPTR( fwThread::Timer ) m_timer;

    /// Message to be sent
    std::string m_message;

    /// Id of the sender
    unsigned int m_senderId;

    /// A message will be sent every m_uiPeriod milliseconds.
    unsigned int m_period;

    /// Current message count
    unsigned int m_msgCount;

    /// Timeline size
    unsigned int m_timelineSize;
};

}  // namespace PoC07TimeLine

#endif  // __POC07TIMELINE_SPRODUCER_HPP__

