/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTERQT_WIDGET_QTAGSELECTORWIDGET_HPP__
#define __FWDICOMIOFILTERQT_WIDGET_QTAGSELECTORWIDGET_HPP__

#include <QLabel>
#include <QPointer>
#include <QWidget>

#include <dcmtk/dcmdata/dctagkey.h>

#include "fwDicomIOFilterQt/widget/QHexSpinBox.hpp"

#include "fwDicomIOFilterQt/config.hpp"

namespace fwDicomIOFilterQt
{
namespace widget
{

/**
 * @class QTagSelectorWidget
 * @brief Widget used to select a dicom tag
 * @date  2014.
 */
class QTagSelectorWidget : public QWidget
{
    Q_OBJECT;

public:
    /// Constructor
    QTagSelectorWidget(QWidget* parent=0);

    /// Destructor
    ~QTagSelectorWidget();

    /// Set tag
    void setTagValue(const DcmTagKey& tag);

    /// Get tag
    DcmTagKey getTag();

protected Q_SLOTS:
    /// Update tag name when selected tag changes
    void updateTagName(int value =0);

protected:
    /// Group spin box
    QPointer< ::fwDicomIOFilterQt::widget::QHexSpinBox > m_groupSpinBox;

    /// Tag spin box
    QPointer< ::fwDicomIOFilterQt::widget::QHexSpinBox > m_elementSpinBox;

    /// tag name label
    QPointer< QLabel > m_tagNameLabel;

    /// Tag
    DcmTagKey m_tag;

};

} // namespace widget
} // namespace fwDicomIOFilterQt


#endif /* __FWDICOMIOFILTERQT_WIDGET_QTAGSELECTORWIDGET_HPP__ */
