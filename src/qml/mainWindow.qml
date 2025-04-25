import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

//import iae

ApplicationWindow {
    visible: true
    width: 1000
    height: 700
    title: "CompileX"
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

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "New Project"
                onTriggered: {
                    newProjectDialog.visible = true
                }
            }
            MenuItem { text: "Save Project" }
            MenuItem { text: "Exit" }
        }

        Menu {
            title: "Configuration"
            MenuItem {
                text: "Create New"
                onTriggered: {
                    createConfigDialog.visible = true
                }
            }
            MenuItem { text: "Edit Existing" }
            MenuItem { text: "Remove" }
            MenuItem { text: "Import" }
            MenuItem { text: "Export" }
        }

        Menu {
            title: "Help"
            MenuItem { text: "View Manual" }
            MenuItem { text: "About" }
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
                    TextField { placeholderText: "Enter project name" }

                    Label { text: "Configuration:" }
                    ComboBox {
                        model: ["Select", "C", "C++", "Java", "Python"]
                    }

                    Label { text: "Student ZIP Directory:" }
                    TextField { placeholderText: "Choose ZIP directory..." }
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
            }

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
}