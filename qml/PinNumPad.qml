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

import QtQuick 2.0
import com.nokia.meego 2.0

Item
{
    id: pinNumPad

    property PinEntry entry

    height: 72 * 5 + 4

    function insertText(text)
    {
        entry.appendChar(text)
    }

    function removeChar()
    {
        entry.removeChar()
    }

    function setEntry(pinType)
    {
        parent.pinType = pinType;

        if (pinType == 'puk')
            {
            entry = pukEntry;
            entry.placeHolderText = pukEntry.stepOneText
            changeTimer.start();
            }
        else if (pinType == 'newpin')
            {
            entry = pukEntry;
            entry.placeHolderText = pukEntry.stepTwoText;
            entry.clear();
            }
        else if (pinType == 'confirm')
            {
            entry = pukEntry;
            entry.placeHolderText = pukEntry.stepThreeText;
            entry.clear();
            }
        else
            {
            entry = pinEntry;
            pinEntry.visible = true;
            pukEntry.visible = false;
            }
    }

    Timer {
        id : quitTimer;
        interval: 1000;
        running: false;
        repeat: false;
        onTriggered: Qt.quit();
    }

    Timer {
        id : changeTimer;
        interval: 1500;
        running: false;
        repeat: false;
        onTriggered: {pukEntry.visible = true; pinEntry.visible = false;}
    }

    Connections {
        target: ofonoSimIf

        onPinOk : {
            entry.placeHolderText = '';
            entry.succeeded();
            quitTimer.start();
        }
        onPinFailed : {
            entry.failed(attemptsLeft);
        }
        onPinNotRequired : {
            entry.notRequired();
        }
        onPinTypeChanged : {
            setEntry(pinType);
        }
    }

    Item
    {
        id: numpad
        width: parent.width; height: 72 * 4;

        Behavior on opacity {PropertyAnimation {duration: 500}}

        Column
        {
            anchors.fill: parent

            Row
            {
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
                NumButton {
                    text: "1";
                    detail: "";
                    onClicked: pinNumPad.insertText('1');
                }
                NumButton {
                    text: "2";
                    detail: "ABC";
                    onClicked: pinNumPad.insertText('2');
                }
                NumButton {
                    text: "3";
                    detail: "DEF";
                    onClicked: pinNumPad.insertText('3');
                }
            }
            Row
            {
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
                NumButton {
                    text: "4";
                    detail: "GHI";
                    onClicked: pinNumPad.insertText('4');
                }
                NumButton {
                    text: "5";
                    detail: "JKL";
                    onClicked: pinNumPad.insertText('5');
                }
                NumButton {
                    text: "6";
                    detail: "MNO";
                    onClicked: pinNumPad.insertText('6');
                }
            }
            Row
            {
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
                NumButton {
                    text: "7";
                    detail: "PGRS";
                    onClicked: pinNumPad.insertText('7');
                }
                NumButton {
                    text: "8";
                    detail: "TUV";
                    onClicked: pinNumPad.insertText('8');
                }
                NumButton {
                    text: "9";
                    detail: "WXYZ";
                    onClicked: pinNumPad.insertText('9');
                }
            }
            Row
            {
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
                NumButton {
                    text: "";
                }
                NumButton {
                    text: "0";
                    detail: "";
                    onClicked: pinNumPad.insertText('0');
                }
                NumButton {
                    text: "DEL";
                    onClicked: pinNumPad.removeChar();
                }
            }
        }
    }

    Item
        {
        id: actions

        width: parent.width; height: 74
        anchors {bottom: parent.bottom}

        Item
        {
            anchors {fill: parent; margins: 4}

            Button {
                id: emergencybutton;
                anchors {left: parent.left}
                width: parent.width / 4
                height: parent.height
                text: "+";
                onClicked: console.log("Emergency calls are not supported");
            }
            Button {
                id: okbutton;
                anchors {left: emergencybutton.right; right: skipbutton.left; margins: 5}
                height: parent.height
                text: "OK";
                onClicked: if (entry.textInput.state == "Input") ofonoSimIf.enterPin(entry.textInput.text.toString());
            }
            Button {
                id: skipbutton;
                anchors {right: parent.right}
                width: parent.width / 4
                height: parent.height
                text: "SKIP";
                onClicked: Qt.quit();
            }
        }
    }
}
