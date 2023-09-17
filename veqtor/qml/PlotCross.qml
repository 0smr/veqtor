// Copyright (C) 2022 smr.
// SPDX-License-Identifier: MIT
// http://0smr.github.io

import QtQuick 2.15
import QtQuick.Controls 2.15

Control {
    id: control

    property real xpos: 0
    property QtObject plot

    readonly property real xvalue: map(xpos, [0, plot.availableWidth], [0, plot.xaxis.dist - 1])
    readonly property real yvalue: plot.list.valueAt(plot.xaxis.min + xvalue);
    readonly property real ypos: map(yvalue, [plot.yaxis.min, plot.yaxis.max], [plot.height, 0]);

    function map(value: real, [a,b], [a2,b2]): real {
        return (value - a) / (b - a) * (b2 - a2) + a2;
    }

    Rectangle {
        y: 0; x: xpos - 0.5
        width: 0.7; height: control.height
        color: control.palette.windowText
        opacity: 0.3
    }

    Rectangle {
        x: 0; y: ypos - 0.5
        width: control.width; height: 0.7
        color: control.palette.windowText
        opacity: 0.3
    }

    Rectangle {
        x: xpos - 0.5; y: ypos - 0.5; width: 1; height: 1
        color: palette.windowText
    }

    Label {
        y: Math.min(ypos + 2, parent.height - implicitHeight - 2)
        x: Math.min(xpos + 2, parent.width - implicitWidth - 2)
        padding: 3
        font.pointSize: 7
        text: yvalue.toFixed(2)

        background: Rectangle {
            border.color: control.palette.windowText
            color: control.palette.dark
            opacity: 0.5
            radius: 3
        }
    }
}
