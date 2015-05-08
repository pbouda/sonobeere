import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    color: "black"
    id: window

    Grid {
        id: grid
        anchors.fill: parent
        columns: sonotopy.gridWidth
        rows: sonotopy.gridHeight

        Repeater {
            model: sonotopy.gridMapColors

            Rectangle {
                color: modelData
                width: window.width / sonotopy.gridWidth
                height: window.height / sonotopy.gridHeight
            }
        }
    }


    SelectBox {
        options: sonotopy.availableAudioDevices
        anchors.top: parent.top
    }
}
