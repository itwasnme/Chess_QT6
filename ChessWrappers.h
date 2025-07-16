#ifndef CHESSWRAPPERS_H
#define CHESSWRAPPERS_H

#include <QObject>
#include "piece.h"

class PieceWrapper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString color READ color CONSTANT)

public:
    explicit PieceWrapper(piece* p, QObject* parent = nullptr);
    QString name() const;
    QString color() const;

private:
    piece* m_piece;
};

#endif // CHESSWRAPPERS_H



/////////////////////////////////////////////////////////////////////



#ifndef BOARDWRAPPER_H
#define BOARDWRAPPER_H

#include <QObject>
#include <QString>
#include "board.h"
#include "AI.h"
#include "NetworkManager.h"

class BoardWrapper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> pieces READ pieces NOTIFY stateChanged)
    Q_PROPERTY(bool turn READ turn NOTIFY stateChanged)
    Q_PROPERTY(bool inCheck READ inCheck NOTIFY stateChanged)
    Q_PROPERTY(bool end READ isGameOver NOTIFY stateChanged)
    Q_PROPERTY(bool promotionFlag READ promotionFlag NOTIFY stateChanged)
    Q_PROPERTY(NetworkManager* networkManager READ getNetworkManager NOTIFY networkManagerChanged)
    Q_PROPERTY(QString Sturn READ currentTurnStatus NOTIFY stateChanged)

public:
    explicit BoardWrapper(QObject* parent = nullptr);
    Q_DISABLE_COPY(BoardWrapper)

    Q_INVOKABLE QString move(int x, int y);
    Q_INVOKABLE QString aimove();
    Q_INVOKABLE void resetGame();
    Q_INVOKABLE void pickPromotion(int x);
    Q_INVOKABLE QString currentTurn();
    Q_INVOKABLE bool startHosting(int port);
    Q_INVOKABLE bool joinGame(const QString& ip, int port);


    bool turn() const { return gameBoard.turn; }
    bool inCheck() const { return gameBoard.in_check; }
    bool isGameOver() const { return gameBoard.end; }
    bool promotionFlag() const { return gameBoard.promotionFlag; }
    QString currentTurnStatus() const { return gameBoard.turn ? "White's Turn" : "Black's Turn"; }
    QList<QObject*> pieces() const { return m_wrappedPieces; }
    QList<QObject*> updatePiecesPositions();
    NetworkManager* getNetworkManager() const { return networkManager; }

signals:
    void stateChanged();
    void networkManagerChanged();

private:
    board gameBoard;
    QList<QObject*> m_wrappedPieces;
    NetworkManager* networkManager = nullptr;
};


#endif

