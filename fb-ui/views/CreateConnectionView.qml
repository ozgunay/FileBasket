import QtQuick 2.9
import QtQuick.Controls 2.2
import FB 1.0
import assets 1.0
import components 1.0

Item {
    property Connection newConnection: masterController.ui_newConnection
    Rectangle {
        anchors.fill: parent
        color: Style.colourBackground
    }
    ScrollView {
        id: scrollView
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: commandBar. top
            margins: Style.sizeScreenMargin
        }
        clip: true

        Column {
            spacing: Style.sizeScreenMargin
            width: scrollView.width
            Panel {
                headerText: "Connection Settings"
                contentComponent:
                Column {
                    spacing: Style.sizeControlSpacing
                    StringEditorSingleLine {
                        stringDecorator: newConnection.ui_username
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }
                    StringEditorSingleLine {
                        stringDecorator: newConnection.ui_password
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }
                    IPEditor {
                        ipDecorator: newConnection.ui_hostname
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }
                    IntegerEditor {
                        intDecorator: newConnection.ui_port
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }
                }
            }
        }
    }
    CommandBar {
        id: commandBar
        commandList:
            masterController.ui_commandController.ui_createConnectionViewContextCommands
    }
}
