import QtQuick 2.9
import assets 1.0

Item {
    property bool isCollapsed: true
    anchors {
        top: parent.top
        bottom: parent.bottom
        left: parent.left
    }
    width: isCollapsed ? Style.widthNavigationBarCollapsed :
        Style.heightNavigationBarExpanded
    Rectangle {
        anchors.fill: parent
        color: Style.colourNavigationBarBackground
        Column {
            width: parent.width
            NavigationButton {
                iconCharacter: "\uf0c9"
                description: ""
            }
            NavigationButton {
                iconCharacter: "\uf015"
                description: "Dashboard"
                hoverColour: "#666699"
                onNavigationButtonClicked:
                    masterController.ui_navigationController.goDashboardView();
            }
            NavigationButton {
                iconCharacter: "\uf6ff"
                description: "New Connection"
                hoverColour: "#666699"
                onNavigationButtonClicked:
                    masterController.ui_navigationController.goCreateConnectionView();
            }
            NavigationButton {
                iconCharacter: "\uf002"
                description: "Find Connection"
                hoverColour: "#666699"
                onNavigationButtonClicked:
                    masterController.ui_navigationController.goFindConnectionView();
            }
        }
    }
    NavigationButton {
        iconCharacter: "\uf0c9"
        description: ""
        hoverColour: "#666699"
        onNavigationButtonClicked: isCollapsed = !isCollapsed
    }
}
