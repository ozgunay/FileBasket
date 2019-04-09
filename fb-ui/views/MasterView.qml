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
        onGoCreateClientView:
            contentFrame.replace("qrc:/views/CreateClientView.qml")
        onGoDashboardView: contentFrame.replace("qrc:/views/DashboardView.qml")
        onGoEditClientView:
            contentFrame.replace("qrc:/views/EditClientView.qml", {selectedClient:
            client})
        onGoFindClientView:
            contentFrame.replace("qrc:/views/FindClientView.qml")
    }
    StackView {
        id: contentFrame
        anchors.fill: parent
        initialItem: Qt.resolvedUrl("qrc:/views/SplashView.qml")
    }
    Rectangle {
        id: navigationBar
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        width: 100
        color: "#000000"
    }
    Component.onCompleted: contentFrame.replace("qrc:/views/DashboardView.qml");
}
