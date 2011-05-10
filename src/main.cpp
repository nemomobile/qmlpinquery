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

#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QEvent>
#include <QKeyEvent>

// Header below is missing from MeeGo OBS
//#include <QSystemScreenSaver>

#include "qmlapplicationviewer.h"
#include "ofonosimif.h"

//QTM_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QmlApplicationViewer viewer;
    bool startedFromDesktop(false);

    if (argc==2){
        if (argv[1] == QString("-desktop")){
            startedFromDesktop = true;
        }
    }

    QDeclarativeContext *context = new QDeclarativeContext(viewer.rootContext());

    if (context->parentContext()) {
        context = context->parentContext();
    }

    OfonoSimIf *ofonoSimIf = new OfonoSimIf();
    context->setContextProperty("ofonoSimIf", ofonoSimIf);

    if (ofonoSimIf->pinRequired() || startedFromDesktop) {
        viewer.setResizeMode(QDeclarativeView::SizeRootObjectToView);
        viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
        viewer.setMainQmlFile(QLatin1String("qml/meego-pinquery/main.qml"));

        // QSystemScreenSaver is not available in MeeGo OBS
        //QSystemScreenSaver *screenSaver = new QSystemScreenSaver();
        //screenSaver->setScreenSaverInhibit();

        viewer.showExpanded();

        // Send fake key presses to try to dismiss screensaver if it is alredy active.
        // Don't delete events in this method
        QKeyEvent* keyPress = new QKeyEvent( QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
        QKeyEvent* keyRelease = new QKeyEvent( QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);

        app.postEvent(&viewer, keyPress);
        app.postEvent(&viewer, keyRelease);
        app.sendPostedEvents(0, 0);

        int ret = app.exec();

        //delete screenSaver;
        delete ofonoSimIf;
        return ret;
    } else {
        delete ofonoSimIf;
        exit(1);
    }
}
