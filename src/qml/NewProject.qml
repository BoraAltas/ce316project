import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs




Dialog {
    id: newProjectDialog
    title: "Create New Project"
    modal: true
    anchors.centerIn: parent
    visible: false
    width: 400
    height: 250


    signal projectCreated(string projectName, string projectPath)

    property string selectedPath: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 30
        anchors.margins: 40

        GridLayout {
            id: formLayout
            columns: 2
            columnSpacing: 30
            rowSpacing: 40
            Layout.fillWidth: true

            Label {
                text: "Project Name:"
            }
            TextField {
                id: nameInput
            }

            Label {
                text: "Project Path:"
            }
            RowLayout {
                Layout.preferredWidth: 200
                spacing: 10

                TextField {
                    id: pathInput
                    Layout.preferredWidth: 150
                    text: newProjectDialog.selectedPath
                    onTextChanged: newProjectDialog.selectedPath = text
                    readOnly: true
                    placeholderText: "Select a folder..."
                }

                Rectangle {
                    width: 30; height: 30; radius: 4
                    color: "transparent"
                    border.color: "#cccccc"; border.width: 1

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/src/qml/images/fileimage.png"
                        width: 20
                        height: 20
                        fillMode: Image.PreserveAspectFit
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: folderDialog.open()

                        onEntered: parent.color = "#eeeeee"
                        onExited: parent.color = "transparent"
                    }
                }
            }
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignRight
            Button {
                text: "Cancel"
                onClicked: newProjectDialog.close()
            }
            Button {
                text: "Create"
                onClicked: {
                    if (nameInput.text.trim() !== "" && selectedPath.trim() !== "") {
                        projectCreated(nameInput.text.trim(), selectedPath.trim())
                        newProjectDialog.close()
                    } else {
                        console.log("Please fill in both fields.")
                    }
                }
            }

        }
    }
    FolderDialog {
        id: folderDialog
        title: "Select Project Folder"
        onAccepted: {
            newProjectDialog.selectedPath = folderDialog.selectedFolder
            pathInput.text = folderDialog.selectedFolder
        }
    }

}

