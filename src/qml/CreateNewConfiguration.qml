import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs


import iae 1.0

Dialog {
    id: createConfigDialog
    title: "Create New Configuration"
    width: 550
    height: 350
    modal: true
    anchors.centerIn: parent

    signal configCreated(string configName, string language, string compilerPath)

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 40

        GridLayout {
            id: formLayout
            columns: 2
            columnSpacing: 25
            rowSpacing: 30
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
                model: [ "C", "C++", "Java", "Python"]
                Layout.fillWidth: true
                currentIndex: -1
                onActivated: if (currentIndex === -1) currentIndex = -1
            }

            Label { text: "Compile/Interpret Parameters:" }
            RowLayout {
                Layout.preferredWidth: 250
                spacing: 10
                TextField {
                    id: compilerParamsField
                    Layout.preferredWidth: 150
                    placeholderText: "e.g. -o output"
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
                onClicked: {
                    configNameField.text = "";
                    compilerParamsField.text = "";
                    languageComboBox.currentIndex = -1
                    createConfigDialog.close()
                }
            }
            Button {
                text: "Create"
                Layout.preferredWidth: 80
                enabled: languageComboBox.currentIndex >= 0
                onClicked: {
                    IAE.saveConfig(
                        configNameField.text,
                        languageComboBox.currentText,
                        compilerParamsField.text
                    )
                    configNameField.text = "";
                    compilerParamsField.text = "";
                    languageComboBox.currentIndex = -1
                    createConfigDialog.close()
                }
            }

        }
    }
}
