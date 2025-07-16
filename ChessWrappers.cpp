#include "ChessWrappers.h"
#include <QString>

PieceWrapper::PieceWrapper(piece* p, QObject* parent) : QObject(parent) // You still need to call base constructor here
{
    this->m_piece = p;
}

QString PieceWrapper::name() const {
    return QString::fromStdString(m_piece->name);
}

QString PieceWrapper::color() const {
    return QString::fromStdString(m_piece->color);
}


////////////////////


BoardWrapper::BoardWrapper(QObject* parent)
    : QObject(parent)
{
    updatePiecesPositions();
}


QList<QObject*> BoardWrapper::updatePiecesPositions(){
    qDeleteAll(m_wrappedPieces);
    m_wrappedPieces.clear();
    for (piece* p : gameBoard.chessboard) {
        m_wrappedPieces.append(new PieceWrapper(p, this));  // `this` = BoardWrapper, parent owns it
    }
    return m_wrappedPieces;
}

QString BoardWrapper::move(int x, int y)
{
    QString result = QString::fromStdString(gameBoard.move(x, y));
    updatePiecesPositions();
    emit stateChanged();  // Inform QML things changed
    return result;
}

QString BoardWrapper::aimove()
{
    moveAI(&gameBoard);
    updatePiecesPositions();
    emit stateChanged();  // Inform QML things changed
    return "";
}

void BoardWrapper::resetGame(){
    gameBoard.initBoard();
    updatePiecesPositions();
}

QString BoardWrapper::currentTurn(){
    if (turn()) return "White's Turn";
    else return "Black's Turn";
}

void BoardWrapper::pickPromotion(int x){
    gameBoard.promotion(x);
    updatePiecesPositions();  // <-- Add this here to refresh wrappers immediately. Glueing is not as trivial...
    emit stateChanged();
}

bool BoardWrapper::startHosting(int port) {
    // Clean up old instance if any
    if (networkManager) {
        delete networkManager;
        networkManager = nullptr;
    }

    networkManager = new NetworkManager(HOST, this);

    // Connect signal for receiving opponent's move
    connect(networkManager, &NetworkManager::moveReceived, this, [this](int from, int to) {
        cout<<"BoardWrapper::startHostingmoveReceived"<<endl;
        gameBoard.move(from, to);
        updatePiecesPositions();
        emit stateChanged();
    });
    connect(networkManager, &NetworkManager::promotionReceived, this, [this](int promo) {
        cout<<"BoardWrapper::startHostingpromotionReceived"<<endl;
        gameBoard.promotion(promo);
        updatePiecesPositions();
        emit stateChanged();
    });

    emit networkManagerChanged();
    if(networkManager->host(port)){
        cout<<"Success!"<<endl;
        return true;
    }else{
        cout<<"Failure!"<<endl;
        return false;
    }
}

bool BoardWrapper::joinGame(const QString& ip, int port) {
    if (networkManager) {
        delete networkManager;
        networkManager = nullptr;
    }

    networkManager = new NetworkManager(CLIENT, this);

    // Connect the same signal
    connect(networkManager, &NetworkManager::moveReceived, this, [this](int from, int to) {
        cout<<"BoardWrapper::joinGame:MoveReceived"<<endl;
        gameBoard.move(from, to);
        updatePiecesPositions();
        emit stateChanged();
    });
    connect(networkManager, &NetworkManager::promotionReceived, this, [this](int promo) {
        cout<<"BoardWrapper::joinGame:promotionReceived"<<endl;
        gameBoard.promotion(promo);
        updatePiecesPositions();
        emit stateChanged();
    });

    emit networkManagerChanged();
    if(networkManager->join(ip, port)){
        cout<<"Success!"<<endl;
        return true;
    }else{
        cout<<"Failure!"<<endl;
        return false;
    }
}





