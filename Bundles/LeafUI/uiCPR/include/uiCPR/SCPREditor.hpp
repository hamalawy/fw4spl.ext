/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UICPR_SCPREDITOR_HPP__
#define __UICPR_SCPREDITOR_HPP__

#include <QObject>
#include <QPointer>

#include <vector>
#include <string>

#include <fwTools/Failed.hpp>

#include <fwCom/Signals.hpp>

#include <gui/editor/IEditor.hpp>

#include "uiCPR/config.hpp"

class QSlider;
class QLineEdit;
class QDoubleSpinBox;
class QPushButton;

namespace uiCPR
{

/**
 * @brief Creates a GUI to set parameters for a curved planar reformation (CPR).
 * @class SCPREditor
 */
class UICPR_CLASS_API SCPREditor : public QObject,
                                   public ::gui::editor::IEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro((SCPREditor)(::gui::editor::IEditor));

    /**
     * @name Constructor/Destructor.
     * @{ */
    UICPR_API SCPREditor () throw();
    UICPR_API virtual ~SCPREditor () throw();
    /**  @} */

    /**
     * @name Signal keys.
     * @{ */
    UICPR_API static const ::fwCom::Signals::SignalKeyType s_HEIGHT_CHANGED_SIG;
    UICPR_API static const ::fwCom::Signals::SignalKeyType s_SPACING_CHANGED_SIG;
    UICPR_API static const ::fwCom::Signals::SignalKeyType s_SLIDER_PROGRESSED_SIG;
    /**  @} */

    /**
     * @name Signal types.
     * @{ */
    typedef ::fwCom::Signal < void (double) > HeightChangedSignalType;
    typedef ::fwCom::Signal < void (double) > SpacingChangedSignalType;
    typedef ::fwCom::Signal < void (double) > SliderProgressedSignalType;
    /**  @} */

protected:

    /**
     * @name Overrides
     * @{ */
    virtual void starting() throw(::fwTools::Failed);
    virtual void stopping() throw(::fwTools::Failed);
    virtual void receiving(::fwServices::ObjectMsg::csptr _msg) throw(::fwTools::Failed);
    virtual void updating() throw(::fwTools::Failed);
    /**  @} */


    /**
     * @brief Overrides IService::configuring().
     * Configuration example :
       @verbatim
       <config>
        <sourceImage uid="..." />   <!-- UID of source image (::fwData::Image)  -->
       </config>
       @endverbatim
     *
     * @throw fwTools::Failed
     */
    virtual void configuring() throw(fwTools::Failed);

    /// Signal emitted when the height value changes.
    HeightChangedSignalType::sptr m_sigHeightChanged;

    /// Signal emitted when the spacing value changes.
    SpacingChangedSignalType::sptr m_sigSpacingChanged;

    /// Signal emitted when the slider value changes.
    SliderProgressedSignalType::sptr m_sigSliderProgressed;

protected Q_SLOTS:

    /// Triggered when the slider value changes.
    void onChangeSliderValue(int newSliderValue);

    /// Triggered when the height value changes.
    void onChangeHeightValue (double newHeight);

    /// Triggered when the spacing value changes.
    void onChangeSpacingValue (double newSpacing);

    /// Triggered when the "Compute" button is clicked.
    void onClickComputeSlotType();

private:

    /// UID of source image to compute CPR.
    std::string m_sourceImageUID;

    /// CPR current rotation angle.
    double m_angle;

    /// CPR current spacing.
    double m_spacing;

    /// CPR minimum spacing, relative to image spacing.
    double m_minSpacing;

    /// CPR maximum spacing, relative to image spacing.
    double m_maxSpacing;

    /// CPR current height.
    double m_height;

    /// CPR minimum height.
    static const double s_MIN_HEIGHT;

    /// CPR maximum height, relative to image size.
    double m_maxHeight;

    /// Widget to adjust CPR height.
    QPointer< QDoubleSpinBox > m_heightSpinBox;

    /// Widget to adjust CPR spacing.
    QPointer< QDoubleSpinBox > m_spacingSpinBox;

    /// Widget to adjust CPR rotation angle.
    QPointer< QSlider > m_rotationSlider;

    /// Show current CPR rotation angle.
    QPointer< QLineEdit > m_angleText;

    /// Widget to trigger CPR computation.
    QPointer< QPushButton> m_computeButton;


};

} // uiCPR

#endif //__UICPR_SCPREDITOR_HPP__

