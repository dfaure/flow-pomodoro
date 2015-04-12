import QtQuick 2.2

Item {
    id: root

    implicitHeight: childrenRect.height
    anchors {
        left: parent ? parent.left : undefined
        leftMargin: 5 * _controller.dpiFactor
        right: parent ? parent.right : undefined
        rightMargin: 5 * _controller.dpiFactor
    }

    FontAwesomeIcon {
        id: addInstanceIcon
        toolTip: qsTr("Add new instance")
        text: "\uf067"
        color: "black"
        size: _style.addTagIconSize
        anchors.verticalCenter: undefined
        onClicked: {
            _plugin.createBackend()
        }
    }

    Column {
        id: column
        spacing: 3 * _controller.dpiFactor
        anchors {
            top: addInstanceIcon.bottom
            topMargin: 10 * _controller.dpiFactor
            left: parent.left
            right: parent.right
        }

        Repeater {
            model: _plugin.storageBackendModel

            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                implicitHeight: childrenRect.height

                Loader {
                    anchors.left: parent.left
                    anchors.leftMargin: 5 * _controller.dpiFactor
                    anchors.right: parent.right
                    source: _plugin.instanceConfigComponentUrl
                }

                FlowCheckBox {
                    id: checkBox
                    anchors.right: removeIcon.left
                    anchors.rightMargin: 3 * _controller.dpiFactor
                    checked: backendInstance.enabled
                    onCheckedChanged: {
                        backendInstance.enabled = !backendInstance.enabled
                    }
                }

                FontAwesomeIcon {
                    id: removeIcon
                    toolTip: qsTr("Remove instance")
                    text: "\uf00d"
                    anchors.right: parent.right
                    anchors.rightMargin: 5 * _controller.dpiFactor
                    size: 21
                    anchors.verticalCenter: undefined
                    color: "black"
                    onClicked: {
                        _plugin.removeBackend(backendInstance)
                    }
                }
            }
        }
    }
}
