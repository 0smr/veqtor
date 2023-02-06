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
    color: '#eee'

    component APoint: Rectangle {
        x: 20; y:10
        width: 5; height: 5; color: '#249cd7'; radius: 5
        DragHandler { margin: 10 }
        HoverHandler { cursorShape: Qt.ArrowCursor }
    }

    function p2s(p) { return (p.x+2).toFixed()+','+(p.y+2).toFixed(); }

    Grid {
        spacing: 5
        rows: -1; columns: 1
        horizontalItemAlignment: Grid.AlignHCenter
        anchors.centerIn: parent

        Veqtor { id: veq
            width: 150; height: 150

            // FIXME: Assign element to property.
            // readonly property var fogLights: {'fill':'yellow'}
            property var topLights: {"fill":"yellow"}

            src: "file:///C:/Users/seyye/Desktop/lb.svg";
        }

        Button {
            checkable: true
            onPressed: {
                veq.document.fogLights.fill = checked ? '#330c6e77' : 'transparent';
                veq.topLights = !checked ? {"fill": '#550c6e77'} : {"fill": 'transparent'}
            }
        }
    }
}
