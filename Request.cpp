//
// Created by 25382 on 2023/11/22.
//

#include "Request.h"
#include <fstream>
#include <memory>

RequestMethod::TcpRequest::TcpRequest(SOCKET sock)
    : sock {sock}
{

}

int RequestMethod::TcpRequest::send(const std::string& message) const {
    return ::send(this->sock, message.c_str(),
                  static_cast<int>(message.length()), 0);
}

int RequestMethod::TcpRequest::recv(std::string& buf, int buff_size, int flag) const {
    std::shared_ptr<Char> tempBuf = std::make_shared<Char>(new char[buff_size]);
    int res = ::recv(sock, tempBuf->data, buff_size, flag);
    if (-1 != res)
        buf = tempBuf->data;
    return res;
}

RequestMethod::HttpRequest::HttpRequest(SOCKET sock)
: TcpRequest(sock)
{

}

int RequestMethod::HttpRequest::sendHeader(const std::string &message) const {
    return TcpRequest::send(message);
}

int RequestMethod::HttpRequest::sendEmptyLine() const {
    return TcpRequest::send("\r\n");
}

int RequestMethod::HttpRequest::sendFile(std::string &fileName, int piece_size, int flag) const {
    std::ifstream f(fileName, std::fstream::binary);
    if (! f.is_open())
        return STATUS::FILE_ERROR;

    size_t fileSize = static_cast<int>(f.tellg());

    if (fileSize < 5 * 1024 * 1024)
    {
        std::string data((std::istreambuf_iterator<char>(f)), (std::istreambuf_iterator<char>()));
        ::send(this->sock, data.c_str(), static_cast<int>(data.length()), flag);
    } // 小于5M
    else
    {
        std::shared_ptr<Char> data = std::make_shared<Char>(new char[piece_size]);
        while (f.read(data->data, piece_size))
        {
            ::send(this->sock, data->data, static_cast<int>(strlen(data->data)), flag);
        }
    } // 发送大文件

    f.close();
    return 0;
}

void RequestMethod::HttpRequest::close() {
    closesocket(this->sock);
}

RequestMethod::HttpRequest::~HttpRequest() {
    this->close();
}



