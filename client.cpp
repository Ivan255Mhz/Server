#include <clocale>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(){

  setlocale(LC_ALL,"ru");

  int sock = socket(AF_INET, SOCK_STREAM,0);

  sockaddr_in serv_addr;
  serv_addr.sin_family =AF_INET;
  serv_addr.sin_port=htons(8080);
  inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr);


  connect(sock,(sockaddr*)&serv_addr,sizeof(serv_addr));
  std::cout<<"Подключено к серверу!"<<std::endl;

  char buffer[1024]{0};

  while (true) {

    std::string message;
    std::cout <<"Вы: ";
    std::getline(std::cin,message);
    send(sock,message.c_str(),message.size(),0);

    read(sock,buffer,1024);

    std::cout<<"Сервер: "<<buffer<<std::endl;
  }
  
  close(sock);


  return 0;
}
