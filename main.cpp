#include "sendFile.h"
#include "head.h"

int main(){
    using std::cout, std::endl;
    WORD ver {MAKEWORD(2, 2)};
    WSAData dat {};
    WSAStartup(ver, &dat);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sin {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(my_host);
    sin.sin_addr.S_un.S_addr = inet_addr(my_ip);

    //绑定端口
    if (SOCKET_ERROR != bind(sock, (sockaddr*)&sin, sizeof(sockaddr))){
        cout << "端口绑定成功" << endl;
    }
    else{
        cout << "端口绑定失败" << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    //监听
    if (SOCKET_ERROR != listen(sock, 128)){
        cout << "监听端口成功" << endl;
    }
    else{
        cout << "监听端口失败" << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    while (true){
        sockaddr_in clientsock {};
        int lenSock = sizeof(sockaddr);
        SOCKET client = accept(sock, (sockaddr*)&clientsock, &lenSock);

        if (SOCKET_ERROR == client){
            cout << "failed to connect";
            break;
        }
        else{
            std::string Filename {"C:\\Users\\25382\\Desktop\\2022年宣传册.zip"};
            cout << "client " << inet_ntoa(clientsock.sin_addr) << " has connected" << endl;
            if (sendFile(client, ZIP, Filename.c_str())){
                shutdown(client, SD_SEND);
                cout << "success to send message to " << inet_ntoa(clientsock.sin_addr) << endl;
            }
            else {
                cout <<  "failed to send message to " << inet_ntoa(clientsock.sin_addr) << endl;
                closesocket(client);
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}