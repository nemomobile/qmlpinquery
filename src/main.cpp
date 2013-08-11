/*
 * Copyright (C) 2013 Robin Burchell <robin+mer@viroteck.net>
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

// Header below is missing from MeeGo OBS
//#include <QSystemScreenSaver>

#include "ofonosimif.h"

#ifdef HAS_BOOSTER
#include <MDeclarativeCache>
#endif

#ifdef HAS_BOOSTER
Q_DECL_EXPORT
#endif
int main(int argc, char **argv)
{
    QGuiApplication *application;
    QQuickView *view;
#ifdef HAS_BOOSTER
    application = MDeclarativeCache::qApplication(argc, argv);
    application->setApplicationName("qmlpinquery");
    application->setOrganizationName("org.nemomobile");
    view = MDeclarativeCache::qQuickView();
#else
    qWarning() << Q_FUNC_INFO << "Warning! Running without booster. This may be a bit slower.";
    QGuiApplication stackApp(argc, argv);
    application->setApplicationName("qmlpinquery");
    application->setOrganizationName("org.nemomobile");
    QQuickView stackView;
    application = &stackApp;
    view = &stackView;
#endif

    OfonoSimIf *ofonoSimIf = new OfonoSimIf();

    if (!ofonoSimIf->pinRequired()) {
        delete ofonoSimIf;
        exit(0);
    }

    QObject::connect(view->engine(), SIGNAL(quit()), application, SLOT(quit()));
    view->rootContext()->setContextProperty("ofonoSimIf", ofonoSimIf);
    view->setSource(QUrl("qrc:/qml/main.qml"));

    // QSystemScreenSaver is not available in MeeGo OBS
    //QSystemScreenSaver *screenSaver = new QSystemScreenSaver();
    //screenSaver->setScreenSaverInhibit();

    view->showFullScreen();

    // Send fake key presses to try to dismiss screensaver if it is alredy active.
    // Don't delete events in this method
    QKeyEvent* keyPress = new QKeyEvent( QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
    QKeyEvent* keyRelease = new QKeyEvent( QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);

    application->postEvent(view, keyPress);
    application->postEvent(view, keyRelease);
    application->sendPostedEvents(0, 0);

    int ret = application->exec();

    //delete screenSaver;
    delete ofonoSimIf;
    return ret;
}
