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
import com.nokia.meego 1.2

Page {
    id: pinPage
    property string pinType: ofonoSimIf.pinType()

    PinEntry {
        id: pinEntry
        visible: true
        placeHolderText: 'Enter PIN code'
        errorText: 'Incorrect PIN code'
        okText: 'PIN code correct'
        anchors {
            top: parent.top
            bottom: numPad.top
            left: parent.left
            right: parent.right
            margins: 10
        }
    }

    PinEntry
    {
        id: pukEntry
        visible: false
        placeHolderText: 'Enter PUK code'
        errorText: 'Resetting PIN code failed'
        okText: 'PIN code resetted successfully'

        property string stepOneText: 'Enter PUK code'
        property string stepTwoText: 'Enter new pin code'
        property string stepThreeText: 'Re-enter new pin code'

        anchors {
            top: parent.top
            bottom: numPad.top
            left: parent.left
            right: parent.right
            margins: 10
        }
    }

    PinNumPad
    {
        id: numPad
        width: parent.width
        anchors {bottom: parent.bottom; right: parent.right}
        entry: setEntry(pinPage.pinType)

    }
}
