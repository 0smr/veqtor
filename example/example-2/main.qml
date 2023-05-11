import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import veqtor 0.1

ApplicationWindow {
    id: window
    width: 400
    height: 150
    visible: true;

    Row {
        anchors.fill: parent
        TextArea {
            id: tfield
            width: window.width - veq.width
            height: parent.height
            text: `<svg viewBox='0 0 30 30'>\n`+
                  `  <path id='p1' stroke='#2af' d='M 0 0 c 3.8 3.8 10.3 3.8 14 0'/>\n`+
                  `  <path id='p2' stroke='#f329' d='M 0 0 a 10 10 0 0 0 14 0'/>\n`+
                  `<svg>`
            selectByMouse: true
            wrapMode: TextField.WrapAnywhere
        }

        Veqtor {
            id: veq; src: tfield.text
            width: window.height
            onSvgLoaded: {
                const path = document.p1;
                path.moveTo(Qt.point(0, 0));
                path.lineTo(Qt.point(10, 10));
                path.lineTo(Qt.point(10, 30), 1);
            }
        }
    }
}
