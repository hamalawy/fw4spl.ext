/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIVIEWPOINTS_SVIEWPOINTSEDITOR_HPP__
#define __UIVIEWPOINTS_SVIEWPOINTSEDITOR_HPP__

#include <QObject>
#include <QPointer>
#include <string>

#include <fwTools/Failed.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiViewPoints/config.hpp"

class QPushButton;
class QCheckBox;
class QListWidget;
class QListWidgetItem;
class QWidget;
class QGridLayout;
class QObject;

namespace fwData
{
class TransformationMatrix3D;
}

namespace uiViewPoints
{

/**
 * @brief SViewPointsEditor is the editor of view points activity.
 * @class SViewPointsEditor
 */
class UIVIEWPOINTS_CLASS_API SViewPointsEditor : public QObject,
                                                 public ::gui::editor::IEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (SViewPointsEditor)(::gui::editor::IEditor) );

    /// Constructor.
    UIVIEWPOINTS_API SViewPointsEditor() throw();

    /// Destructor.
    UIVIEWPOINTS_API virtual ~SViewPointsEditor() throw();

    /**
     * @name Signal type and key.
     * @{ */
    UIVIEWPOINTS_API static const ::fwCom::Signals::SignalKeyType s_DIRECT_TARGET_CHANGED_SIG;
    typedef ::fwCom::Signal< void (SPTR(::fwData::TransformationMatrix3D)) > DirectTargetChangedSignalType;
    /**  @} */

    UIVIEWPOINTS_API static const std::string s_FIELD_NAME;

protected:

    /**
     * @name Overrides.
     * @{ */
    virtual void starting() throw(::fwTools::Failed);
    virtual void stopping() throw(::fwTools::Failed);
    virtual void receiving(CSPTR(::fwServices::ObjectMsg) msg) throw(::fwTools::Failed);
    virtual void updating() throw(::fwTools::Failed);
    /**  @} */

    /**
     * @brief Overrides IService::configuring().
     * Configuration example :
       @verbatim
       <config>
        <transform uid="..." /> <!-- ::fwData::TransformationMatrix3D to update with selected ::fwData::Point -->
       </config>
       @endverbatim
     *
     * @throw fwTools::Failed
     */
    virtual void configuring() throw(fwTools::Failed);

    /// Signal triggered when a view point is double clicked.
    DirectTargetChangedSignalType::sptr m_sigDirectTargetChanged;

protected Q_SLOTS:

    /// This method is called when the "Add view point" button is clicked.
    void onClickAddPoint();

    /// This method is called when the "Rename view point" button is clicked.
    void onClickRenamePoint();

    /// This method is called when the "Remove view point" button is clicked.
    void onClickRemovePoint();

    /// This method is called when the "Remove all points" button is clicked.
    void onClickRemoveAllPoints();

    /// This method is called when an item in the QListWidget is clicked.
    void onClickItem(QListWidgetItem * item);

    /// This method is called when an item in the QListWidget is double clicked.
    void onDoubleClickItem(QListWidgetItem * item);

private:

    /// Current matrix for view point.
    std::string m_transformUID;

    /// Number of view points.
    int m_nbViewPoints;

    /// Widget to add a view point.
    QPointer< QPushButton > m_addPointButton;

    /// Widget to rename selected view point.
    QPointer< QPushButton > m_renamePointButton;

    /// Widget to remove selected view point.
    QPointer< QPushButton > m_removePointButton;

    /// Widget to remove all view points.
    QPointer< QPushButton > m_removeAllPointsButton;

    /// Widget listing all registered view points.
    QPointer< QListWidget> m_viewPointsList;

};

} // uiViewPoints

#endif /*__UIVIEWPOINTS_SVIEWPOINTSEDITOR_HPP__*/

