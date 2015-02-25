/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UINETWORK__SOBJECTCONNECTORTREE_HPP__
#define __UINETWORK__SOBJECTCONNECTORTREE_HPP__

#include "uiNetwork/config.hpp"
#include "uiNetwork/ConnectorModel.hpp"
#include "uiNetwork/ObjectConnectorWidget.hpp"
#include "uiNetwork/AObjectConnectorWidget.hpp"

#include <gui/editor/IEditor.hpp>
#include <fwData/Composite.hpp>
#include <fwData/Vector.hpp>

#include <QObject>
#include <QString>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLineEdit>

#include <boost/regex.hpp>

namespace uiNetwork
{


/**
 * @brief This editors allows to create listener/sender services on the object.
 *
 * If the current object is a Composite or a Vector, this editors shows the lists of its sub-objects and allows to add
 * sub-objects. On each sub-objects, we could create listener/sender services.
 */
class UINETWORK_CLASS_API SObjectConnectorTree :
    public QObject,
    public ::gui::editor::IEditor
{
Q_OBJECT

public:



    fwCoreServiceClassDefinitionsMacro ( (SObjectConnectorTree) (::gui::editor::IEditor));

    /**
     * @brief configure the service and initialize the ConnectorModel
     *
     * @verbatim
        <service uid="${GENERIC_UID}_object_connector_view" type="::gui::editor::IEditor" impl="::uiNetwork::SObjectConnectorTree" autoConnect="yes">
            <filter>T.*</filter>
            <config impl="::ioZMQ::SZeroMQSender">
                <!-- service configuration -->
            </config>
            <config impl="::ioZMQ::SZeroMQListener">
               <!-- service configuration -->
            </config>
        </service>
       @endverbatim
     *
     * - \b filter (optional): regex to filter objects
     * - \b config (optional): specific configuration of the listener/sender service to create
     *   - \b impl: implementation of service to configure
     */
    UINETWORK_API void configuring() throw (::fwTools::Failed);

    /**
     * @brief construct the IHM and the Data object tree
     */
    UINETWORK_API void starting() throw (::fwTools::Failed);

    /**
     * @brief destruct the IHM
     */
    UINETWORK_API void stopping() throw (::fwTools::Failed);

    /// Overrides
    UINETWORK_API void updating() throw (::fwTools::Failed);

    /// Overrides
    UINETWORK_API void receiving (::fwServices::ObjectMsg::csptr _msg) throw (::fwTools::Failed);

    /// Overrides
    UINETWORK_API void swapping() throw (::fwTools::Failed);

    /// Constructor
    UINETWORK_API SObjectConnectorTree();

    /// Destructor
    UINETWORK_API ~SObjectConnectorTree() throw();

private:

    void initRootConnector();

private Q_SLOTS:

    /**
     * @brief slot called when user change the filter in the m_filterEditor
     */
    void onFilterChange(QString const &str);

    /**
     * @brief refresh the tree widgets
     */
    void refreshTree();


private:
    /// main layout
    QVBoxLayout *m_layout;

    /// filter editor contain
    QLineEdit *m_filterEditor;

    /// Tree widget contain all object connector widgets
    QTreeWidget *m_objTree;

    /// root object connector
    AObjectConnectorWidget *m_root;

    /// regex to filter object can be connected
    ::boost::regex m_filter;

    /// Model contain all information needed by ANetworkServiceWidget instances
    ConnectorModel m_model;

};



} // namespace uiNetwork

#endif /*__UINETWORK__SOBJECTCONNECTORTREE_HPP__*/

