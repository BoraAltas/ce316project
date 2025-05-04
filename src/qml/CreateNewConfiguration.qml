import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs



Dialog {
    id: createConfigDialog
    title: "Create New Configuration"
    width: 450
    height: 350
    modal: true
    anchors.centerIn: parent

    signal configCreated(string configName, string language, string compilerPath)

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

            Label { text: "Configuration Name:" }
            TextField {
                id: configNameField
                placeholderText: "e.g. C Assignment Config"
                Layout.fillWidth: true
            }

            Label { text: "Language:" }
            ComboBox {
                id: languageComboBox
                model: ["Select", "C", "C++", "Java", "Python"]
                Layout.fillWidth: true
            }

            Label { text: "Compiler Path:" }
            RowLayout {
                Layout.preferredWidth: 250
                spacing: 10
                TextField {
                    id: compilerPathField
                    Layout.preferredWidth: 150
                    placeholderText: "e.g. C:/MinGW/bin/gcc.exe"
                    readOnly: true
                }

                Rectangle {
                    width: 30; height: 30; radius: 4
                    color: "transparent"
                    border.color: "#cccccc"; border.width: 1

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/src/qml/images/fileimage.png"
                        width: 20; height: 20
                        fillMode: Image.PreserveAspectFit
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: compilerFileDialog.open()

                        onEntered: parent.color = "#eeeeee"
                        onExited: parent.color = "transparent"
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            id: buttonRow
            Layout.alignment: Qt.AlignRight
            spacing: 20

            Button {
                text: "Cancel"
                Layout.preferredWidth: 80
                onClicked: createConfigDialog.close()
            }
            Button {
                text: "Create"
                Layout.preferredWidth: 80
                onClicked: {
                    createConfigDialog.configCreated(
                        configNameField.text,
                        languageComboBox.currentText,
                        compilerPathField.text
                    )
                    createConfigDialog.close()
                }
            }

        }
    }
    FileDialog {
        id: compilerFileDialog
        title: "Select Compiler Executable"
        nameFilters: ["Executable Files (*.exe)", "All Files (*)"]
        onAccepted: {
            compilerPathField.text = compilerFileDialog.selectedFile
        }
    }

}
