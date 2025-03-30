#include "client.hpp"
#include <stdexcept>

int main() {
    try {
        std::string name;
        std::cout << "Введите ваше имя: ";
        std::getline(std::cin, name);

        Client client("127.0.0.1", 8080, name);
        
        std::string message;
        while (client.isRunning()) {
            std::cout << "> ";
            std::getline(std::cin, message);
            client.sendMessage(message);
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
