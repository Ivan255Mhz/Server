#include <clocale>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(){

  setlocale(LC_ALL,"ru");

  int server_fd = socket(AF_INET,SOCK_STREAM,0);

  sockaddr_in addres;

  addres.sin_family=AF_INET;
  addres.sin_addr.s_addr=INADDR_ANY;
  addres.sin_port=htons(8080);

  bind(server_fd ,(sockaddr*)&addres, sizeof(addres));
  listen(server_fd, 5);

  std::cout<<"Сервер запущен. Ожидание подключения..."<<std::endl;

  int client_socket = accept(server_fd,nullptr,nullptr);

  std::cout<<"Клиент подключен!"<<std::endl;

  char buffer[1024]{0};

  while (true) {

    read(client_socket, buffer,1024);

    std::cout<<"Клиент: "<< buffer<<std::endl;

    std::string response;
    std::cout<<"Сервер: ";
    std::getline(std::cin,response);
    send(client_socket, response.c_str(),response.size(),0);
  
  }

  close(server_fd);
  return 0;
}
