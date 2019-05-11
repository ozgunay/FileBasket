import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import assets 1.0
import components 1.0

Window {
    visible: true
    width: Style.widthWindow
    height: Style.heightWindow
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

    NavigationBar {
        id: navigationBar
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
    Component.onCompleted: contentFrame.replace("qrc:/views/DashboardView.qml");
}
