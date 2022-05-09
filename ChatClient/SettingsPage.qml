import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

Page {
    id: root

    property string defUserName: "Roman"
    property string defHost: "ws://localhost:2424"

    Binding {
            target: backend
            property: "userName"
            value: defUserName
    }

    Binding {
            target: backend
            property: "hostAddress"
            value: defHost
    }


    header: ToolBar {

        ToolButton {
            text: qsTr("Chat")
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.push("qrc:/ChatPage.qml")
        }

        Label {
            id: pageTitle
            text: inConversationWith
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 2

        Label {
            text: qsTr("Username")
        }
        TextField {
            id: userNameTextField
            Layout.fillWidth: true
            text: defUserName
            onTextChanged: backend.userName = text
        }

        Label {
            text: qsTr("Host")
        }
        TextField {
            Layout.fillWidth: true
            text: defHost
            onTextChanged: backend.hostAddress = text
        }

        Item {
            Layout.fillHeight: true
            Layout.columnSpan: 2
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.columnSpan: 2

            RowLayout {
                anchors.centerIn: parent
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        Qt.quit()
                    }
                }
                Button {
                    text: qsTr("OK")
                    onClicked: {
                        backend.connectToHost()
                        root.StackView.view.push("qrc:/ChatPage.qml")
                    }

                }
            }
        }
    }
}
