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

import QtQuick 1.0

Item
{
    id: numButton

    property Item numpad

    property string text: ""
    property string detail: ""

    signal clicked

    width: parent.width / 3
    height: 72

    SystemPalette { id: activePalette; colorGroup: SystemPalette.Active }

    Rectangle
    {
        anchors {fill: parent; margins: 2}
        color: activePalette.button
        border {width: 1}
        radius: width / 16

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: !mouseArea.pressed ? activePalette.light : activePalette.button
            }
            GradientStop {
                position: 1.0
                color: !mouseArea.pressed ? activePalette.button : activePalette.dark
            }
        }
    }

    Text
    {
        width: parent.width
        height: parent.height * 0.6
        anchors {top: parent.top; margins: 4}
        text: parent.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font {pixelSize: parent.height / 2}
        color: activePalette.buttonText
    }

    Text
    {
        width: parent.width
        height: parent.height / 4
        anchors {bottom: parent.bottom; margins: 8}
        text: parent.detail
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font {pixelSize: parent.height / 4}
        color: activePalette.dark
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked: numButton.clicked();
    }
}
