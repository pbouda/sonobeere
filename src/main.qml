import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true

    GridView {
        anchors.fill: parent
        model: sonotopy.gridMap

        delegate: Rectangle {
            color: white
            width: parent.width / sonotopy.gridWidth
            height: parent.height / sonotopy.gridHeight
            Text {
                anchors.centerIn: parent
                text: "T"
                color: black
            }
        }
    }

}
