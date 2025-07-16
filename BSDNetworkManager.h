#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H


#include <string>

enum Role { HOST, CLIENT };

class NetworkManager {
public:
    NetworkManager(Role role);
    ~NetworkManager();

    // Setup connections
    bool host(int port);                            // Start server and accept one connection
    bool join(const std::string& ip, int port);     // Connect to host

    // Send and receive moves
    bool sendMove(int from, int to);                // Send move over TCP
    bool receiveMove(int& from, int& to);           // Blocking receive

private:
    int socket_fd = -1;
    int conn_fd = -1;   // Only used for host
    Role role;

    bool sendAll(const std::string& msg);
    std::string recvAll();
};



#endif // NETWORKMANAGER_H
