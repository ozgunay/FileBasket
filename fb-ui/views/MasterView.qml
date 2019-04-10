import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Connection Mangement")
    Connections {
        target: masterController.ui_navigationController
        onGoCreateConnectionView:
            contentFrame.replace("qrc:/views/CreateConnectionView.qml")
        onGoDashboardView: contentFrame.replace("qrc:/views/DashboardView.qml")
        onGoFindConnectionView: contentFrame.replace("qrc:/views/FindConnectionView.qml")
        onGoEditConnectionView:
            contentFrame.replace("qrc:/views/EditConnectionView.qml", {selectedConnection:
            Connection})
    }
    StackView {
        id: contentFrame
        clip: true
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            left: navigationBar.right
        }
        initialItem: Qt.resolvedUrl("qrc:/views/SplashView.qml")
    }
    Rectangle {
        id: navigationBar
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        width: 120
        color: "#000000"
        Column {
            Button {
                text: "Dashboard"
                onClicked:
                    masterController.ui_navigationController.goDashboardView()
            }
            Button {
                text: "New Connection"
                onClicked:
                    masterController.ui_navigationController.goCreateConnectionView()
            }
            Button {
                text: "Find Connection"
                onClicked:
                    masterController.ui_navigationController.goFindConnectionView()
            }
        }
    }
    Component.onCompleted: contentFrame.replace("qrc:/views/DashboardView.qml");
}
