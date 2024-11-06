import QtQuick 2.0
import QtQuick.Controls 2.0

//Item {
//    Rectangle {
//        width: 200
//        height: 200
//        color: "#808080"

//        Column {
//            anchors.horizontalCenter:  parent.horizontalCenter
//            anchors.verticalCenter: parent.verticalCenter
//            Text {
//                text: "id: " + id
//            }
//            Text {
//                text: "name: " + name
//            }
//            Text {
//                text: "email: " + email
//            }
//        }
//    }

//}


Item {
    width: 300
    height: 150
    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        border.color: "#cccccc"
        radius: 10

        Column {
            spacing: 10
            anchors.fill: parent
            anchors.margins: 10 // 使用 margins 代替 padding

            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter

                Item {
                    width: 50
                    height: 50

                    Rectangle {
                        anchors.fill: parent
                        radius: 25 // 圆形外框
                        color: "#e0e0e0" // 背景色
                        border.color: "#cccccc"
                        border.width: 1

                        Image {
                            source: avatar  // 头像路径
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                        }
                    }
                }

                Column {
                    spacing: 5
                    Text { text: "ID: " + id } // 用户 ID
                    Text { text: "Name: " + username } // 用户姓名
                    Text { text: "Email: " + email } // 用户邮箱
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                Button {
                    id: addFriendButton
                    text: isFriend ? "好友" : "添加好友" // 按钮文本根据好友状态变化
                    onClicked: {
                        // 添加好友的逻辑
                        if (!isFriend) {
                            addFriend(id); // 调用添加好友的函数
                        }
                    }
                }

                Button {
                    text: "发送消息" // 发送消息按钮
                    onClicked: {
                        // 发送消息的逻辑
                        sendMessage(id); // 调用发送消息的函数
                    }
                }
            }
        }
    }
}
