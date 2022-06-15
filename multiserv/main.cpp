// Сервер
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include<stdio.h>
#include<stdlib.h>
#include <cstdlib>
#include<arpa/inet.h>
using namespace std;
#define PORT 5000
#define MAXLINE 1024
int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}
static int my_sum(const char *src)
{
    int sum = 0;
    while (*src) {
        while (*src && !isdigit(*src))
            ++src;
        sum += atoi(src);
        while (*src && isdigit(*src))
            ++src;
    }

    return sum;
}
int main()
{
    int listenfd, connfd, udpfd, nready, maxfdp1;
    char buff[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr;
    void sig_chld(int);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr,0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, 10);

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    FD_ZERO(&rset);

    maxfdp1 = max(listenfd, udpfd) + 1;
    for (;;) {

        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        nready = select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {
            len = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
            if ((childpid = fork()) == 0) {
                close(listenfd);
                while(1){
                    char buffer[MAXLINE];
                                    memset(buffer,0,sizeof(buffer));
                cout<<("Сообщение от TCP клиента: ");
                  int bytesReceived = recv(connfd, buffer, 1024, 0);
                cout << string(buffer, 0, bytesReceived)<< endl;

                int sum=my_sum(buffer);
                char str[4096];
                sprintf(str, "%d", sum);

        string strln, s;
        s+=" Искомые числа:";
        for (int i = 0; buffer[i]; i++)
           {
               if (isdigit(buffer[i]))
                   s += buffer[i];
               s+=" ";
           }

        int n= s.length();
        char digit[n+1];
        strcpy(digit,s.c_str());

        strcat(str,digit);
        if(sum > 0){
              send(connfd, str, sizeof(digit), 0);
          }else{
          send(connfd, buffer, sizeof(buffer), 0);
          }
          cout<<str;
                  }
              close(connfd);
              exit(0);
            }
          }

        if (FD_ISSET(udpfd, &rset)) {
            len = sizeof(cliaddr);
            memset(buff,0, sizeof(buff));
            cout<<("\nСообщение от UDP клиента: ");
            n = recvfrom(udpfd, buff, sizeof(buff), 0,(struct sockaddr*)&cliaddr, &len);
            int sum=my_sum(buff);
            char str[4096];
            sprintf(str, "%d", sum);

    string strln, s;
    s+=" Искомые числа:";
    for (int i = 0; buff[i]; i++)
       {
           if (isdigit(buff[i]))
               s += buff[i];
           s+=" ";
       }

    int n= s.length();
    char digit[n+1];
    strcpy(digit,s.c_str());

    strcat(str,digit);
                cout << string(buff, 0,sizeof(buff))<< endl;
                if (sum>0){
            sendto(udpfd, str, sizeof(digit), 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
                  }else{
                    sendto(udpfd, buff, sizeof(buff), 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
                  }
            cout<<str;
        }
      }
}
