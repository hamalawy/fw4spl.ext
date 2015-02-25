/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwData/Object.hpp>
#include <fwServices/Base.hpp>
#include <boost/type.hpp>
#include <boost/integer_traits.hpp>
#include <QWidget>
#include <QHBoxLayout>
#include <QString>
#include <QFormLayout>
#include <QVariant>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <ioZMQ/Patterns.hpp>
#include "uiZMQ/SZMQConfigEditor.hpp"

namespace uiZMQ
{
const ::fwCom::Signals::SignalKeyType SZMQConfigEditor::s_CONFIGURATION_UPDATED_SIGNAL = "configurationUpdated";

//-----------------------------------------------------------------------------

SZMQConfigEditor::SZMQConfigEditor()
{
    m_configurationUpdatedSig = ConfigurationUpdatedSignalType::New();

    ::fwCom::HasSignals::m_signals (SZMQConfigEditor::s_CONFIGURATION_UPDATED_SIGNAL,  m_configurationUpdatedSig);
}

//-----------------------------------------------------------------------------

SZMQConfigEditor::~SZMQConfigEditor() throw()
{
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::configuring() throw (::fwTools::Failed)
{
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QPushButton *validateButton;
    QPushButton *cancelButton;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *portLayout;

    mainLayout     = new QVBoxLayout();
    buttonLayout   = new QHBoxLayout();
    validateButton = new QPushButton("Validate");
    cancelButton   = new QPushButton("Cancel");
    formLayout     = new QFormLayout();
    m_portWidget   = new QWidget();
    portLayout     = new QHBoxLayout();
    m_host         = new QLineEdit();
    m_modeBox      = new QComboBox();
    m_patternBox   = new QComboBox();
    m_title        = new QLabel("Host");
    m_protocolBox  = new QComboBox();
    m_port         = new QSpinBox();

    m_modeBox->addItem(QString::fromStdString("Client"), QVariant(static_cast<int>(::zmqNetwork::Socket::Client)));
    m_modeBox->addItem(QString::fromStdString("Server"), QVariant(static_cast<int>(::zmqNetwork::Socket::Server)));
    portLayout->addWidget(new QLabel("Port : "));
    portLayout->addWidget(m_port);
    m_protocolBox->addItem(QString::fromStdString("tcp"));
    m_protocolBox->addItem(QString::fromStdString("ipc"));
    m_modeBox->setCurrentIndex(0);
    this->initPatternBox();

    m_port->setMinimum(0);
    m_port->setMaximum(boost::integer_traits<boost::uint16_t>::const_max);

    formLayout->addRow(QString::fromStdString("Protocol : "), m_protocolBox);
    formLayout->addRow(QString::fromStdString("Mode : "), m_modeBox);
    formLayout->addRow(QString::fromStdString("Pattern : "), m_patternBox);
    formLayout->addRow(m_title, m_host);
    formLayout->addRow(m_portWidget);


    buttonLayout->addWidget(validateButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    m_dialog.setModal(true);
    m_dialog.setLayout(mainLayout);
    m_portWidget->setLayout(portLayout);

    QObject::connect(validateButton, SIGNAL(clicked()), this, SLOT(onValidate()));
    QObject::connect(cancelButton, SIGNAL(clicked()), &m_dialog, SLOT(close()));
    m_protocol = m_protocolBox->currentText().toStdString() + "://";
    QObject::connect(m_protocolBox, SIGNAL(currentIndexChanged( const QString &)), this,
                     SLOT(onChangeProtocol(QString const&)));
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::starting() throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::stopping() throw (::fwTools::Failed)
{
    m_dialog.close();
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::updating() throw (::fwTools::Failed)
{
    m_dialog.show();
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::onChangeProtocol(QString const & protocol)
{
    m_protocol = protocol.toStdString() + "://";
    if (m_protocol == "ipc://")
    {
        m_portWidget->hide();
        m_title->setText("File : ");
    }
    else
    {
        m_portWidget->show();
        m_title->setText("Host : ");
    }
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::onValidate()
{
    std::stringstream stream;
    std::string host;
    ::zmqNetwork::Socket::PatternMode pattern;
    ::zmqNetwork::Socket::SocketMode sockMode;

    sockMode = static_cast< ::zmqNetwork::Socket::SocketMode>(m_modeBox->currentData().toInt());
    pattern  = static_cast< ::zmqNetwork::Socket::PatternMode>(m_patternBox->currentData().toInt());
    if (m_protocol == "tcp://")
    {
        stream << m_protocol << m_host->text().toStdString()  << ":" << m_port->value();
    }
    else
    {
        stream << m_protocol << m_host->text().toStdString();
    }
    stream >> host;
    fwServicesNotifyMacro(this->getLightID(), m_configurationUpdatedSig, (pattern, sockMode, host));
    m_dialog.close();
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::receiving (::fwServices::ObjectMsg::csptr _msg) throw (::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void SZMQConfigEditor::swapping() throw (::fwTools::Failed)
{
    // Classic default approach to update service when oject change
    this->stopping();
    this->starting();
}

//-----------------------------------------------------------------------------

} // namespace uiZMQ


