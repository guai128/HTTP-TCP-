#include "sendFile.h"

bool sendFile(SOCKET sock, int type, const std::string& buffer){
    std::string message {};
    message += "HTTP/1.1 200 OK\r\n";
    message += SERVER;
    message += formatChange(type);
    message += "content-length: ";
    message += std::to_string(buffer.length());
    message += "\r\n";
    
    message += "\r\n";

    if (-1 == send(sock, message.c_str(), message.length(), 0)){
        return false;
    }

    if (-1 == send(sock, buffer.c_str(), buffer.size(), 0)){
        return false;
    }

    return true;
}

bool sendFile(SOCKET sock, int type, const char* filename){
    //file process
    std::fstream file(const_cast<char *>(filename), std::ios::in | std::ios::binary);
    file.seekg(0, std::ios::beg);

    if (!file.is_open()) throw std::runtime_error(std::string("failed to open file ") + filename);

    std::string message {};
    message += "HTTP/1.1 200 OK\r\n";
    message += SERVER;
    message += formatChange(type);
    //message += "Connection: keep-alive\r\n";
    message += "Content-Length: ";
    message += std::to_string(getFileSize(filename));
    message += "\r\n";

    message += "\r\n";

    if (-1 == send(sock, message.c_str(), message.length(), 0)){
        return false;
    }

    char buffer[BUFFERSIZE];
    while(file.good()){
        file.read(buffer, BUFFERSIZE);
        send(sock, buffer, BUFFERSIZE, 0);
        memset(buffer, 0, BUFFERSIZE);
    }

    send(sock, ".", 2, 0);
    file.close();

    return true;
}

bool sendFile(SOCKET sock, const char* datagram){
    if (-1 == send(sock, datagram, sizeof(datagram), 0)){
        return false;
    }
    return true;
}

std::string formatChange(int type){
    switch (type)
    {
    case 0:
        return std::string("Content-Type: ") + " text/plain\r\n"                  ;
    case 1:
        return std::string("Content-Type: ") + " application/msword\r\n"          ;
    case 2:
        return std::string("Content-Type: ") + " application/pdf\r\n"             ;
    case 3:
        return std::string("Content-Type: ") + " application/zip\r\n"             ;
    case 4: 
        return std::string("Content-Type: ") + " image/png\r\n"                   ;
    case 5:
        return std::string("Content-Type: ") + " text/html\r\n"                   ;
    case 6:
        return std::string("Content-Type: ") + " image/jpeg\r\n"                  ;	
    case 7:
        return std::string("Content-Type: ") + " audio/wav\r\n"                   ;		
    case 8:
        return std::string("Content-Type: ") + " application/x-ppt\r\n"           ;		
    case 9:
    	return std::string("Content-Type: ") + " video/mpg\r\n"                   ;		
    default:
        return std::string("Content-Type: ") + " application/octet-stream\r\n"    ;
    }
}

int getFileSize(char* filename){
    struct _stat info;
    
    _stat(filename, &info);
    int len = info.st_size;

    return len;   
}

int getFileSize(std::string filename){
    return getFileSize(filename.c_str());
}

int getFileSize(const char* filename){
    struct _stat info;
    
    _stat(filename, &info);
    int len = info.st_size;

    return len;   
}

std::string nameExplain(const char* filename){
    std::string tmp(filename);
    std::string result {};
    for (auto p=tmp.end() - 1; *p != '.'; --p){
        result.insert(result.begin(), *p);
    }
    return result;
}