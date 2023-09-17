// Copyright (C) 2022 smr.
// SPDX-License-Identifier: MIT
// http://0smr.github.io

import QtQuick 2.15
import QtQuick.Controls 2.15

import veqtor 0.1

Control {
    id: control

    property Range xaxis: Range { max: 10; onChanged: Qt.callLater(priv.reDraw) }
    property Range yaxis: Range { max: 10; onChanged: Qt.callLater(priv.reDraw) }
    property real strokeWidth: 0.4

    readonly property List list: List {
        onClread: priv.plot.clear();
        onDataShifted: priv.plot.shift();
        onValueAppended: priv.reDraw();
        onDataChanged: priv.reDraw();
    }

    onWidthChanged: Qt.callLater(priv.reDraw)
    onHeightChanged: Qt.callLater(priv.reDraw)

    QtObject {
        id: priv
        property var plot: veq.document.plot

        function map(value: real, from, to): real {
            return (value - from[0]) / (from[1] - from[0]) * (to[1] - to[0]) + to[0];
        }

        function valueToPoint(x: int, y: int): point {
            return Qt.point(
                map(x, [0, xaxis.dist - 1], [0, control.availableWidth]),
                map(y, [yaxis.min, yaxis.max], [control.availableHeight, 0])
            )
        }

        function reDraw() {
            priv.plot.clear();
            const partialData = list.slice(xaxis.min, xaxis.max);
            partialData.forEach((data, index) => {
                const point = valueToPoint(index, data);
                priv.plot.lineTo(point);
            });
        }
    }

    contentItem: Control {
        contentItem: Veqtor {
            id: veq
            src: "<svg viewBox='0 0 100 100'><path id='plot' fill='none'/></svg>"
            onSvgLoaded: {
                const path = document.plot;
                root.viewBox.width = Qt.binding(() => control.availableWidth);
                root.viewBox.height = Qt.binding(() => control.availableHeight);
                path.strokeWidth = Qt.binding(() => control.strokeWidth);
                path.stroke = Qt.binding(() => control.palette.highlight);
            }
        }
    }

    background: Item {}
}
