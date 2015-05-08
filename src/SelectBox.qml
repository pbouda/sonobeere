import QtQuick 2.0
import QtQuick.Layouts 1.1

Column {

    property alias options: selectList.model
    anchors.fill: parent
    id: selectColumn

    Rectangle {
        id: selectBox
        height: parent.height/10
        width: parent.width
        color: "black"

        Text {
            text: " " + sonotopy.currentAudioDeviceName + " ▾"
            color: "white"
            font.pixelSize: parent.height*0.7
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (selectList.visible)
                    selectList.visible = false;
                else
                    selectList.visible = true;
            }
        }
    }

    ListView {
        id: selectList
        visible: false
        height: parent.height/10*9
        width: parent.width

        delegate: Rectangle {
            height: selectColumn.height/10
            width: selectColumn.width
            color: "black"

            Text {
                text: " " + modelData
                font.pixelSize: parent.height*0.7
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    sonotopy.currentAudioDeviceName = modelData;
                    selectList.visible = false;
                }
            }

        }
    }
}

