import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: removeConfigDialog
    title: "Remove Configuration"
    width: 350
    height: 350
    modal: true
    anchors.centerIn: parent

    property var configModel: []
    property string filterText: ""
    property string selectedConfigName: ""

    property var allConfigs: []
    property var filteredConfigs: allConfigs

    signal configRemoved(string configName)

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        Label {
            text: "Select Configuration to Remove:"
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }
        TextField {
            id: searchField
            placeholderText: "Search configuration..."
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            onTextChanged: filterText = text
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: configModel.filter(c => c.name.toLowerCase().includes(filterText.toLowerCase()))

            delegate: Rectangle {
                width: listView.width
                height: 20
                color: modelData.name === selectedConfigName ? "#ff0000" : "#f0f0f0"
                border.color: "#cccccc"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedConfigName = modelData.name
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: modelData.name
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 20

            Button {
                text: "Remove"
                Layout.preferredWidth: 80
                enabled: selectedConfigName !== ""
                onClicked: {
                    if (selectedConfigName !== "") {
                        removeConfigDialog.configRemoved(selectedConfigName)
                        removeConfigDialog.close()
                    }
                }
            }
            Button {
                text: "Cancel"
                Layout.preferredWidth: 80
                onClicked: removeConfigDialog.close()
            }
        }
    }
}