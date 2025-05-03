import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: editConfigDialog
    title: "Edit Existing Configuration"
    width: 400
    height: 350
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

        TextField {
            id: searchField
            placeholderText: "Search configurations..."
            Layout.fillWidth: true
            onTextChanged: filterText = text
        }

        ListView {
            id: listView
            visible: searchField.activeFocus || searchField.text.length > 0
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            clip: true

            model: configModel.filter(c => c.name.toLowerCase().includes(filterText.toLowerCase()))

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
                        searchField.text = modelData.name
                        searchField.focus = false
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: modelData.name
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

            Label { text: "Options for compiler/interpreter:" }
            TextField {
                id: compileParamsField
                placeholderText: "Options for compiler/interpreter"
                Layout.fillWidth: true
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 20

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
            Button {
                Layout.preferredWidth: 80
                text: "Cancel"
                onClicked: editConfigDialog.close()
            }
        }
    }
}
