import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: createConfigDialog
    title: "Create New Configuration"
    width: 400
    height: 300
    modal: true  // Makes the dialog modal (blocks interaction with the main window)

    ColumnLayout {
        spacing: 10
        anchors.centerIn: parent

        TextField {
            id: configNameField
            // Text: "Enter Configuration Name"
        }

        ComboBox {
            id: languageComboBox
            model: ["Select", "C", "C++", "Java", "Python"]
            // Text: "Select Language"
        }

        TextField {
            id: compilerPathField
            // Text: "Enter Compiler Path"
        }

        RowLayout {
            spacing: 20

            Button {
                text: "Cancel"
                onClicked: createConfigDialog.close()  // Close dialog
            }

            Button {
                text: "Create"
                onClicked: {
                    // Add logic to save the configuration
                    console.log("Creating config: ", configNameField.text)
                    createConfigDialog.close()  // Close dialog after creating
                }
            }
        }
    }
}
