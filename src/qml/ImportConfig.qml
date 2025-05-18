import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import iae

Dialog {
    id: importConfigDialog
    title: "Import Configuration"
    width: 400
    height: 250
    modal: true
    anchors.centerIn: parent

    signal importRequested(string importFilePath)

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        Label {
            text: "Import File Path:"
            font.bold: true
        }

        TextField {
            id: filePathField
            placeholderText: "e.g. C:/configs/myconfig.json"
            Layout.fillWidth: true
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 20

            Button {
                text: "Cancel"
                Layout.preferredWidth: 80
                onClicked: importConfigDialog.close()
            }
            Button {
                text: "Import"
                Layout.preferredWidth: 80
                enabled: filePathField.text.length > 0
                onClicked: {
                    IAE.importConfig(filePathField.text)
                    importConfigDialog.close()
                }
            }

        }
    }
}
