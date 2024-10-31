import QtQuick 2.0

Item {
    Rectangle {
        width: 200
        height: 200
        color: "#808080"

        Column {
            anchors.horizontalCenter:  parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Text {
                text: "id: " + "13"
            }
            Text {
                text: "name: " + "huwei"
            }
            Text {
                text: "email: " + "weiwei7025130@163.com"
            }
        }
    }

}
