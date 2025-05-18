import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

import iae

ApplicationWindow {
    visible: true
    width: 1000
    height: 700
    title: "AssignCheck"
    color: "#f5f5f5"

    Component.onCompleted: {
            IAE.Initialize()
    }

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
    RemoveConfig {
        id: removeConfigDialog
        onConfigRemoved: (configName) => {
            console.log("Removed config:", configName)
            IAE.removeConfig(configName)
            removeConfigDialog.configModel = IAE.getConfigsAsVariantList()
            configModel.clear()
            IAE.getConfigsAsVariantList().forEach(config => configModel.append({ name: config.name }))
        }
        ////////////
      //  visible: false
        ////////////
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
        configModel: IAE.getConfigsAsVariantList()
        onExportRequested: (configName, filePath) => {
            console.log("Exporting", configName, "to", filePath)
            IAE.exportConfig(configName, filePath)
        }
    }


    menuBar: MenuBar {


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
                    editConfigDialog.configModel = IAE.getConfigsAsVariantList()
                    editConfigDialog.visible = true
                }
            }

            MenuItem {
                text: "Remove"
                onTriggered: {
                    removeConfigDialog.configModel = IAE.getConfigsAsVariantList()
                    removeConfigDialog.filteredConfigs = removeConfigDialog.configModel
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
                    TextField {
                        id: projectNameField
                        Layout.preferredWidth: 200
                        Layout.fillWidth: false
                        placeholderText: "Enter Project Name"
                    }

                    Label { text: "Configuration:" }
                    ComboBox {
                        id: configComboBox
                        Layout.preferredWidth: 200
                        model: configModel

                        ListModel {
                            id: configModel
                            Component.onCompleted: {
                                configModel.clear()
                                IAE.getConfigsAsVariantList().forEach(config => append({ name: config.name }));
                            }
                        }

                        Connections {
                            target: IAE
                            function onConfigsChanged() {
                                configModel.clear();
                                IAE.getConfigsAsVariantList().forEach(config => configModel.append({ name: config.name }));
                            }
                        }

                        delegate: ItemDelegate {
                            text: model.name
                        }
                    }

                    Label { text: "Student ZIP Directory:"}

                        RowLayout {
                            Layout.preferredWidth: 310
                            spacing: 10

                            TextField {
                                id: zipFilePathField
                                Layout.preferredWidth: 280
                                placeholderText: "Choose project file..."
                                text: fileDialogHelper.selectedFile
                                readOnly: true
                            }
                            Rectangle {
                                width: 25
                                height: 20
                                radius: 4
                                color: "transparent"
                                border.color: "#cccccc"
                                border.width: 1

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
                                    onClicked: fileDialogHelper.openFileDialog()

                                    onEntered: parent.color = "#eeeeee"
                                    onExited: parent.color = "transparent"
                                }
                            }
                        }

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


                    RowLayout {
                        Layout.preferredWidth: 310
                        spacing: 10

                        TextField {
                            id: programArgsField
                            Layout.preferredWidth: 280
                            placeholderText: "e.g. input.txt output.txt"
                        }

                        Rectangle {
                            width: 25
                            height: 20
                            radius: 4
                            color: "transparent"
                            border.color: "#cccccc"
                            border.width: 1

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
                                onClicked: {
                                    onClicked: programArgDialog.open()
                                }

                                onEntered: parent.color = "#eeeeee"
                                onExited: parent.color = "transparent"
                            }
                        }
                    }

                    Label { text: "Expected Output File:" }


                    RowLayout {
                        Layout.preferredWidth: 310
                        spacing: 10

                        TextField {
                            id: expectedOutputField
                            Layout.preferredWidth: 280
                            placeholderText: "e.g. expected.txt"
                        }

                        Rectangle {
                            width: 25
                            height: 20
                            radius: 4
                            color: "transparent"
                            border.color: "#cccccc"
                            border.width: 1

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
                                onClicked: {
                                    onClicked: expectedOutputDialog.open()
                                }

                                onEntered: parent.color = "#eeeeee"
                                onExited: parent.color = "transparent"
                            }
                        }
                    }
                }
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter

                Button { text: "Run All"
                    onClicked: {
                        IAE.isEmpty()
                        zipHandler.setProjectName(projectNameField.text)
                        zipHandler.unzipFile(zipDirectoryField.text)
                        IAE.createProject(
                            projectNameField.text,
                            configComboBox.currentText,
                            programArgsField.text,
                            expectedOutputField.text
                        )
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
                            Layout.fillWidth: true
                            height: 40
                            border.color: "black"
                            Text { anchors.centerIn: parent; text: "Project Name"; }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: IAE.projects

                        delegate: RowLayout {
                            spacing: 1
                            Rectangle {
                                Layout.preferredWidth: 938
                                height: 30
                                color: "white"
                                border.color: "#cccccc"
                                Text { anchors.centerIn: parent; text: modelData.projectName }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        studentDialog.projectName = modelData.projectName
                                        studentDialog.studentModel = modelData.students
                                        studentDialog.visible = true
                                    }
                                }
                            }
                        }
                    }

                    Dialog {
                        id: studentDialog
                        title: "Students in Project"
                        width: 800
                        height: 400
                        modal: true
                        visible: false

                        property string projectName: ""
                        property var studentModel: null

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 10

                            Label {
                                text: "Project: " + studentDialog.projectName
                                font.bold: true
                                font.pixelSize: 16
                            }

                            ScrollView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                ListView {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    model: studentDialog.studentModel

                                    delegate: RowLayout {
                                        spacing: 10
                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignLeft

                                        Rectangle {
                                            Layout.preferredWidth: 200
                                            height: 40
                                            color: "white"
                                            border.color: "#cccccc"
                                            Text { anchors.centerIn: parent; text: modelData.studentID }
                                        }
                                        Rectangle {
                                            Layout.preferredWidth: 200
                                            height: 40
                                            color: "white"
                                            border.color: "#cccccc"
                                            Text { anchors.centerIn: parent; text: modelData.result }
                                        }
                                        Rectangle {
                                            Layout.preferredWidth: 200
                                            height: 40
                                            color: "white"
                                            border.color: "#cccccc"
                                            Text { anchors.centerIn: parent; text: modelData.success ? "Pass" : "Fail" }
                                        }
                                    }
                                }
                            }

                            Button {
                                text: "Close"
                                Layout.alignment: Qt.AlignRight
                                onClicked: studentDialog.visible = false
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
    FileDialog {
        id: programArgDialog
        title: "Select Program Arguments File"
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted: {
            programArgsField.text = selectedFile
        }
    }

    FileDialog {
        id: expectedOutputDialog
        title: "Select Expected Output File"
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted: {
            expectedOutputField.text = selectedFile
        }
    }


}
