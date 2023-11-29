#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <cstdio>
#include <cstring>
#include <winsock2.h>
#include <cstdlib>
#include "tool.h"
#include "ThreadPool.cpp"
using std::cout, std::endl;

int main(int argc, char** args)
{
    WSADATA wasData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAdr, clntAdr;

    int clntAdrSize;

    if (WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
        ErrorHandling("WSAStartup() error!");

    hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = inet_addr(args[1]);
    servAdr.sin_port = htons(atoi(args[2]));

    if (SOCKET_ERROR == bind(hServSock, (SOCKADDR*)(&servAdr), sizeof(servAdr)))
        ErrorHandling("bind() error!");
    if (SOCKET_ERROR == listen(hServSock, 5))
        ErrorHandling("listen() error!");
    printf("ip: %s, port: %d\n", inet_ntoa(servAdr.sin_addr), ntohs(servAdr.sin_port));


    // 建立线程池
    ThreadPool<std::function<unsigned(void*)>> pool(12);
    while (1)
    {
        printf("start listen...\n");
        clntAdrSize = sizeof(clntAdr);
        hClntSock = accept(hServSock, (SOCKADDR*)(&clntAdr), &clntAdrSize);
        printf("Connection Request : %s : %d\n",
               inet_ntoa(clntAdr.sin_addr), clntAdr.sin_port);
        pool.addTask(RequestMethod::RequestHandler, (void*)hClntSock);
    }

    closesocket(hServSock);
    WSACleanup();
    return 0;
}

