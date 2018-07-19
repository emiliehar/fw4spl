import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import tuto08.AppManager 1.0
import fwVTKQml 1.0
import uiImageQml 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true

    onClosing: appManager.uninitialize();

    Component.onCompleted: appManager.initialize()

    AppManager {
        id: appManager
        frameBuffer: scene3D

        onImageLoaded: {
            sliceEditor.enabled=true
            sliceSelector.enabled=true
            displayScanButton.enabled = true
        }
    }

    MenuBar {
        Menu {
            title: "File"

            MenuItem {
                text: "Open image"
                shortcut: "Ctrl+I"
                onTriggered: appManager.onOpenImage()
            }
            MenuItem {
                text: "Open meshes"
                shortcut: "Ctrl+M"
                onTriggered: appManager.onOpenModel()
            }
            MenuItem {
                text: "Open texture"
                shortcut: "Ctrl+T"
                onTriggered: appManager.onOpenTexture()
            }
            MenuItem {
                text: "Quit"
                shortcut: "Ctrl+Q"
            }
        }
    }

    ColumnLayout {
        spacing: 2
        anchors.fill: parent
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#006699"

            FrameBuffer {
                id: scene3D
                anchors.fill: parent

                onReady: appManager.createVtkScene()

                onWidthChanged: initialize()

                onHeightChanged: initialize()
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.minimumWidth: parent.width
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
                    visible: true
                    enabled: false

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
                    visible: true
                    enabled: false
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
                visible: true
                enabled: false
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

