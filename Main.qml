import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 700
    height: 700
    title: "Reflection Todo"

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        anchors.margins: 20

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            spacing: 10

            TextField {
                id: input
                Layout.fillWidth: true
                placeholderText: "Add new task..."
                onEditingFinished: function() {
                    if (input.text.length > 0) {
                        TaskModel.addTask(input.text)
                        input.text = ""
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: "Add"
                onClicked: {
                    if (input.text.length > 0) {
                        TaskModel.addTask(input.text)
                        input.text = ""
                    }
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            model: ReflectionModel
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                width: parent.width
                height: 80
                radius: 8
                color: Done ? "#d4f8d4" : "#ffffff"
                border.color: "#cccccc"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    CheckBox {
                        checked: Done
                        onToggled: Done = checked
                    }

                    TextField {
                        text: Title
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        onEditingFinished: title = text
                    }
                }
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 20
            Column {
                Text { text: "Reflection Roles:" ; font.pixelSize: 35}
                spacing: 3
                Repeater {
                    model: ReflectionModel.getRoleNames()
                    delegate: Text {
                        text: modelData
                        font.pixelSize: 35
                        font.weight: Font.Bold
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: 2
                Layout.preferredHeight: parent.height
                color: "black"
            }

            Column {
                Text { text: "Traditional Roles:" ; font.pixelSize: 35}
                spacing: 3
                Repeater {
                    model: TraditionalModel.getRoleNames()
                    delegate: Text {
                        text: modelData
                        font.pixelSize: 35
                        font.weight: Font.Bold
                    }
                }
            }
        }



    }
}
