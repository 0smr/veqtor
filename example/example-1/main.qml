import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.1

import veqtor 0.1

Window {
    id: window

    width: 400
    height: 300
    visible: true
    color: '#fff'

    component APoint: Rectangle {
        width: 5; height: 5; color: '#249cd7'; radius: 2
        DragHandler { margin: 10 }
        HoverHandler { cursorShape: Qt.ArrowCursor }
    }

    Grid {
        spacing: 5
        rows: -1; columns: 1
        horizontalItemAlignment: Grid.AlignHCenter
        anchors.centerIn: parent

        Control {
            width: 200; height: 150
            contentItem: Veqtor {
                id: veq;
                property var topLights: {fill: "#ffaabb66"}
                src: "qrc:/resources/lamborghini.svg";
            }
            background: Rectangle { border.width:1; color:'transparent' }
        }

        Slider {
            from: -1
            onValueChanged: {
                const p = veq.document.topLights;
                p.origin = Qt.point(133,133);
                p.transform = [{t:'rotate', angle: value * 90}];
            }
        }

        Button {
            checkable: true
            onPressed: {
                veq.document.fogLights.fill = checked ? '#330c6e77' : 'transparent';
                veq.topLights = !checked ? {fill: '#550c6e77'} : {fill: 'transparent'}
            }
        }
    }
}
