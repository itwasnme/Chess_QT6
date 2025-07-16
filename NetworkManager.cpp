#include "NetworkManager.h"
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <iostream>

NetworkManager::NetworkManager(Role role, QObject* parent)
    : QObject(parent), role(role) {}

NetworkManager::~NetworkManager() {
    if (socket) socket->close();
    if (server) server->close();
    delete socket;
    delete server;
}

bool NetworkManager::host(int port) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);

    bool success = server->listen(QHostAddress::Any, port);
    if (!success) {
        qWarning() << "Failed to listen on port" << port;
        return false;
    }

    qInfo("Server listening on port %d", port);
    return true;
}

void NetworkManager::onNewConnection() {
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    qInfo("Client connected.");
    emit connected();
    emit connectionStatusChanged();
}

bool NetworkManager::join(const QString& ip, int port) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::connected);
    socket->connectToHost(ip, port);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onDisconnected);
    emit connectionStatusChanged();
    return true; // Success will be notified via `connected()` signal
}

bool NetworkManager::sendMove(int from, int to) {
    QString msg = QString("%1:%2\n").arg(from).arg(to);
    return sendAll(msg);
}

bool NetworkManager::sendPromotion(int promo) {
    std::cout<<"NetworkManager::sendPromotion"<<std::endl;
    QString msg = QString("%1\n").arg(promo);
    return sendAll(msg);
}

bool NetworkManager::sendAll(const QString& msg) {
    if (!socket || !socket->isOpen()) return false;
    QByteArray data = msg.toUtf8();
    qint64 totalSent = 0;
    while (totalSent < data.size()) {
        qint64 sent = socket->write(data.mid(totalSent));
        if (sent <= 0) return false;
        totalSent += sent;
    }
    return socket->flush();
}

bool NetworkManager::connectionStatus() {
    return socket && socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::disconnect() {
    if (socket) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(3000);  // optional timeout
        }
    }

    if (server) {
        server->close();
    }

    qInfo() << "Disconnected cleanly.";
    emit disconnected();
}

void NetworkManager::onReadyRead() {
    if (!socket) return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        QString msg = QString::fromUtf8(line).trimmed();
        QStringList parts = msg.split(":");
        if (parts.size() == 1) {
            bool ok1 = false;
            int promo = parts[0].toInt(&ok1);
            if (ok1) {
                std::cout<<"NetworkManager::onReadyRead():promotionReceived"<<std::endl;
                emit promotionReceived(promo);
            }
        }
        if (parts.size() == 2) {
            bool ok1 = false, ok2 = false;
            int from = parts[0].toInt(&ok1);
            int to = parts[1].toInt(&ok2);
            if (ok1 && ok2) {
                std::cout<<"NetworkManager::onReadyRead():moveReceived"<<std::endl;
                emit moveReceived(from, to);
            }
        }
    }
}

void NetworkManager::onDisconnected() {
    qInfo() << "Peer disconnected.";
    socket->close();
    emit disconnected();
    emit connectionStatusChanged();
}



