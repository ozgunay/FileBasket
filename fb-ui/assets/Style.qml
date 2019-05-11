pragma Singleton
import QtQuick 2.9

Item {

    function hscale(size) {
        return Math.round(size * (widthWindow / widthWindowReference))
    }
    function vscale(size) {
        return Math.round(size * (heightWindow / heightWindowReference))
    }
    function tscale(size) {
        return Math.round((hscale(size) + vscale(size)) / 2)
    }

    readonly property real widthWindow: 640
    readonly property real heightWindow: 480
    readonly property real widthWindowReference: 640
    readonly property real heightWindowReference: 480

    ////////////////////////////////////// FONT ///////////////////////////////////////////////////
    property alias fontAwesome: fontAwesomeLoader.name
    readonly property color colourBackground: "#efefef"

    readonly property real sizeScreenMargin: tscale(20)
    readonly property real sizeControlSpacing: tscale(10)

    FontLoader {
        id: fontAwesomeLoader
        source: "qrc:/assets/fontawesome.otf"
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// NAVIGATION BAR //////////////////////////////////////////////
    readonly property color colourNavigationBarBackground: "#000000"
    readonly property color colourNavigationBarFont: "#ffffff"
    readonly property int pixelSizeNavigationBarIcon: 32
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// BUTTON SETTINGS /////////////////////////////////////////////
    readonly property real widthNavigationButtonIcon: 60
    readonly property real heightNavigationButtonIcon: widthNavigationButtonIcon
    readonly property real widthNavigationButtonDescription: 120
    readonly property real heightNavigationButtonDescription: heightNavigationButtonIcon
    readonly property real widthNavigationButton: widthNavigationButtonIcon +
        widthNavigationButtonDescription
    readonly property real heightNavigationButton:
        Math.max(heightNavigationButtonIcon, heightNavigationButtonDescription)
    readonly property int pixelSizeNavigationBarText: 15
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// COMMAND BAR SETTINGS ////////////////////////////////////////
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
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// DATA CONTROLS ///////////////////////////////////////////////
    readonly property color colourDataControlsBackground: "#ffffff"
    readonly property color colourDataControlsFont: "#131313"
    readonly property int pixelSizeDataControls: 18
    readonly property real widthDataControls: 400
    readonly property real heightDataControls: 40
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// PANEL //////////////////////////////////////////////////////
    readonly property color colourPanelBackground: "#ffffff"
    readonly property color colourPanelBackgroundHover: "#ececec"
    readonly property color colourPanelHeaderBackground: "#131313"
    readonly property color colourPanelHeaderFont: "#ffffff"
    readonly property color colourPanelFont: "#131313"
    readonly property int pixelSizePanelHeader: tscale(18)
    readonly property real heightPanelHeader: vscale(40)
    readonly property real sizeShadowOffset: tscale(5)
    readonly property color colourShadow: "#dedede"
    ///////////////////////////////////////////////////////////////////////////////////////////////

}
