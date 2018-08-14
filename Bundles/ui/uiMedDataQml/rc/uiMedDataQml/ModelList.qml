import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Material 2.0
import uiMedDataQml 1.0

Item {
    id: modelList

    signal serviceCreated(var srv)

    Component.onCompleted: {
            serviceCreated(modelSeriesList)
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            ListView {
                id: modelTableView
                anchors.fill: parent
                delegate: organDelegate
                highlight: Rectangle {
                    Layout.fillWidth: true
                    color: "lightsteelblue";
                    radius: 3
                }
                focus: true

                Component {
                        id: organDelegate
                        Row {
                            spacing: 10
                            Layout.fillWidth: true
                            CheckBox { checked: organVisibility }
                            Text { text: organName }
                            Text { text: structureType }
                        }
                    }

                model: ListModel {
                    id: sourceModel

                    ListElement {
                        organVisibility: true
                        organName: "skin"
                        structureType: "Skin"
                    }
                    ListElement {
                        organVisibility: false
                        organName: "liver"
                        structureType: "Liver"
                    }
                    ListElement {
                        organVisibility: true
                        organName: "Bones"
                        structureType: "Bones"
                    }
                    ListElement {
                        organVisibility: true
                        organName: "Tumor"
                        structureType: "Tumor"
                    }
                    ListElement {
                        organVisibility: false
                        organName: "Left Kidney"
                        structureType: "Left Kidney"
                    }
                    ListElement {
                        organVisibility: false
                        organName: "Left Lung"
                        structureType: "Left Lung"
                    }
                    ListElement {
                        organVisibility: false
                        organName: "Right Kidney"
                        structureType: "Right Kidney"
                    }
                    ListElement {
                        organVisibility: false
                        organName: "Right Lung"
                        structureType: "Right Lung"
                    }
                }

                SModelSeriesList {
                    id: modelSeriesList

                    onAddOrgan: {
                        console.log(str)
                    }
                }

            }
        }
    }
}
