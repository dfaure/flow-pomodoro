import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0


import "qrc:/qml/"

Item {
    id: root
    implicitHeight: childrenRect.height

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose text file")
        onAccepted: {
            console.log("Settings to " + fileUrl)
            backendInstance.filename = fileUrl
        }
    }

    Row {
        spacing: 7 * _controller.dpiFactor

        FontAwesomeIcon {
            text: "\uf07c"
            color: "black"
            font.pixelSize: 17 * _controller.dpiFactor
            onClicked: {
                fileDialog.open()
            }
        }

        Text {
            text: backendInstance.shortname ? backendInstance.shortname : qsTr("no file selected")
        }
    }
}
