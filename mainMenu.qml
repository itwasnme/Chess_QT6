import QtQuick 2.15

Window {
    id: rootWindow
    width: Screen.width * 0.5
    height: Screen.width * 0.5
    visible: true

    Item{
        Loader {
            id: gameLoader
            anchors.fill: parent
            active: false
            source: ""

            onLoaded: {
                if (item && item.goBackToMenu) {
                    item.goBackToMenu.connect(() => {
                        console.log("Returning to main menu")
                        gameLoader.source = ""
                        mainMenu.visible = true
                    })
                }
            }
        }
        anchors.fill: parent
        Rectangle{
            id: mainMenu
            anchors.fill: parent
            color:"black"

            Rectangle{
                id: logoRectangle
                width: mainMenu.width * 0.75
                height: mainMenu.height * 0.2
                anchors.horizontalCenter: mainMenu.horizontalCenter
                anchors.top: mainMenu.top
                anchors.topMargin: mainMenu.height * 0.08
                color: "black"
                Image{
                    id: logo
                    anchors.fill: parent
                    source: "assets/logo.png"
                    smooth: true
                }
            }

            Rectangle{
                id: localMultiplayerRectangle
                implicitWidth: localMultiplayer.implicitWidth
                implicitHeight: localMultiplayer.implicitHeight
                anchors.horizontalCenter: mainMenu.horizontalCenter
                anchors.top: logoRectangle.top
                anchors.topMargin: logoRectangle.height * 1.3
                color: localMultiplayerMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                radius: 5
                Text {
                    id: localMultiplayer
                    text: qsTr("LOCAL MULTIPLAYER")
                    color: "white"
                    font.bold: true
                    //anchors.fill: localMultiplayerRectangle
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: mainMenu.height * 0.05
                    padding: 5
                }
                MouseArea{
                    acceptedButtons: Qt.LeftButton
                    id: localMultiplayerMouseArea
                    anchors.fill: localMultiplayerRectangle
                    hoverEnabled: true
                    //onEntered: localMultiplayerRectangle.color = Qt.rgba(225/255, 225/255, 225/255, 0.2)
                    //onExited: localMultiplayerRectangle.color = "black"
                    onClicked: {
                        console.log("Handle local multiplayer")
                        mainMenu.visible = false
                        gameLoader.source = "qrc:/Chess_QT6/board.qml"
                        gameLoader.active = true
                    }
                }
            }

            Rectangle{
                id: joinOnlineMultiplayerRectangle
                implicitWidth: joinOnlineMultiplayer.implicitWidth
                implicitHeight: joinOnlineMultiplayer.implicitHeight
                anchors.horizontalCenter: mainMenu.horizontalCenter
                anchors.top: localMultiplayerRectangle.top
                anchors.topMargin: localMultiplayerRectangle.height * 1.1
                color: joinOnlineMultiplayerMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                radius: 5
                Text {
                    id: joinOnlineMultiplayer
                    text: qsTr("JOIN ONLINE MULTIPLAYER")
                    color: "white"
                    font.bold: true
                    //anchors.fill: onlineMultiplayerRectangle
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: mainMenu.height * 0.05
                    padding: 5
                }
                MouseArea{
                    acceptedButtons: Qt.LeftButton
                    id: joinOnlineMultiplayerMouseArea
                    anchors.fill: joinOnlineMultiplayerRectangle
                    hoverEnabled: true
                    onClicked: {
                        console.log("Handle join online multiplayer")
                        mainMenu.visible = false
                        gameLoader.source = "qrc:/Chess_QT6/joinMultiplayer.qml"
                        gameLoader.active = true
                    }
                }
            }

            Rectangle{
                id: hostOnlineMultiplayerRectangle
                implicitWidth: hostOnlineMultiplayer.implicitWidth
                implicitHeight: hostOnlineMultiplayer.implicitHeight
                anchors.horizontalCenter: mainMenu.horizontalCenter
                anchors.top: joinOnlineMultiplayerRectangle.top
                anchors.topMargin: joinOnlineMultiplayerRectangle.height * 1.1
                color: hostOnlineMultiplayerMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                radius: 5
                Text {
                    id: hostOnlineMultiplayer
                    text: qsTr("HOST ONLINE MULTIPLAYER")
                    color: "white"
                    font.bold: true
                    //anchors.fill: onlineMultiplayerRectangle
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: mainMenu.height * 0.05
                    padding: 5
                }
                MouseArea{
                    acceptedButtons: Qt.LeftButton
                    id: hostOnlineMultiplayerMouseArea
                    anchors.fill: hostOnlineMultiplayerRectangle
                    hoverEnabled: true
                    onClicked: {
                        console.log("Handle host online multiplayer")
                        mainMenu.visible = false
                        gameLoader.source = "qrc:/Chess_QT6/hostMultiplayer.qml"
                        gameLoader.active = true
                    }
                }
            }

            Rectangle{
                id: playVsComputerRectangle
                implicitWidth: playVsComputer.implicitWidth
                implicitHeight: playVsComputer.implicitHeight
                anchors.horizontalCenter: mainMenu.horizontalCenter
                anchors.top: hostOnlineMultiplayerRectangle.top
                anchors.topMargin: hostOnlineMultiplayerRectangle.height * 1.1
                color: playVsComputerMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                radius: 5
                Text {
                    id: playVsComputer
                    text: qsTr("PLAY VS COMPUTER")
                    color: "white"
                    font.bold: true
                    //anchors.fill: playVsComputerRectangle
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: mainMenu.height * 0.05
                    padding: 5
                }
                MouseArea{
                    acceptedButtons: Qt.LeftButton
                    id: playVsComputerMouseArea
                    anchors.fill: playVsComputerRectangle
                    hoverEnabled: true
                    onClicked: {
                        console.log("Handle play vs cpu")
                        mainMenu.visible = false
                        gameLoader.source = "qrc:/Chess_QT6/aiboard.qml"
                        gameLoader.active = true
                    }
                }
            }
        }
    }
}
