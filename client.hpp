#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>
#include <string.h>

class Client {
private:
    int sock;
    sockaddr_in serv_addr;
    std::string name;
    std::atomic<bool> running;

    void setupConnection() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            throw std::runtime_error("Ошибка создания сокета");
        }

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            throw std::runtime_error("Неверный адрес");
        }

        if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
            throw std::runtime_error("Ошибка подключения");
        }
      
    }

    void receiveMessages() {
        char buffer[2048];
        while (running) {
            ssize_t bytes_read = read(sock, buffer, sizeof(buffer));
            if (bytes_read <= 0) {
                running = false;
                break;
            }
            std::cout << "\r" << std::string(buffer, bytes_read) << "\n> " << std::flush;
        }
    }

public:
    Client(const std::string& server_ip, int port, const std::string& client_name)
        : name(client_name), running(true) {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
            throw std::runtime_error("Неверный адрес сервера");
        }
        
        setupConnection();
        send(sock, name.c_str(), name.size(), 0);
        
        std::thread receiver(&Client::receiveMessages, this);
        receiver.detach();
    }

    void sendMessage(const std::string& message) {
        send(sock, message.c_str(), message.size(), 0);
    }

    bool isRunning() const {
        return running;
    }

    ~Client() {
        running = false;
        close(sock);
    }
};

#endif
