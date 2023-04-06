#include "head.h"

#define CHAR  0
#define DOCX  1
#define PDF   2
#define ZIP   3
#define PNG   4
#define HTML  5
#define JPG   6
#define AUDIO 7
#define PPT   8
#define MPV   9

#define SERVER "Server: dsb 1.0\r\n"
#define my_ip "172.28.162.24"
#define my_host 80
#define BUFFERSIZE 10240

/*
    directly send message and existed datagram to https
*/
bool sendFile(SOCKET sock, int type, std::string& buffer);
bool sendFile(SOCKET sock, int type, const char* filename);
/*
    send the datagram to https
*/
bool sendFile(SOCKET sock, const char* datagram);

std::string formatChange(int type);

int getFileSize(char* filename);
int getFileSize(std::string filename);
int getFileSize(const char* filename);

std::string nameExplain(const char* filename);