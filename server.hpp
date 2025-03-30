#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <map>
#include <atomic>
#include <string.h>

class Server {
private:
    int server_fd;
    sockaddr_in address;
    const int PORT;
    std::atomic<bool> running;
    std::map<int, std::string> clients;
    std::mutex clients_mutex;

    void setupServer() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            throw std::runtime_error("Ошибка создания сокета");
        }

        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            throw std::runtime_error("Ошибка setsockopt");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
            throw std::runtime_error("Ошибка привязки");
        }

        if (listen(server_fd, 5) < 0) {
            throw std::runtime_error("Ошибка прослушивания");
        }
    }

    void handleClient(int client_socket) {
        char buffer[2048];
        
        // Получение имени
        ssize_t name_len = read(client_socket, buffer, sizeof(buffer));
        if (name_len <= 0) {
            close(client_socket);
            return;
        }
        
        std::string client_name(buffer, name_len);
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients[client_socket] = client_name;
        }
        
        broadcastMessage("*** " + client_name + " присоединился к чату ***");

        // Обработка сообщений
        while (running) {
            ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
            if (bytes_read <= 0) break;
            
            std::string message = client_name + ": " + std::string(buffer, bytes_read);
            broadcastMessage(message, client_socket);
        }

        // Отключение
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.erase(client_socket);
        }
        close(client_socket);
        broadcastMessage("*** " + client_name + " покинул чат ***");
    }

    void broadcastMessage(const std::string& message, int exclude_socket = -1) {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (const auto& [sock, _] : clients) {
            if (sock != exclude_socket) {
                send(sock, message.c_str(), message.size(), 0);
            }
        }
    }

public:
    Server(int port) : PORT(port), running(true) {
        setupServer();
        std::cout << "Сервер запущен на порту " << PORT << std::endl;
    }

    void start() {
        while (running) {
            int client_socket = accept(server_fd, nullptr, nullptr);
            if (client_socket < 0) continue;
            
            std::thread([this, client_socket]() {
                handleClient(client_socket);
            }).detach();
        }
    }

    ~Server() {
        running = false;
        close(server_fd);
    }
};

#endif
