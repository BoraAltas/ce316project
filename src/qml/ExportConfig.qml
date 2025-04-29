import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: exportConfigDialog
    title: "Export Configuration"
    width: 400
    height: 300
    modal: true
    anchors.centerIn: parent

    property var configModel: [] // List of available configs
    property string selectedConfigName: ""

    signal exportRequested(string configName, string exportFileName)

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        Label {
            text: "Select Configuration:"
            font.bold: true
        }

        ComboBox {
            id: configComboBox
            model: configModel.map(c => c.name)
            Layout.fillWidth: true
            onCurrentTextChanged: selectedConfigName = currentText
        }

        Label {
            text: "Export File Name:"
            font.bold: true
        }

        TextField {
            id: fileNameField
            placeholderText: "e.g. config.json"
            Layout.fillWidth: true
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 20

            Button {
                text: "Export"
                Layout.preferredWidth: 80
                enabled: selectedConfigName !== "" && fileNameField.text.length > 0
                onClicked: {
                    exportConfigDialog.exportRequested(selectedConfigName, fileNameField.text)
                    exportConfigDialog.close()
                }
            }
            Button {
                text: "Cancel"
                Layout.preferredWidth: 80
                onClicked: exportConfigDialog.close()
            }
        }
    }
}
