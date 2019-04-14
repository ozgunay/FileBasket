pragma Singleton
import QtQuick 2.9

Item {
    ////////////////////////////////////// FONT ///////////////////////////////////////////////////
    property alias fontAwesome: fontAwesomeLoader.name
    readonly property color colourBackground: "#efefef"
    FontLoader {
        id: fontAwesomeLoader
        source: "qrc:/assets/fontawesome.otf"
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// NAVIGATION BAR //////////////////////////////////////////////
    readonly property color colourNavigationBarBackground: "#000000"
    readonly property color colourNavigationBarFont: "#ffffff"
    readonly property int pixelSizeNavigationBarIcon: 42
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// BUTTON SETTINGS /////////////////////////////////////////////
    readonly property real widthNavigationButtonIcon: 80
    readonly property real heightNavigationButtonIcon: widthNavigationButtonIcon
    readonly property real widthNavigationButtonDescription: 240
    readonly property real heightNavigationButtonDescription: heightNavigationButtonIcon
    readonly property real widthNavigationButton: widthNavigationButtonIcon +
        widthNavigationButtonDescription
    readonly property real heightNavigationButton:
        Math.max(heightNavigationButtonIcon, heightNavigationButtonDescription)
    readonly property int pixelSizeNavigationBarText: 22
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// COMMAND BAR SETTINGS /////////////////////////////////////////////
    readonly property color colourCommandBarBackground: "#cecece"
    readonly property color colourCommandBarFont: "#131313"
    readonly property color colourCommandBarFontDisabled: "#636363"
    readonly property real heightCommandBar: heightCommandButton
    readonly property int pixelSizeCommandBarIcon: 32
    readonly property int pixelSizeCommandBarText: 12
    readonly property real widthCommandButton: 80
    readonly property real heightCommandButton: widthCommandButton
    ///////////////////////////////////////////////////////////////////////////////////////////////
    readonly property real widthNavigationBarCollapsed: widthNavigationButtonIcon
    readonly property real heightNavigationBarExpanded: widthNavigationButton
}
