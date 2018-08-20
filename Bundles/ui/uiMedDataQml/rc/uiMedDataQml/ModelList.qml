import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Material 2.0
import uiMedDataQml 1.0

Item {
    id: modelList

    signal serviceCreated(var srv)

    enabled: false

    Component.onCompleted: {
        serviceCreated(modelSeriesList)
    }

    SModelSeriesList {
        id: modelSeriesList

        listModel: organListModel

        onStarted: {
            modelList.enabled = true
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {

            Layout.fillWidth: true
            Layout.maximumHeight: 50

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"

                CheckBox {
                    id: hideAll
                    text: "Hide all organs"
                    anchors.fill: parent
                    anchors.rightMargin: 4
                    anchors.left: parent.left
                    onCheckedChanged: {
                        modelSeriesList.onShowReconstructions(checked)
                        checkAll.enabled = !checked
                        uncheckAll.enabled = !checked
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 100
                color: "transparent"

                Button {
                    id: checkAll
                    text: "Check all"
                    anchors.fill: parent
                    anchors.rightMargin: 4
                    anchors.left: parent.left
                    onClicked: {
                        modelSeriesList.onCheckAllBoxes(true)
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 100
                color: "transparent"

                Button {
                    id: uncheckAll
                    text: "Uncheck all"
                    anchors.fill: parent
                    anchors.rightMargin: 4
                    anchors.left: parent.left
                    onClicked: {
                        modelSeriesList.onCheckAllBoxes(false)
                    }
                }
            }
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            focus: true

            highlight: Rectangle {
                color: "lightsteelblue";
                radius: 3
            }

            header: Component {
                id: organHeader
                Item{
                    width: listView.width
                    height: 30
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10

                    Row {
                        spacing: 5
                        Layout.fillWidth: true
                        Text { text: "V"; width: 15 }
                        Text { text: "Organ name"; width: 120; font.bold: true }
                        Text { text: "StructureType"; width: 80; font.bold: true }
                    }
                }
            }

            delegate: Component {
                id: organDelegate
                Item{
                    width: listView.width
                    height: 30
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10

                    Row {
                        spacing: 5
                        Layout.fillWidth: true
                        CheckBox { checked: organVisibility; width: 15 }
                        Text { text: organName; width: 120 }
                        Text { text: structureType; width: 80 }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            listView.currentIndex = index
                        }
                    }
                }
            }

            model: OrganListModel {
                id: organListModel
            }

            onCurrentItemChanged: console.log("[OSLM_LOG] " + listView.currentIndex + ' selected')
        }
    }
}
