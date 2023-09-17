// Copyright (C) 2022 smr.
// SPDX-License-Identifier: MIT
// http://0smr.github.io

import QtQuick 2.15

QtObject {
    readonly property real dist: Math.abs(max - min)
    property real min: 0
    property real max: 1

    signal changed()

    onMaxChanged: changed()
    onMinChanged: changed()
}
