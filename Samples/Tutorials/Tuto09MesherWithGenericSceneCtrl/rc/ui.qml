import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import fw4spl.tuto.Tuto09Manager 1.0
import fw4spl.fwVTKQml 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true

    Tuto09Manager {
        id: tuto09
        frameBuffer: scene3D

        onImageLoaded: {
            sliceEditor.visible=true
            displayScanButton.visible = true
            saveMesh.enabled = true
            actionMesh50.enabled = true
            actionMesh80.enabled = true
        }
    }

    MenuBar {
        Menu {
            title: "File"

            MenuItem {
                text: "Open image"
                shortcut: "Ctrl+O"
                onTriggered: tuto09.onOpenImage()
            }
            MenuItem {
                id: saveMesh
                text: "Save mesh"
                shortcut: "Ctrl+S"
                enabled: false
                onTriggered: tuto09.onSaveMesh()
            }
            MenuItem {
                text: "Quit"
                shortcut: "Ctrl+Q"
            }
        }
        Menu {
            title: "Mesher"

            MenuItem {
                id: actionMesh50
                text: "Create Mesh 50"
                enabled: false
                onTriggered: tuto09.applyMesher(50)
            }
            MenuItem {
                id: actionMesh80
                text: "Create Mesh 80"
                enabled: false
                onTriggered: tuto09.applyMesher(80)
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

                onReady: tuto09.initialize()

                onWidthChanged: initialize()

                onHeightChanged: initialize()
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.minimumWidth: parent.width
            Layout.maximumHeight: 50
            spacing: 4

            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 150

                color: "transparent"
                ComboBox {
                    id: sliceEditor
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 4
                    visible: false


                    model: ["One slice", "Three slice"]

                    currentIndex: 1
                    onActivated: {
                        imageAdaptor.updateSliceMode((index == 0) ? 1 : 3)
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
                    checked: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    text: "Scan"
                    onCheckedChanged: {
                        sliceSelector.enabled = !checked && imageAdaptor.image
                    }
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
                    onClicked: {
                        snapshotEditor.onSnapButton()
                    }
                }
            }
        }
    }
}

