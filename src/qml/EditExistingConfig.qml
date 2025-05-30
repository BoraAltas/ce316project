import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs


import iae 1.0

Dialog {
    id: editConfigDialog
    title: "Edit Existing Configuration"
    width: 430
    height: 400
    modal: true
    anchors.centerIn: parent

    property var configModel: []
    property string filterText: ""

    signal configUpdated(string oldConfigName, string newConfigName, string language, string compilerPath)

    property string oldConfigName: ""
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        ScrollView {
            Layout.fillWidth: true
            implicitHeight: 70

            ListView {
                id: listView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                model: configModel

                delegate: Rectangle {
                    width: listView.width
                    height: 20
                    color: ListView.isCurrentItem ? "#a0a0a0" : "#f0f0f0"
                    border.color: "#cccccc"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index
                            configNameField.text = modelData.name
                            languageComboBox.currentIndex = languageComboBox.model.indexOf(modelData.language)
                            compileParamsField.text = modelData.compileParams
                            oldConfigName = modelData.name
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData.name
                    }
                }
            }
        }
        GridLayout {
            columns: 2
            columnSpacing: 20
            rowSpacing: 20
            Layout.fillWidth: true

            Label { text: "Configuration Name:" }
            TextField {
                id: configNameField
                placeholderText: "Config name"
                Layout.fillWidth: true
            }

            Label { text: "Language:" }
            ComboBox {
                id: languageComboBox
                Layout.fillWidth: true
                model: ["C", "C++", "Java", "Python"]
                currentIndex: -1
                onActivated: if (currentIndex === -1) currentIndex = -1
            }

            Label { text: "Compile/Interpret Parameter:" }
            RowLayout{
                Layout.fillWidth: true
                spacing: 10
                TextField {
                    id: compileParamsField
                    placeholderText: "Parameters"
                    Layout.fillWidth: true
                }
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 20

            Button {
                Layout.preferredWidth: 80
                text: "Cancel"
                onClicked: editConfigDialog.close()
            }
            Button {
                text: "Save"
                Layout.preferredWidth: 80
                enabled: languageComboBox.currentIndex >= 0
                onClicked: {
                    if (oldConfigName !== "") {
                        editConfigDialog.configUpdated(
                            oldConfigName,
                            configNameField.text,
                            languageComboBox.currentText,
                            compileParamsField.text
                        )
                        iae.editConfig(
                            configNameField.text,
                            languageComboBox.currentText,
                            compileParamsField.text
                        )
                        editConfigDialog.close()
                    } else {
                        console.log("Please select a configuration first!")
                    }
                }
            }

        }
    }
}
