import QtQuick 2.9
import FB 1.0
import assets 1.0

Item {
    property IpDecorator ipDecorator
    height: width > textLabel.width + textValue.width ?
        Style.heightDataControls : Style.heightDataControls * 2

    Flow {
        anchors.fill: parent

        Rectangle {
        width: Style.widthDataControls
        height: Style.heightDataControls
        color: Style.colourBackground
            Text {
                id: textLabel
                anchors {
                    fill: parent
                    margins: Style.heightDataControls / 4
                }
                text: ipDecorator.ui_label
                color: Style.colourDataControlsFont
                font.pixelSize: Style.pixelSizeDataControls
                verticalAlignment: Qt.AlignVCenter
            }
        }
        Rectangle {
            id: background
            width: Style.widthDataControls
            height: Style.heightDataControls
            color: Style.colourDataControlsBackground
            border {
                width: 1
                color: Style.colourDataControlsFont
            }
            TextInput {
                id: textValue
                anchors {
                    fill: parent
                    margins: Style.heightDataControls / 4
                }
                text: ipDecorator.ui_value
                color: Style.colourDataControlsFont
                font.pixelSize: Style.pixelSizeDataControls
                verticalAlignment: Qt.AlignVCenter
            }
        }
        Binding {
            target: ipDecorator
            property: "ui_value"
            value: textValue.text
        }
    }
}
