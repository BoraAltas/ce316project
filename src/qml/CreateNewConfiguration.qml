import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

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
            TextField {
                id: compilerPathField
                placeholderText: "e.g. C:/MinGW/bin/gcc.exe"
                Layout.fillWidth: true
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
                onClicked: createConfigDialog.close()
            }

            Button {
                text: "Create"
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
}
