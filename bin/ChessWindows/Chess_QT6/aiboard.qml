import QtQuick

Item {
    id: root
    width: Screen.width * 0.5
    height: Screen.width * 0.5

    Component.onCompleted: {
        //console.log("gameBoard is", typeof gameBoard)
        //console.log("pieces are", typeof gameBoard?.pieces)
    }
    property var pieceList: gameBoard.pieces
    property int firstClickIndex: -1  // stores first click (-1 means no click yet)
    property string gameFeedback: ""
    property string currentTurn: "White's Turn"
    signal goBackToMenu()

    Rectangle {
        id: chessBoard
        anchors.fill: parent
        color: "black"
        transformOrigin: Item.Center

        Row{
            id: topRow
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottomMargin: 5
            spacing: parent.width * 0.09

            Rectangle{
                id: backButton
                implicitWidth: backButtonDialog.implicitWidth
                implicitHeight: backButtonDialog.implicitHeight
                radius: 5
                color: backButtonMouseArea.containsMouse ? Qt.rgba(1, 0, 0, 0.4) : "black"
                Text {
                    id: backButtonDialog
                    text: qsTr("Exit Game")
                    color: "white"
                    padding: 3
                    font.pixelSize: topRow.width * 0.025
                }
                MouseArea{
                    id: backButtonMouseArea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    hoverEnabled: true
                    onClicked: {
                        console.log("Handle going back to main menu")
                        gameBoard.resetGame()
                        root.goBackToMenu()
                    }
                }
            }

            Rectangle{
                id: gameOver
                implicitWidth: gameOverDialog.implicitWidth
                height: gameOverDialog.implicitHeight
                color: "black"
                visible: gameBoard.end

                Text {
                    id: gameOverDialog
                    text: gameBoard.turn ? "♔ White is in checkmate — Black wins!" : "♚ Black is in checkmate — White wins!"
                    color: "red"
                    font.bold: true
                    font.pixelSize: topRow.width * 0.03
                    padding: 3
                }
            }
        }

        Grid {
            id: boardGrid
            anchors.top: topRow.bottom
            anchors.bottom: footer.top
            anchors.bottomMargin: 10
            width: height
            anchors.horizontalCenter: parent.horizontalCenter
            rows: 8
            columns: 8

            Repeater {
                id: boardRepeater
                model: 64
                delegate: Rectangle {
                    id: square
                    width: boardGrid.width / 8
                    height: boardGrid.height / 8
                    border.width: 2
                    color: (Math.floor(index / 8) + index % 8) % 2 === 0 ? "#f0d9b5" : "#b58863"
                    border.color: (Math.floor(index / 8) + index % 8) % 2 === 0 ? "#f0d9b5" : "#b58863"

                    Image {
                        anchors.centerIn: parent
                        source: pieceList[index] && pieceList[index].name && pieceList[index].color
                                ? "assets/" + pieceList[index].color + pieceList[index].name + ".png"
                                : ""
                        visible: source !== ""
                        anchors.fill: parent
                        anchors.topMargin: pieceList[index].name === "king" ? parent.width * 0.1 : parent.width * 0.2
                        anchors.bottomMargin: parent.width * 0.05
                        anchors.leftMargin: pieceList[index].name === "pawn" ? parent.width * 0.1 : parent.width * 0.05
                        anchors.rightMargin: pieceList[index].name === "pawn" ? parent.width * 0.1 : parent.width * 0.05
                        smooth: false
                    }

                    MouseArea {
                        id: currentSquare
                        anchors.fill: parent
                        enabled: gameBoard.end ? false : true
                        onClicked: {
                            if (root.firstClickIndex === -1) {
                                // First click: store index
                                if(pieceList[index].name !== "" && ((pieceList[index].color === "white" && gameBoard.turn) || (pieceList[index].color === "black" && !gameBoard.turn) )){
                                    root.firstClickIndex = index
                                    square.color = Qt.rgba(134/255, 186/255, 255/255, 0.9)
                                    square.border.color = Qt.rgba(134/255, 186/255, 255/255, 0.5)
                                    console.log("First click stored at index:", index)
                                }
                            } else {
                                var oldItem = boardRepeater.itemAt(root.firstClickIndex);
                                if (oldItem) {030998

                                    oldItem.color = ((Math.floor(root.firstClickIndex / 8) + root.firstClickIndex % 8) % 2 === 0) ? "#f0d9b5" : "#b58863";
                                    oldItem.border.color = ((Math.floor(root.firstClickIndex / 8) + root.firstClickIndex % 8) % 2 === 0) ? "#f0d9b5" : "#b58863";
                                }
                                const x = root.firstClickIndex
                                const y = index
                                if(x != y){
                                    console.log("Moving piece from", x, "to", y)
                                    gameFeedback = gameBoard.move(x, y)
                                    console.log("Move result:", gameFeedback)
                                    currentTurn = gameBoard.currentTurn()
                                    if(gameFeedback == "" && !gameBoard.promotionFlag){
                                        gameBoard.aimove()
                                        currentTurn = gameBoard.currentTurn()
                                    }
                                }
                                root.firstClickIndex = -1  // reset
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: footer
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            color: "black"

            // Dynamically set size depending on which element is visible
            implicitWidth: gameBoard.promotionFlag ? promotionRow.implicitWidth : feedback.implicitWidth
            implicitHeight: gameBoard.promotionFlag ? promotionRow.implicitHeight : feedback.implicitHeight

            // Feedback text when not promoting
            Text {
                id: feedback
                visible: !gameBoard.promotionFlag
                anchors.horizontalCenter: parent.horizontalCenter
                text: gameFeedback === "" ? currentTurn : gameFeedback
                color: "white"
                font.bold: true
                bottomPadding: 5
                topPadding: 2
                font.pixelSize: root.height <= root.width ? root.width * 0.02 : root.height * 0.02
            }

            // Promotion options when promotion flag is true
            Row {
                id: promotionRow
                visible: gameBoard.promotionFlag
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: parent.width * 0.05

                Rectangle {
                    id: queenPromotion
                    width: queenText.implicitWidth
                    height: queenText.implicitHeight
                    radius: 5
                    color: queenMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                    Text {
                        id: queenText
                        anchors.centerIn: parent
                        text: qsTr("Promote to Queen")
                        color: "white"
                        font.bold: true
                        bottomPadding: 5
                        topPadding: 2
                        font.pixelSize: root.height <= root.width ? root.width * 0.02 : root.height * 0.02
                    }
                    MouseArea {
                        id: queenMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            gameBoard.pickPromotion(0)
                            gameBoard.aimove()
                        }
                    }
                }

                Rectangle {
                    id: rookPromotion
                    width: rookText.implicitWidth
                    height: rookText.implicitHeight
                    radius: 5
                    color: rookMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                    Text {
                        id: rookText
                        anchors.centerIn: parent
                        text: qsTr("Promote to Rook")
                        color: "white"
                        font.bold: true
                        bottomPadding: 5
                        topPadding: 2
                        font.pixelSize: root.height <= root.width ? root.width * 0.02 : root.height * 0.02
                    }
                    MouseArea {
                        id: rookMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            gameBoard.pickPromotion(1)
                            gameBoard.aimove(x, y)
                        }
                    }
                }

                Rectangle {
                    id: bishopPromotion
                    width: bishopText.implicitWidth
                    height: bishopText.implicitHeight
                    radius: 5
                    color: bishopMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                    Text {
                        id: bishopText
                        anchors.centerIn: parent
                        text: qsTr("Promote to Bishop")
                        color: "white"
                        font.bold: true
                        bottomPadding: 5
                        topPadding: 2
                        font.pixelSize: root.height <= root.width ? root.width * 0.02 : root.height * 0.02
                    }
                    MouseArea {
                        id: bishopMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            gameBoard.pickPromotion(2)
                            gameBoard.aimove(x, y)
                        }
                    }
                }

                Rectangle {
                    id: knightPromotion
                    width: knightText.implicitWidth
                    height: knightText.implicitHeight
                    radius: 5
                    color: knightMouseArea.containsMouse ? Qt.rgba(225/255, 225/255, 225/255, 0.2) : "black"
                    Text {
                        id: knightText
                        anchors.centerIn: parent
                        text: qsTr("Promote to Knight")
                        color: "white"
                        font.bold: true
                        bottomPadding: 5
                        topPadding: 2
                        font.pixelSize: root.height <= root.width ? root.width * 0.02 : root.height * 0.02
                    }
                    MouseArea {
                        id: knightMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            gameBoard.pickPromotion(3)
                            gameBoard.aimove(x, y)
                        }
                    }
                }

            }
        }
    }
}
