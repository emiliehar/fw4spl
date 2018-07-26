import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0

Item {
    id: sliceIndexSelector

    property int from: 0
    property int to: 0
    property int sliceIndex: 2

    signal updatedSliceIndex(int index, int value)
    signal updatedSliceType(int from, int to);

    RowLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            color: "transparent"

            ComboBox {
                id: sliceType
                anchors.fill: parent
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                model: ["Sagittal", "Frontal", "Axial"]

                currentIndex: sliceIndexSelector.sliceIndex

                onActivated: {
                    updatedSliceType(sliceIndexSelector.sliceIndex, index)
                    sliceIndexSelector.sliceIndex = index
                }
            }

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "transparent"

            Slider {
                id: slider
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                from: sliceIndexSelector.from
                to: sliceIndexSelector.to
                stepSize: 1

                onValueChanged: {
                    textField.text = value + " / " + to
                    updatedSliceIndex(sliceIndexSelector.sliceIndex, value)
                }
            }
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 75

            color: "transparent"

            Text {
                id: textField
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: slider.value + " / " + slider.to
                enabled: false
                color: Material.color(Material.Green)
                font.bold: true
            }
        }
    }

}
