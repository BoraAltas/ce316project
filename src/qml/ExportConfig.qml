import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

import iae

Dialog {
    id: exportConfigDialog
    title: "Export Configuration"
    width: 400
    height: 300
    modal: true
    anchors.centerIn: parent

    property var configModel: [] // List of available configs
    property string selectedConfigName: ""

    signal exportRequested(string configName, string exportFilePath)

    FolderDialog {
        id: saveFolderDialog
        title: "Select Directory to Save Configuration"

        onAccepted: {
            //exportRequested(selectedConfigName, saveFolderDialog.folder + "/" + selectedConfigName + ".json")
            let sanitizedPath = String(saveFolderDialog.currentFolder).replace("file://", "");
            IAE.exportConfig(selectedConfigName, sanitizedPath + "/" + selectedConfigName + ".json")
            exportConfigDialog.close()
        }
    }

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

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 20
            Button {
                text: "Cancel"
                Layout.preferredWidth: 80
                onClicked: exportConfigDialog.close()
            }
            Button {
                text: "Export"
                Layout.preferredWidth: 80
                enabled: selectedConfigName !== ""
                onClicked: {
                    saveFolderDialog.open()
                }
            }

        }
    }
}
