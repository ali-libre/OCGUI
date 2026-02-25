import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Rectangle {
    id: profileDetail
    // Layout.minimumWidth: 280
    width: 280
    // height: childrenRect.height
    Layout.fillHeight: true
    Layout.fillWidth: true
    // anchors.verticalCenter: parent.verticalCenter
    // Layout.top: undefined
    // Layout.bottom: undefined

    color: "transparent"
    // border.color: "red"
    // border.width: 10
    property alias profileName: profileName.text
    property alias server: serverField.text
    property alias username: userField.text
    property alias password: passField.text
    property alias args: extraArgs.text
    property int textFieldHeight: 30

    Component.onCompleted: {
        console.log(height , childrenRect.height, "test", )
        // height = childrenRect.height
    }

    signal saveRequested()
    signal deleteRequested()
    signal connectRequested()

    ColumnLayout {
        // anchors.fill: parent
        // anchors.margins: 16
        spacing: 5

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Add / Update"
                Layout.fillWidth: true
                // Layout.maximumHeight: textFieldHeight
                onClicked: profileDetail.saveRequested()
            }

            Button {
                text: "Delete"
                Layout.preferredWidth: 100
                // Layout.maximumHeight: textFieldHeight
                onClicked: profileDetail.deleteRequested()
            }
        }

        TextField {
            id: profileName
            Layout.fillWidth: true
            // Layout.maximumHeight: textFieldHeight
            placeholderText: "Profile name"
        }

        TextField {
            id: serverField
            Layout.fillWidth: true
            // Layout.maximumHeight: textFieldHeight
            placeholderText: "Server URL"
        }

        TextField {
            id: userField
            Layout.fillWidth: true
            // Layout.maximumHeight: textFieldHeight
            placeholderText: "Username"
        }

        TextField {
            id: passField
            Layout.fillWidth: true
            // Layout.maximumHeight: textFieldHeight
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        TextField {
            id: extraArgs
            Layout.fillWidth: true
            // Layout.maximumHeight: textFieldHeight
            placeholderText: "Extra arguments (optional)"
        }

        Button {
            id: connectBtn
            Layout.fillWidth: true
            text: connected ? "Disconnect" : "Connect"
            height: textFieldHeight
            Material.background: connected ? Material.Red : Material.Green

            onClicked: profileDetail.connectRequested()
        }

        CheckBox {
            id: systemProxyCheck
            text: "System proxy"
            Layout.leftMargin: 20
            checked: systemProxy.enabled
            onToggled: {
                if (checked)
                    systemProxy.enableSystemProxy("127.0.0.1", 8080, "127.0.0.1", 1080)
                else
                    systemProxy.disableSystemProxy()
            }
        }
    }
}
