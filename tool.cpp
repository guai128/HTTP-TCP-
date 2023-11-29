//
// Created by 25382 on 2023/11/22.
//

#include "tool.h"
#include <sstream>
#include <iostream>
#include <unordered_map>

using std::cout, std::endl;
unsigned RequestMethod::RequestHandler(void* arg)
{
    HttpRequest hClnt((SOCKET)arg);
    using std::string, std::stringstream;
    string buf, method, version, ct, fileName;
    stringstream ss;

    // 获取请求
    hClnt.recv(buf);
    std::cout << buf << std::endl;
    if (buf.find("HTTP/") == -1)
    {
        SendErrorMSG(hClnt);
        return 1;
    }
    ss << buf;
    ss >> method;
    if (method ==  "GET")
    {
        return RequestMethod::GETHandler(hClnt, ss);
    }
    else if (method == "POST")
    {
        return RequestMethod::POSTHandler(hClnt, ss);
    }
    else if (method == "HEAD")
    {
        return RequestMethod::HEADHandler(hClnt, ss);
    }
    else
    {
        SendErrorMSG(hClnt);
        return 1;
    }
}

int RequestMethod::GETHandler(HttpRequest& hClnt, std::stringstream& ss)
{
    using std::string, std::stringstream;
    string version, ct, fileName;
    ss >> fileName;
    fileName.erase(fileName.begin()); // 去除开头的/
    ss >> version;
    ct = ContentType(fileName); // 查看Content-type
    cout << "fileName:" << fileName << ", ct: " << ct << ", start send data\n" << endl;
    SendData(hClnt, ct, fileName);
    return 0;
}

int RequestMethod::POSTHandler(HttpRequest& hClnt, std::stringstream& ss)
{
    SendErrorMSG(hClnt);
    return 1;
}

int RequestMethod::HEADHandler(HttpRequest& hClnt, std::stringstream& ss)
{
    using std::string, std::stringstream;
    string version, ct, fileName;
    ss >> fileName;
    fileName.erase(fileName.begin()); // 去除开头的/
    ss >> version;
    ct = ContentType(fileName); // 查看Content-type
    cout << "fileName:" << fileName << ", ct: " << ct << ", start send data\n" << endl;
    SendData(hClnt, ct, fileName);
    string protocol = "HTTP/1.0 200 OK\r\n";
    string servName = "Server:simple web server\r\n";
    FILE* sendFile;
    ss.clear();
    ss << "Content-type: " << ct << "\r\n";
    string cntType {ss.str()};
    cout << "cntType: " << cntType << endl;
    if ((sendFile = fopen(fileName.c_str(), "r")) == nullptr)
    {
        SendErrorMSG(hClnt);
        return -1;
    }
    // 传输头消息
    hClnt.sendHeader(protocol);
    hClnt.sendHeader(servName);
    hClnt.sendHeader(cntType);

    return 0;
}


std::string ContentType(const std::string& fileName)
{
    using std::string;
    // 将文件后缀映射到返回内容
    static std::unordered_map<string, string> all_types =
            {
                    {".", "application/x-"},
                    {".*", "application/octet-stream"},
                    {".pdf", "application/pdf"},
                    {".ai", "application/postscript"},
                    {".xml", "application/atom+xml"},
                    {".js", "application/javascript"},
                    {".json", "application/json"},
                    {".rdf", "application/rdf+xml"},
                    {".zip", "application/zip"},
                    {".gzip", "application/gzip"},
                    {".dll", "application/x-msdownload"},
                    {".exe", "application/x-msdownload"},
                    {".html", "text/html"},
                    {".htm", "text/html"},
                    {".wav", "audio/wav"},
                    {".mp4", "video/mpeg4"},
                    {".jpe", "image/jpeg"},
                    {".jpeg", "image/jpeg"},
                    {".jpg", "image/jpeg"},
                    {".png", "image/png"},
                    {".ico", "image/x-icon"}
            };

    string extension;
    size_t pos = fileName.find_last_of('.');
    if (pos != -1)
        extension = fileName.substr(pos, fileName.length() - pos);
    else
        extension = '.';

    cout << "Extension : " << extension << endl;
    if (all_types.count(extension))
        return all_types[extension];
    else
        return "text/plain";
}

void SendData(RequestMethod::HttpRequest& sock, const std::string& ct, std::string& fileName)
{
    using std::string, std::stringstream;
    string protocol = "HTTP/1.0 200 OK\r\n";
    string servName = "Server:simple web server\r\n";
    FILE* sendFile;
    stringstream ss;
    ss << "Content-type: " << ct << "\r\n";
    string cntType {ss.str()};
    cout << "cntType: " << cntType << endl;
    if ((sendFile = fopen(fileName.c_str(), "r")) == nullptr)
    {
        SendErrorMSG(sock);
        return;
    }
    // 传输头消息
    sock.sendHeader(protocol);
    sock.sendHeader(servName);
    sock.sendHeader(cntType);
    // sock.sendHeader("Transfer-Encoding: chunked\r\n");

    // 传输空行
    sock.sendEmptyLine();

    // 传输请求数据
    sock.sendFile(fileName);
    sock.sendEmptyLine();
    // shutdown(sock.getSocket(), 0);
}

void SendErrorMSG(RequestMethod::HttpRequest& sock)
{
    using std::string, std::stringstream;
    string protocol = "HTTP/1.0 400 Bad Request\r\n";
    string servName = "Server:simple web server\r\n";
    string cntType = "Content-type:text/html\r\n";
    string content = "<html><head><title>NETWORK</title></head><body><font size=+5><br> \
error request</font></body></html>";
    stringstream ss;
    ss << "Content-length:" << content.length() << "\r\n";
    string cntLen {ss.str()};

    // 传输头消息
    sock.sendHeader(protocol);
    sock.sendHeader(servName);
    sock.sendHeader(cntLen);
    sock.sendHeader(cntType);

    // 传输空行
    sock.sendEmptyLine();
    sock.send(content);
    sock.sendEmptyLine();
}

void ErrorHandling(const std::string& message)
{
    cout << "Error: << " << message << endl;
    exit(0);
}
