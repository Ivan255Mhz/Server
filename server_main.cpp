#include "server.hpp"
#include <stdexcept>

int main() {
    try {
        Server server(8080);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
