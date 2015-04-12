import QtQuick 2.2

Plugins {
    id: root
    anchors.fill: parent
    smallText: qsTr("Storage plugins allow you to load tasks from other backends.") +
          "\n" + (_storagePluginModel.count > 0 ? qsTr("The following storage plugins were found:") : "")

    noPluginsText: qsTr("no storage plugins were found")
    pluginModel: _storagePluginModel
    showCheckBox: false
}
