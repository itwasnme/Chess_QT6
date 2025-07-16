#include "NetworkManager.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>

NetworkManager::NetworkManager(Role role) : role(role) {}

NetworkManager::~NetworkManager() {
    if (conn_fd != -1) close(conn_fd);
    if (socket_fd != -1) close(socket_fd);
}

bool NetworkManager::host(int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); //Create the socket  (ipv4, TCP, default tcp)
    if (socket_fd < 0) return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // IPV4
    serverAddr.sin_port = htons(port); // LOCAL PORT
    serverAddr.sin_addr.s_addr = INADDR_ANY; //All available interfaces

    if (bind(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) //Bind it to all local IP interfaces and a local Port
        return false;

    if (listen(socket_fd, 1) < 0) return false; //Mark socket as a passive socket (one that will accept ONE incoming connections)

    std::cout << "Waiting for connection on port " << port << "...\n";
    conn_fd = accept(socket_fd, nullptr, nullptr); //Accept connection and return a file descriptor for it
    return conn_fd >= 0;
}

bool NetworkManager::join(const std::string& ip, int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); //Create the socket  (ipv4, TCP, default tcp)
    if (socket_fd < 0) return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // IPV4
    serverAddr.sin_port = htons(port); // Remote PORT
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr); // Remote IP address

    return connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) >= 0; // Attempt 3-way handshake
}

bool NetworkManager::sendMove(int from, int to) {
    std::ostringstream oss;
    oss << from << ":" << to << "\n";
    return sendAll(oss.str());
}

bool NetworkManager::receiveMove(int& from, int& to) {
    std::string msg = recvAll();
    std::istringstream iss(msg);
    std::string f, t;
    if (std::getline(iss, f, ':') && std::getline(iss, t)) {
        from = std::stoi(f);
        to = std::stoi(t);
        return true;
    }
    return false;
}

bool NetworkManager::sendAll(const std::string& msg) {
    const char* buf = msg.c_str();
    int totalSent = 0;
    int len = msg.length();
    int fd = (role == HOST) ? conn_fd : socket_fd;

    while (totalSent < len) {
        int sent = send(fd, buf + totalSent, len - totalSent, 0);
        if (sent <= 0) return false;
        totalSent += sent;
    }
    return true;
}

std::string NetworkManager::recvAll() {
    char buf[1024];
    int fd = (role == HOST) ? conn_fd : socket_fd;
    std::string result;

    while (true) {
        int bytes = recv(fd, buf, sizeof(buf), 0);
        if (bytes <= 0) break;
        result.append(buf, bytes);
        if (result.back() == '\n') break;
    }
    return result;
}

