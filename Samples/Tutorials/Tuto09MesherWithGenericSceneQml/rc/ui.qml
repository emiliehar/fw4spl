import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import tuto09 1.0
import fwVTKQml 1.0
import uiImageQml 1.0
import uiMedDataQml 1.0
import uiReconstructionQml 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true

    title: qsTr("Tuto09MesherWithGenericScene 0.1")
    onClosing: appManager.uninitialize();

    Component.onCompleted: appManager.initialize()

    AppManager {
        id: appManager
        // @disable-check M16
        frameBuffer: scene3D

        // @disable-check M16
        onImageLoaded: {
            sliceEditor.visible=true
            sliceSelector.visible=true
            displayScanButton.visible = true
            saveMesh.enabled = true
            actionMesh50.enabled = true
            actionMesh80.enabled = true
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            Action {
                text: qsTr("&Open image")
                shortcut: "Ctrl+O"
                onTriggered: appManager.onOpenImage()
            }
            Action {
                id: saveMesh
                text: qsTr("&Save mesh")
                shortcut: "Ctrl+S"
                enabled: false
                onTriggered: appManager.onSaveMesh()
            }
            Action {
                text: qsTr("&Quit")
                shortcut: "Ctrl+Q"
                onTriggered: root.close()
            }
        }
        Menu {
            title: "Mesher"

            Action {
                id: actionMesh50
                text: qsTr("Create Mesh 50")
                enabled: false
                onTriggered: appManager.applyMesher(50)
            }
            Action {
                id: actionMesh80
                text: qsTr("Create Mesh 80")
                enabled: false
                onTriggered: appManager.applyMesher(80)
            }
        }
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            spacing: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#006699"

                FrameBuffer {
                    id: scene3D
                    anchors.fill: parent

                    // @disable-check M16
                    onReady: appManager.createVtkScene()

                    onWidthChanged: initialize()

                    onHeightChanged: initialize()
                }
            }

            RowLayout {
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                spacing: 4

                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 150

                    color: "transparent"
                    ComboBox {
                        id: sliceEditor
                        anchors.fill: parent
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 4
                        visible: false

                        model: ["One slice", "Three slice"]

                        currentIndex: 1
                        onActivated: {
                            appManager.onUpdateSliceMode((index == 0) ? 1 : 3)
                        }
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 75

                    color: "transparent"
                    Button {
                        id: displayScanButton
                        visible: false
                        checkable: true
                        checked: true
                        anchors.fill: parent
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        text: "Scan"
                        onCheckedChanged: {
                            sliceEditor.enabled = checked
                            appManager.onShowScan(checked)
                        }
                    }
                }

                SliceSelector {
                    id: sliceSelector
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: false
                    from: 0
                    to: 80

                    onServiceCreated: {
                        appManager.onServiceCreated(srv)
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 70
                    color: "transparent"

                    Button {
                        id: snapButton
                        text: "Snap"
                        anchors.fill: parent
                        anchors.rightMargin: 4
                        anchors.left: parent.left
                        onClicked: {
                            snapFileDialog.open()
                        }
                    }
                }
            }
        }

        ScrollView {

            Layout.fillHeight: true
            Layout.minimumWidth: 305
            spacing: 10

            ColumnLayout {

                Rectangle {
                    Layout.preferredHeight: 300
                    Layout.preferredWidth: 300

                    color: "transparent"

                    ModelList {
                        id: modelList
                        anchors.fill: parent

                        onServiceCreated: {
                            appManager.onServiceCreated(srv)
                        }
                    }
                }

                Rectangle {
                    Layout.preferredHeight: 60
                    Layout.preferredWidth: 300

                    color: "transparent"

                    OrganMaterialEditor {
                        id: organMaterialEditor
                        anchors.fill: parent

                        onServiceCreated: {
                            appManager.onServiceCreated(srv)
                        }
                    }
                }

                Rectangle {
                    Layout.preferredHeight: 400
                    Layout.preferredWidth: 300

                    color: "transparent"

                    RepresentationEditor {
                        id: representationEditor
                        anchors.fill: parent

                        onServiceCreated: {
                            appManager.onServiceCreated(srv)
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: snapFileDialog
        title: "Save snapshot as"
        folder: shortcuts.home
        selectExisting: false
        nameFilters: [
            "Image file (*.jpg *.jpeg *.bmp *.png *.tiff)",
            "jpeg (*.jpg *.jpeg)",
            "bmp (*.bmp)",
            "png (*.png)",
            "tiff (*.tiff)",
            "all (*.*)"
        ]
        onAccepted: {
            appManager.onSnap(snapFileDialog.fileUrl)
        }
    }

}

