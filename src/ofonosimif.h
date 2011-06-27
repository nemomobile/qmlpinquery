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

#ifndef OFONOSIMIF_H
#define OFONOSIMIF_H

#include <QtCore/QObject>
#include <ofono-qt/ofonosimmanager.h>

class PukInfo : public QObject
{
    Q_OBJECT

public:
    PukInfo() {};
    void reset() { m_puk = QString(""); m_newpin = QString("");};

public:
    QString m_puk;
    QString m_newpin;
};

class OfonoSimIf : public QObject
{
    Q_OBJECT

public:
    OfonoSimIf();
    bool pinRequired();

Q_SIGNALS:
    void pinOk();
    void pinFailed(int attemptsLeft);
    void pinNotRequired();
    void pinTypeChanged(QString pinType);

public Q_SLOTS:
    void enterPin(QString pinCode);
    int attemptsLeft();
    QString pinType(); // 'pin', 'puk', 'newpin', 'confirm'

private slots:
    void enterPinComplete(bool success);
    void resetPinComplete(bool success);
    void pinRequiredChanged(const QString &pinType);
    //void pinRetriesChanged(const OfonoPinRetries &pinRetries);

private:
    void startup();

private:
    OfonoSimManager *m_simManager;
    bool m_pinRequired;
    int m_attemptsLeft;
    QString m_pinType;
    PukInfo m_pukInfo;
};

#endif // OFONOSIMIF_H
