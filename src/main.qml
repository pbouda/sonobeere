import QtQuick 2.4
import QtQuick.Window 2.2


Window {
    visible: true
    color: "blue"

    Grid {
        id: main
        anchors.fill: parent
        columns: sonotopy.gridWidth
        rows: sonotopy.gridHeight

        Repeater {
            model: sonotopy.gridMapColors

            Rectangle {
                color: modelData
                width: main.width / sonotopy.gridWidth
                height: main.height / sonotopy.gridHeight
            }
        }

    }

}
