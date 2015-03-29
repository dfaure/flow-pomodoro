import QtQuick 2.2

Plugins {
    id: root
    anchors.fill: parent
    smallText: qsTr("Plugins disable certain distractions when a task is running. Distractions are re-enabled when the task stops.") +
          "\n" + (_distractionsPluginModel.count > 0 ? qsTr("The following plugins were found:") : "")

    noPluginsText: qsTr("no distractions plugins were found")
    pluginModel: _distractionsPluginModel
}
