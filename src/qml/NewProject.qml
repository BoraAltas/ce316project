import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

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
        spacing: 10
        anchors.margins: 50

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
            TextField {
                id: pathInput
                text: newProjectDialog.selectedPath
                onTextChanged: newProjectDialog.selectedPath = text
            }
        }
        Item {
            Layout.fillHeight: true
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
                    if (nameInput.text !== "" && selectedPath !== "") {
                        projectCreated(nameInput.text, selectedPath)
                        newProjectDialog.close()
                    } else {
                        console.log("Please fill in both fields.")
                    }
                }
            }
        }
    }
}
