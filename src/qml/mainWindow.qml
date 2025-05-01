import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

//import iae

ApplicationWindow {
    visible: true
    width: 1000
    height: 700
    title: "AssignCheck"
    color: "#f5f5f5"

    CreateNewConfiguration {
        id: createConfigDialog
        onConfigCreated: (configName, language, compilerPath) => {
            console.log("New config created:");
            console.log("Name:", configName);
            console.log("Language:", language);
            console.log("Compiler:", compilerPath);

        }
        visible: false

    }
    NewProject{
        id:newProjectDialog
        visible: false
        onProjectCreated: (name, path) => {
            console.log("New Project Created:")
            console.log("Name:", name)
            console.log("Path:", path)
        }

    }
    EditExistingConfig{
        id: editConfigDialog
        onConfigUpdated: (oldName, newName, language, compilerPath) => {
            console.log("Config updated:");
            console.log("Old Name:", oldName);
            console.log("New Name:", newName);
            console.log("Language:", language);
            console.log("Compiler Path:", compilerPath);
        }
        visible: false
    }
    RemoveConfig{
        id: removeConfigDialog
        onConfigRemoved: (configName) => {
            console.log("Removed config:", configName)
            //  remove from backend
        }
        visible: false
    }
    ImportConfig{
        id: importConfigDialog
        onImportRequested: (filePath) => {
            console.log("Import requested with file path:", filePath)
            // call backend later
        }
    }
    ExportConfig {
        id: exportConfigDialog
        onExportRequested: (configName, fileName) => {
            console.log("Export requested:")
            console.log("Config:", configName)
            console.log("Save As:", fileName)
            // call backend save logic
        }
    }


    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "New Project"
                onTriggered: {
                    newProjectDialog.visible = true
                }
            }
        }

        Menu {
            title: "Configuration"
            MenuItem {
                text: "Create New"
                onTriggered: {
                    createConfigDialog.visible = true
                }
            }
            MenuItem { text: "Edit Existing"
                onTriggered: {
                    editConfigDialog.configModel = [
                        { name: "C Config", language: "C", compilerPath: "C:/MinGW/bin/gcc.exe" },
                        { name: "Python Conf", language: "Python", compilerPath: "python" }
                        // fill after
                    ]
                    editConfigDialog.visible = true
                }
            }

            MenuItem {
                text: "Remove"
                onTriggered: {
                    removeConfigDialog.configModel = [
                        { name: "C Config", language: "C", compilerPath: "gcc" },
                        { name: "Python Conf", language: "Python", compilerPath: "python" }
                    ]
                    removeConfigDialog.filteredConfigs = removeConfigDialog.allConfigs
                    removeConfigDialog.visible = true
                }
            }
            MenuItem {
                onTriggered: importConfigDialog.visible = true
                contentItem: Row {
                    spacing: 6
                    Text {
                        text: "Import"
                    }
                    Image {
                        source: "qrc:/src/qml/images/import.png"
                        width: 20; height: 16
                        fillMode: Image.PreserveAspectFit
                    }

                }
            }
            MenuItem {
                onTriggered: {
                    exportConfigDialog.configModel = [
                        { name: "C Config", language: "C", compilerPath: "C:/MinGW/bin/gcc.exe" },
                        { name: "Python Conf", language: "Python", compilerPath: "python" }
                        // fill it properly from your real config list
                    ]
                    exportConfigDialog.visible = true
                }
                contentItem: Row {
                    spacing: 6
                    Text {
                        text: "Export"
                    }
                    Image {
                        source: "qrc:/src/qml/images/export.png"
                        width: 20; height: 16
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }

        Menu {
            title: "Help"
           MenuItem {
               onTriggered: {
                   helpDialog.open()
               }
               contentItem: Row {
                   spacing: 6
                   Text {
                       text: "How to use"
                   }
                   Image {
                       source: "qrc:/src/qml/images/help.png"
                       width: 20; height: 20
                       fillMode: Image.PreserveAspectFit
                   }

               }
            }
        }
    }



    Rectangle {
        anchors.fill: parent
        anchors.margins: 20
        color: "transparent"

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            GroupBox {
                title: "Project Information"
                Layout.fillWidth: true

                GridLayout {
                    columns: 2
                    rowSpacing: 10
                    columnSpacing: 20
                    anchors.fill: parent

                    Label { text: "Project Name:" }
                    ComboBox {
                        id: projectNameComboBox
                        Layout.preferredWidth: 200
                        Layout.fillWidth: false

                        model: [
                            "Project A",
                            "Project B",
                            "Project C",
                        ]
                    }

                    Label { text: "Configuration:" }
                    ComboBox {
                        Layout.preferredWidth: 200
                        model: ["Select", "C", "C++", "Java", "Python"]
                    }

                    Label { text: "Student ZIP Directory:" }
                    TextField {
                        Layout.preferredWidth: 200
                        placeholderText: "Choose ZIP directory..." }
                }
            }

            GroupBox {
                title: "Assignment Test Parameters"
                Layout.fillWidth: true

                GridLayout {
                    columns: 2
                    rowSpacing: 10
                    columnSpacing: 20
                    anchors.fill: parent

                    Label { text: "Program Arguments:" }
                    TextField { placeholderText: "e.g. input.txt output.txt" }

                    Label { text: "Expected Output File:" }
                    TextField { placeholderText: "e.g. expected.txt" }
                }
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter

                Button { text: "Run All" }
                Button { text: "Clear Results" }
                Button { text: "Open File"

                onClicked: {
                    zipHandler.openFileDialog()
                    }
                }
            }
            /*Label {
                text: zipHandler.selectedFile
                anchors.top: parent.top
                anchors.topMargin: 50
            }*/

            GroupBox {
                title: "Projects"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 1
                        Rectangle {
                            color: "#dcdcdc"
                            Layout.preferredWidth: 150
                            height: 40
                            border.color: "black"
                            Text { anchors.centerIn: parent; text: "Project ID" }
                        }
                        Rectangle {
                            color: "#dcdcdc"
                            Layout.preferredWidth: 788
                            height: 40
                            border.color: "black"
                            Text { anchors.centerIn: parent; text: "Project Name"; }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: ListModel {
                            ListElement { projectID: "123456"; projectName: "Project A" }
                            ListElement { projectID: "234567"; projectName: "Project B" }
                            ListElement { projectID: "345678"; projectName: "Project C" }
                        }

                        delegate: RowLayout {
                            spacing: 1
                            Rectangle {
                                Layout.preferredWidth: 150
                                height: 30
                                color: "white"
                                border.color: "#cccccc"
                                Text { anchors.centerIn: parent; text: projectID }
                            }
                            Rectangle {
                                Layout.preferredWidth: 788
                                height: 30
                                color: "white"
                                border.color: "#cccccc"
                                Text { anchors.centerIn: parent; text: projectName }
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: helpDialog
        title: "How to Use AssignCheck"
        anchors.centerIn: parent
        modal: true
        width: 500
        height: 400

        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 20

            ScrollView {
                id: scrollArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                contentWidth: scrollArea.width
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                Column {
                    width: scrollArea.width
                    spacing: 10

                    Text {
                        text: "Welcome to AssignCheck!\n\n"
                            + "1. Create a New Project from the 'File' menu.\n"
                            + "2. Set a Configuration (like C, C++, Java, or Python).\n"
                            + "3. Select the Student ZIP Directory.\n"
                            + "4. Define Program Arguments and Expected Output.\n"
                            + "5. Press 'Run All' to automatically compile and test assignments.\n"
                            + "6. You can Create, Edit, Remove, Import or Export configurations under the 'Configuration' menu.\n\n"
                            + "Good luck!\n\n"

                        wrapMode: Text.Wrap
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
            Button {
                text: "Close"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: 80
                Layout.preferredHeight: 30
                onClicked: helpDialog.close()
            }
        }
    }
}