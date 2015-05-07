import QtQuick 2.4
import QtQuick.Window 2.2


Window {
    visible: true
    color: "blue"

    Grid {
        anchors.fill: parent
        columns: sonotopy.gridWidth
        rows: sonotopy.gridHeight

        Repeater {
            model: sonotopy.gridHeight
            id: row

            Repeater {
                model: sonotopy.gridWidth
                Rectangle {
                    color: "white"
                    width: parent.width / sonotopy.gridWidth
                    height: parent.height / sonotopy.gridHeight
                    Text {
                        anchors.centerIn: parent
                        text: sonotopy.gridMapAt(index, row.index)
                        color: "black"
                    }
                }
            }
        }

    }

}
