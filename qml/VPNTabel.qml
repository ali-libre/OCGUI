import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Qt.labs.qmlmodels 1.0

TableView {
    id: profileTable
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.minimumWidth: 600
    Layout.minimumHeight: 400

    // anchors.fill: parent
    clip: true
    columnSpacing: 2
    rowSpacing: 2
    leftMargin: verticalHeader.implicitWidth + 5
    topMargin: horizontalHeader.childrenRect.height + 5
    property var profileDetail
    columnWidthProvider: function(col) { return columnWidths[col] }
    rowHeightProvider: function (column) { return 25 }
    function refresh() {
        profileModel.rows = jsArray(profiles.getProfiles())
    }
    property var horizontal_header_data: ["name", "server", "user", "args"]
    property var columnWidths: [140, 180, 120, 140]
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.horizontal: ScrollBar{
        //policy: "AlwaysOn"
    }
    ScrollBar.vertical: ScrollBar{
        //policy: "AlwaysOn"
    }
    ScrollIndicator.horizontal: ScrollIndicator { }
    ScrollIndicator.vertical: ScrollIndicator { }

    Component.onCompleted: {
        refresh()

    }
    model: TableModel {
        id: profileModel
        TableModelColumn { display: "name" }
        TableModelColumn { display: "server" }
        TableModelColumn { display: "username" }
        TableModelColumn { display: "args" }

        // rows: jsArray(profiles.getProfiles())
    }

    Row {
        id: horizontalHeader
        y: profileTable.contentY
        z: 2
        spacing: 2

        Repeater {
            model: profileTable.columns
            Rectangle {
                width: columnWidths[index]
                height: 30
                color: "#b5b5b5"

                Text {
                    anchors.centerIn: parent
                    text: horizontal_header_data[index]
                    color: "white"

                }

                Rectangle {
                    id: handle
                    anchors.right: parent.right
                    width: 6
                    height: parent.height
                    color: "transparent"
                }
                // onWidthChanged: profileTable.focusChanged()
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        profileTable.forceLayout()
                        console.log(mouseX)
                    }
                }
            }

        }

    }

    Column {
            id: verticalHeader
            x: profileTable.contentX
            z: 2
            spacing: 2
            Repeater {
                model: profileTable.rows
                Label {
                    width: 36;
                    height: profileTable.rowHeightProvider(modelData)
                    text: index
                    padding: 10
                    verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                    color: "white"
                    background: Rectangle { color: "#b5b5b5" }
                }
            }
        }


    delegate:
        Rectangle {
        implicitHeight: 36
        width: columnWidths[column]
        color: selectedRow === row ? "#336699" : "#222"
        border.color: "#444"


        Text {
            anchors.centerIn: parent
            color: "white"
            text: display
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                selectedRow = row
                let p = profileModel.rows[row]

                profileDetail.profileName = p.name || ""
                profileDetail.server = p.server || ""
                profileDetail.username = p.username || ""
                profileDetail.password = p.password || ""
                profileDetail.args = p.args || ""
            }

        }
    }
}
