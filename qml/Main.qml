import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Qt.labs.qmlmodels

ApplicationWindow {
    visible: true
    width: 1000
    height: 800
    title: "OpenConnect GUI"
    minimumWidth: 900

    Material.theme: Material.Dark
    Material.accent: Material.Blue

    property bool connected: false
    property bool autoScroll: true
    property int selectedRow: -1

    function jsArray(v) {
        var a = []
        for (var i = 0; i < v.length; i++)
            a.push(v[i])
        return a
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Label {
                text: "OpenConnect Client"
                font.pixelSize: 22
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                text: "Profiles"
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }

        // جدول + فرم
        RowLayout {
            id: profilerow
            // Layout.fillWidth: true
            // Layout.fillHeight: true
            spacing: 20
            Layout.leftMargin: 20
            Layout.rightMargin: 20

            // جدول
            VPNTabel {
                id: vpntable
                profileDetail: profileDetail
            }

            ProfileDetail {
                id: profileDetail

                onSaveRequested: {
                    profiles.saveProfile(
                        profileDetail.profileName,
                        profileDetail.server,
                        profileDetail.username,
                        profileDetail.password,
                        profileDetail.args
                    )
                    vpntable.refresh()
                }

                onDeleteRequested: {
                    profiles.deleteProfile(profileDetail.profileName)
                    vpntable.refresh()
                }

                onConnectRequested: {
                    if (!connected) {
                        vpn.connectVPN(
                            profileDetail.server,
                            profileDetail.username,
                            profileDetail.password,
                            profileDetail.args
                        )
                    } else {
                        vpn.disconnectVPN()
                    }
                }
            }

        }

        // لاگ پایین
        Rectangle {
            id: logPanel
            Layout.fillWidth: true
            Layout.preferredHeight: 220
            radius: 6
            color: "#222"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true

                    Button {
                        id: cleanBtn
                        text: "Clean"
                        Layout.preferredWidth: 100
                        onClicked: logArea.text = ""
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: logArea
                        readOnly: true
                        wrapMode: TextArea.Wrap
                        color: "white"
                    }
                }
            }
        }
    }

    Connections {
        target: vpn

        function onLogUpdated(text) {
            logArea.text += text
            if (autoScroll)
                logArea.cursorPosition = logArea.length
        }

        function onConnectionStateChanged(state) {
            connected = state
        }
    }
}
