import QtQuick
import QtQuick.Controls
import QtCore

ApplicationWindow { // Main app window
    id: mainWindow
    visible: true
    title: "Ce316 Project" //TODO: Find a better name

    // save/reload the window size
    width: settings.width
    height: settings.height

    minimumHeight: 450
    minimumWidth: 700

    Settings { //window size and maximized state, other settings can be added here, also saved onClosing()
        id: settings

        property bool maximized
        property alias height: mainWindow.height
        property alias width: mainWindow.width
    }

    Rectangle { // Top bar
        id: topBar

        color: "lightgrey"
        border.color: "black"
        border.width: 2

        anchors.left: parent.left // Size and alignment, parent is mainWindow
        anchors.right: parent.right // because it is created inside that component
        height: 50
        width: parent.width

        Button {
            text: "Configure"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            anchors.leftMargin: 10
            anchors.rightMargin: 10

            onClicked: Qt.quit() //TODO, will be bounded to cpp code using signals or Q_INVOKABLE
        }

        Button {
            text: "Import"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right

            anchors.leftMargin: 10
            anchors.rightMargin: 100

            onClicked:
                Qt.quit() //TODO, will be bounded to cpp code using signals or Q_INVOKABLE
        }

        Button {
            text: "Help"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right

            anchors.leftMargin: 10
            anchors.rightMargin: 10

            onClicked: Qt.quit() //TODO
        }
    }


    Button {
        text: "Open File"

        // Position the button at the top-left corner with margin
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 50
        anchors.topMargin: 50

        z: 10  // Ensure the button is on top of other components

        onClicked: {
            fileDialogHelper.openFileDialog()
        }
    }



    // Label to show the selected file path
    Label {
        text: fileDialogHelper.selectedFile
        anchors.top: parent.top
        anchors.topMargin: 50
    }


    Rectangle { // Rest of the window
        id: mainView

        height: parent.height - topBar.height
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        color: "lightblue"
        border.color: "black"
        border.width: 2

        Rectangle { // temp
            id: contentArea
            anchors.centerIn: parent //center at the parent component

            width: parent.width * 0.9
            height: parent.height * 0.8

            color: "white"
            border.color: "black"
            border.width: 2

            Text {
                id: contentText
                anchors.centerIn: parent
                text: "hoppala"
                font.pointSize: 20
            }
        }
    }


    Component.onCompleted: { // After the creation of the window (MainWindow)
        if (settings.maximized) {
            mainWindow.showMaximized();
        }
    }

    onClosing: { // Closing of the window, saving screen size and maximization setting
        if (mainWindow.visibility === Window.Maximized) {
            settings.maximized = true;
        } else {
            settings.maximized = false;
            settings.height = mainWindow.height;
            settings.width = mainWindow.width;
        }
    }
}