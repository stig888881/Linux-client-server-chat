// Клиент
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#define PORT 5000
#define MAXLINE 1024
using namespace std;
int main()
{
  int n;
  unsigned int len;
    int sockfd;
    int sockfdudp;
    char buffer[MAXLINE];
     string userInput;
     string Typeprotocol;
    struct sockaddr_in servaddr;


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Ошибка создания сокета");
        exit(0);
    }
    if ((sockfdudp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Ошибка создания сокета");
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&servaddr,
                             sizeof(servaddr)) < 0) {
        printf("\n Ошибка : нет соединения \n");
    }
cout<<"Введите протокол отправки(TCP/UDP):";
getline(cin, Typeprotocol);
if(Typeprotocol=="TCP"){
    do{
    cout << "> ";
     getline(cin, userInput);
     int sendRes = send(sockfd, userInput.c_str(), userInput.size() + 1, 0);
     if (sendRes == -1)
     {
         cout << "Не получается отправить на сервер\r\n";
         continue;
     }
     memset(buffer, 0, 1024);
            int bytesReceived = recv(sockfd, buffer, 1024, 0);
 cout << "Сервер: " << string(buffer, bytesReceived) << "\r\n";

      }while(true);
  }else{
    do{
        cout << "> ";
         getline(cin, userInput);
         sendto(sockfdudp,userInput.c_str() , userInput.size(), 0, (const struct sockaddr*)&servaddr,sizeof(servaddr));
         n = recvfrom(sockfdudp, (char*)buffer, MAXLINE,0, (struct sockaddr*)&servaddr,&len);
         cout << "Сервер> " << string(buffer, n) << "\r\n";
      }while(true);
  }
        close(sockfd);
         return 0;
}
