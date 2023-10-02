// Copyright (C) 2022 smr.
// SPDX-License-Identifier: MIT
// http://0smr.github.io

import QtQuick 2.15

QtObject {
    id: root

    readonly property QtObject priv: QtObject {
        property var rawdata: []
    }

    signal clread();
    signal dataChanged();
    signal dataShifted();
    signal valueRemoved(int index);
    signal valueChanged(int index, real value);
    signal valueInserted(int index, real value);
    signal valueAppended(int length, real value);

    function forEach(callback) { priv.rawdata.forEach(callback); }
    function map(callback) { return priv.rawdata.map(callback); }

    /// getters
    function at(index: int) { return priv.rawdata[index]; }
    function length() { return priv.rawdata.length; }
    function count() { return priv.rawdata.length; }
    function empty() { return priv.rawdata.length <= 0; }
    function data() { return priv.rawdata; }

    function slice(start, end = -1) {
        return priv.rawdata.slice(start, end);
    }

    function last() {
        return !empty() ? priv.rawdata[count() - 1] :
                          undefined;
    }

    function shift(): real {
        const result = priv.rawdata.shift();
        root.dataShifted();
        return result;
    }

    function valueAt(x: real) {
        const i = Math.floor(x), dx = x - i;
        return (at(i + 1) - at(i)) * dx + at(i);
    }

    /// setters
    function clear() { setData(); }
    function setData(data = []) {
        priv.rawdata = data;
        priv.rawdataChanged();
        root.dataChanged();
    }

    function set(index: int, value: real) {
        priv.rawdata[index] = value;
        priv.rawdataChanged();
        root.valueChanged(index, value);
    }

    function remove(index: int, nums: int) {
        priv.rawdata.splice(index, nums);
        priv.rawdataChanged();
        root.valueRemoved(index, nums);
    }

    function append(value: real) {
        priv.rawdata.push(value);
        priv.rawdataChanged();
        root.valueAppended(priv.rawdata.length, value);
    }
}

