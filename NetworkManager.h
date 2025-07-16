#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

enum Role { HOST, CLIENT };

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(Role role, QObject* parent = nullptr);
    ~NetworkManager();

    bool host(int port);                            // Start listening
    bool join(const QString& ip, int port);         // Connect to host

    Q_INVOKABLE bool sendMove(int from, int to);                // Send move
    Q_INVOKABLE bool sendPromotion(int promo);                // Send Promotion
    Q_INVOKABLE void disconnect();

    Q_PROPERTY(bool connectionUP READ connectionStatus NOTIFY connectionStatusChanged)
    bool connectionStatus();

signals:
    void connected();                               // Emitted when connected (host or client)
    void moveReceived(int from, int to);            // Emitted when a move is received
    void promotionReceived(int promo);              // Emitted when pomotion choice is received
    void disconnected();
    void connectionStatusChanged();


private slots:
    void onNewConnection();
    void onReadyRead();                             // Handle incoming data
    void onDisconnected();

private:
    Role role;
    QTcpServer* server = nullptr;
    QTcpSocket* socket = nullptr;

    bool sendAll(const QString& msg);
};

#endif // NETWORKMANAGER_H
