import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15

Page {
    id: root
    header: ToolBar {

        ToolButton {
            text: qsTr("Back")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.pop()
        }

        Label {
            id: pageTitle
            text: inConversationWith
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

ColumnLayout {
    anchors.fill: parent

    ListView {
        id: listView
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: pane.leftPadding + messageField.leftPadding
        displayMarginBeginning: 40
        displayMarginEnd: 40
        verticalLayoutDirection: ListView.BottomToTop
        spacing: 12

        model: sqlConversationModel
        delegate: Column {
            anchors.right: sentByMe ? listView.contentItem.right : undefined
            spacing: 6

            readonly property bool sentByMe: model.author === backend.userName

            Row {
                id: messageRow
                spacing: 6
                anchors.right: sentByMe ? parent.right : undefined

                Image {
                    id: avatar
                    source: !sentByMe ? "qrc:/" + model.author.replace(" ", "_") + ".png" : ""
                }

                Rectangle {
                    width: Math.min(messageText.implicitWidth + 24, listView.width - avatar.width - messageRow.spacing)
                    height: messageText.implicitHeight + 24
                    color: sentByMe ? "lightgrey" : "steelblue"

                    Label {
                        id: messageText
                        text: model.message
                        color: sentByMe ? "black" : "white"
                        anchors.fill: parent
                        anchors.margins: 12
                        wrapMode: Label.Wrap
                    }
                }
            }

            Label {
                id: timestampText
                text: sentByMe ? Qt.formatDateTime(model.timestamp, "d MMM hh:mm")
                               : model.author + ' ' + Qt.formatDateTime(model.timestamp, "d MMM hh:mm")
                color: "lightgrey"
                anchors.right: sentByMe ? parent.right : undefined
            }
        }

        ScrollBar.vertical: ScrollBar {}
    }

    Pane {
        id: pane
        Layout.fillWidth: true

        RowLayout {
            width: parent.width

            TextArea {
                id: messageField
                Layout.fillWidth: true
                placeholderText: qsTr("Compose message")
                wrapMode: TextArea.Wrap
            }

            Button {
                id: sendButton
                text: qsTr("Send")
                enabled: messageField.length > 0
                onClicked: {
                    backend.sendMessage(messageField.text)
                    messageField.text = ""
                }
            }
        }
    }
}
}
