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

    m_simManager = new OfonoSimManager(OfonoModem::AutomaticSelect, "", this);

    if (m_simManager->pinRequired() == QString("pin")) {
        m_pinRequired = true;
        m_pinType = QString("pin");
        m_attemptsLeft = RETRIES_PIN;
    } if (m_simManager->pinRequired() == QString("puk")) {
        m_pinRequired = true;
        m_pinType = QString("puk");
        m_attemptsLeft = RETRIES_PUK;
    }

    //OfonoPinRetries retries = m_simManager->pinRetries();

    //if (retries.contains("pin")) {
    //    m_attemptsLeft = retries["pin"];
    //}

    connect(m_simManager, SIGNAL(enterPinComplete(bool)), this, SLOT(enterPinComplete(bool)));
    connect(m_simManager, SIGNAL(resetPinComplete(bool)), this, SLOT(resetPinComplete(bool)));
    connect(m_simManager, SIGNAL(pinRequiredChanged(const QString&)), this, SLOT(pinRequiredChanged(const QString&)));
    //connect(m_simManager, SIGNAL(pinRetriesChanged(const OfonoPinRetries&)), this, SLOT(pinRetriesChanged(const OfonoPinRetries&)));

    if (m_simManager->modem()->isValid()) {
        qDebug() << QString("Modem ok");

        if (!m_simManager->modem()->powered()) {
            m_simManager->modem()->setPowered(true);
        }

        if (!m_simManager->modem()->online()) {
            m_simManager->modem()->setOnline(true);
        }
    } else {
        qDebug() << m_simManager->errorName();
        qDebug() << m_simManager->errorMessage();
    }

    qDebug() << QString("<--OfonoSimIf::startup");
}

bool OfonoSimIf::pinRequired()
{
    qDebug() << QString("-->OfonoSimIf::pinRequired");

    if (!m_simManager) {
        startup();
    }

    qDebug() << QString("pinRequired:") << m_pinRequired;

    return m_pinRequired;
}

void OfonoSimIf::enterPin(QString pinCode)
{
    qDebug() << QString("-->OfonoSimIf::enterPin");

    if (!m_simManager) {
        startup();
    }

    qDebug() << QString("pinCode") << pinCode;

    if (pinRequired()) {
        if (m_pinType == QString("pin")) {
            m_simManager->enterPin(m_pinType, pinCode);

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
                m_simManager->resetPin(QString("puk"), m_pukInfo.m_puk, pinCode);
            } else {
                // change from confirm to puk
                m_pinType = QString("puk");
                emit pinTypeChanged(m_pinType);
                emit pinFailed(m_attemptsLeft);
            }

            m_pukInfo.reset();
        }

        qDebug() << QString("m_pinType") << m_pinType;
    } else {
        emit pinNotRequired();
    }
    qDebug() << QString("<--OfonoSimIf::enterPin");
}

int OfonoSimIf::attemptsLeft()
{
    return m_attemptsLeft;
}

QString OfonoSimIf::pinType()
{
    return m_pinType;
}

void OfonoSimIf::enterPinComplete(bool success)
{
    qDebug() << QString("-->OfonoSimIf::enterPinComplete");

    if (success) {
        emit pinOk();
    } else {
        // TODO: remove when retries are received from ofono-qt
        if (m_attemptsLeft > 0) {
            m_attemptsLeft--;
        }

        emit pinFailed(m_attemptsLeft);

        qDebug() << m_simManager->errorName();
        qDebug() << m_simManager->errorMessage();
    }

    qDebug() << QString("<--OfonoSimIf::enterPinComplete");
}

void OfonoSimIf::resetPinComplete(bool success)
{
    qDebug() << QString("-->OfonoSimIf::resetPinComplete");

    if (success) {
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
        qDebug() << m_simManager->errorName();
        qDebug() << m_simManager->errorMessage();
    }

    qDebug() << QString("<--OfonoSimIf::resetPinComplete");
}

void OfonoSimIf::pinRequiredChanged(const QString &pinType)
{
    qDebug() << QString("-->OfonoSimIf::pinRequiredChanged");

    if (pinType == QString("pin")) {
        m_pinRequired = true;
        m_pinType = pinType;
        m_attemptsLeft = RETRIES_PIN;
        qDebug() << QString("OfonoSimIf::pinRequiredChanged: pin");
    } else if (pinType == QString("puk")) {
        m_pinRequired = true;
        m_pinType = pinType;
        m_attemptsLeft = RETRIES_PUK;
        qDebug() << QString("OfonoSimIf::pinRequiredChanged: puk");
    } else {
        m_pinRequired = false;
        qDebug() << QString("OfonoSimIf::pinRequiredChanged: NO");
    }

    emit pinTypeChanged(m_pinType);
}

//void OfonoSimIf::pinRetriesChanged(const OfonoPinRetries &pinRetries)
//{
//    if (pinRetries.contains("pin")) {
//        m_attemptsLeft = pinRetries["pin"];
//    }
//}
