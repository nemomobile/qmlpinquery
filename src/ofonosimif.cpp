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

#define PIN_RETRIES 3

OfonoSimIf::OfonoSimIf() :
    m_simManager(0),
    m_pinRequired(false),
    m_attemptsLeft(PIN_RETRIES)
{
}

void OfonoSimIf::startup()
{
    qDebug() << QString("-->OfonoSimIf::startup");

    m_simManager = new OfonoSimManager(OfonoModem::AutomaticSelect, "", this);

    if (m_simManager->pinRequired() == QString("pin")) {
        m_pinRequired = true;
    }

    //OfonoPinRetries retries = m_simManager->pinRetries();

    //if (retries.contains("pin")) {
    //    m_attemptsLeft = retries["pin"];
    //}

    connect(m_simManager, SIGNAL(enterPinComplete(bool)), this, SLOT(enterPinComplete(bool)));
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

    qDebug() << QString("pin") << pinCode;

    if (pinRequired()) {
        m_simManager->enterPin(QString("pin"), pinCode);
    } else {
        emit pinNotRequired();
    }
    qDebug() << QString("<--OfonoSimIf::enterPin");
}

int OfonoSimIf::attemptsLeft()
{
    return m_attemptsLeft;
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

void OfonoSimIf::pinRequiredChanged(const QString &pinType)
{
    if (pinType == QString("pin")) {
        m_pinRequired = true;
        qDebug() << QString("OfonoSimIf::pinRequiredChanged: YES");
    } else {
        m_pinRequired = false;
        qDebug() << QString("OfonoSimIf::pinRequiredChanged: NO");
    }
}

//void OfonoSimIf::pinRetriesChanged(const OfonoPinRetries &pinRetries)
//{
//    if (pinRetries.contains("pin")) {
//        m_attemptsLeft = pinRetries["pin"];
//    }
//}
