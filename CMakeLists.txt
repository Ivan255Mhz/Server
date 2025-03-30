cmake_minimum_required(VERSION 3.10)
project(chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Сервер
add_executable(server
    server_main.cpp
    server.hpp
)

# Клиент
add_executable(client
    client_main.cpp
    client.hpp
)

# Общие настройки
foreach(target server client)
    target_link_libraries(${target} pthread)
endforeach()
