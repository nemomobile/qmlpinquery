/*
* This file is part of meego-pinquery
*
* Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
*
* Contact: Sirpa Kemppainen <sirpa.h.kemppainen@nokia.com>
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of Nokia Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <QtDebug>
#include "ofonosimif.h"
#include <qofonomanager.h>
#include <qofonomodem.h>

#define RETRIES_PIN 3
#define RETRIES_PUK 10

OfonoSimIf::OfonoSimIf() :
    m_simManager(0),
    m_pinRequired(false),
    m_attemptsLeft(RETRIES_PIN),
    m_pinType("none")
{
}

void OfonoSimIf::startup()
{
    qDebug() << QString("-->OfonoSimIf::startup");

    QOfonoManager ofonomanager;
    QOfonoModem modem;
    modem.setModemPath(ofonomanager.modems().at(0));
    qDebug() << Q_FUNC_INFO << modem.modemPath();

    m_simManager = new QOfonoSimManager(this);
    m_simManager->setModemPath(modem.modemPath());


    if (m_simManager->pinRequired() == QOfonoSimManager::SimPin) {
        m_pinRequired = true;
        m_pinType = QString("pin");
        m_attemptsLeft = RETRIES_PIN;
    } if (m_simManager->pinRequired() == QOfonoSimManager::SimPuk) {
        m_pinRequired = true;
        m_pinType = QString("puk");
        m_attemptsLeft = RETRIES_PUK;
    }

    pinRetriesChanged(m_simManager->pinRetries());

    connect(m_simManager, SIGNAL(enterPinComplete(QOfonoSimManager::Error,QString)),
            this, SLOT(enterPinComplete(QOfonoSimManager::Error,QString)));
    connect(m_simManager, SIGNAL(resetPinComplete(QOfonoSimManager::Error,QString)),
            this, SLOT(resetPinComplete(QOfonoSimManager::Error,QString)));
    connect(m_simManager, SIGNAL(pinRequiredChanged(int)),
            this, SLOT(pinRequiredChanged(int)));

    connect(m_simManager, SIGNAL(pinRetriesChanged(QVariantMap)),
    this, SLOT(pinRetriesChanged(QVariantMap)));

    if (modem.isValid()) {
        qDebug() << "Modem ok";

        if (!modem.powered()) {
            modem.setPowered(true);
        }

        if (!modem.online()) {
            modem.setOnline(true);
        }
    }
    qDebug() << "<--OfonoSimIf::startup";
}

bool OfonoSimIf::pinRequired()
{
    qDebug() << "-->OfonoSimIf::pinRequired";

    if (!m_simManager) {
        startup();
    }

    qDebug() << "pinRequired:" << m_pinRequired;

    return m_pinRequired;
}

void OfonoSimIf::enterPin(QString pinCode)
{
    qDebug() << "-->OfonoSimIf::enterPin";

    if (!m_simManager) {
        startup();
    }

    if (pinRequired()) {
        if (m_pinType == QString("pin")) {
            m_simManager->enterPin(QOfonoSimManager::SimPin, pinCode);

        } else if (m_pinType == QString("puk")) {
            m_pinType = QString("newpin");
            m_pukInfo.m_puk = pinCode;
            emit pinTypeChanged(m_pinType);

        } else if (m_pinType == QString("newpin")) {
            m_pinType = QString("confirm");
            m_pukInfo.m_newpin = pinCode;
            emit pinTypeChanged(m_pinType);

        } else if (m_pinType == QString("confirm")) {
            if (m_pukInfo.m_newpin == pinCode) {
                m_simManager->resetPin(QOfonoSimManager::SimPuk, m_pukInfo.m_puk, pinCode);
            } else {
                // change from confirm to puk
                m_pinType = QString("puk");
                emit pinTypeChanged(m_pinType);
                emit pinFailed(m_attemptsLeft);
            }

            m_pukInfo.reset();
        }

        qDebug() << "m_pinType" << m_pinType;
    } else {
        emit pinNotRequired();
    }
    qDebug() << "<--OfonoSimIf::enterPin";
}

int OfonoSimIf::attemptsLeft()
{
    return m_attemptsLeft;
}

QString OfonoSimIf::pinType()
{
    return m_pinType;
}

void OfonoSimIf::enterPinComplete(QOfonoSimManager::Error error, const QString &errorString)
{
    qDebug() << "-->OfonoSimIf::enterPinComplete";

    if (error == QOfonoSimManager::NoError) {
        emit pinOk();
    } else {
        // TODO: remove when retries are received from ofono-qt
        if (m_attemptsLeft > 0) {
            m_attemptsLeft--;
        }
        emit pinFailed(m_attemptsLeft);
    }

    qDebug() << "<--OfonoSimIf::enterPinComplete";
}

void OfonoSimIf::resetPinComplete(QOfonoSimManager::Error error, const QString &errorString)
{
    qDebug() << "-->OfonoSimIf::resetPinComplete";

    if (error == QOfonoSimManager::NoError) {
        emit pinOk();
    } else {
        // TODO: remove when retries are received from ofono-qt
        if (m_attemptsLeft > 0) {
            m_attemptsLeft--;
        }

        // change from confirm to puk
        m_pinType = QString("puk");
        emit pinTypeChanged(m_pinType);
        emit pinFailed(m_attemptsLeft);
    }

    qDebug() << "<--OfonoSimIf::resetPinComplete";
}


void OfonoSimIf::pinRequiredChanged(int pinType)
{
    qDebug() << "-->OfonoSimIf::pinRequiredChanged";

    if (pinType == QOfonoSimManager::SimPin) {
        m_pinRequired = true;
        m_pinType = pinType;
        m_attemptsLeft = RETRIES_PIN;
        qDebug() << "OfonoSimIf::pinRequiredChanged: pin";
    } else if (pinType == QOfonoSimManager::SimPuk) {
        m_pinRequired = true;
        m_pinType = pinType;
        m_attemptsLeft = RETRIES_PUK;
        qDebug() << "OfonoSimIf::pinRequiredChanged: puk";
    } else {
        m_pinRequired = false;
        qDebug() << "OfonoSimIf::pinRequiredChanged: NO";
    }

    emit pinTypeChanged(m_pinType);
}

void OfonoSimIf::pinRetriesChanged(const QVariantMap &pinRetries)
{
    if (pinRetries.keys().contains("1")) { //QOfonoSimManager::PinType
        m_attemptsLeft = pinRetries["1"].toInt();
    }
}
