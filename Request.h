//
// Created by 25382 on 2023/11/22.
//

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include "winsock2.h"

struct Char {
    Char(char* elem)
        : data {elem}
    {

    }

    ~Char()
    {
        delete data;
    }

    char* data;
};

namespace RequestMethod
{
    class TcpRequest {
    public:
        explicit TcpRequest(SOCKET);

    public:
        // 直接发送消息
        int send(const std::string&) const;
        int recv(std::string& buf, int buff_size=2048, int flag=0) const;

        // 获取SOCKET句柄
        [[nodiscard]] SOCKET getSocket() const
        {
            return sock;
        }

    private:
        // forbid to copy and move
        TcpRequest& operator=(TcpRequest&) = default;
        TcpRequest& operator=(TcpRequest&&) = default;
        TcpRequest(TcpRequest&) = default;
        TcpRequest(TcpRequest&&) = default;

// data
    protected:
        SOCKET  sock;
    };

    class HttpRequest : public TcpRequest
    {
    public:
        explicit HttpRequest(SOCKET);
        enum STATUS {NETWORK_ERROR=-1, FILE_ERROR=-2, SUCCESS=0};

        ~HttpRequest();
    public:
        // network
        int sendHeader(const std::string& message) const;
        int sendEmptyLine() const;
        int sendFile(std::string& fileName, int piece_size=1024, int flag=0) const;
        void close();
    };
}

#endif //HTTP_REQUEST_H
