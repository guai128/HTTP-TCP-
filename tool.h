//
// Created by 25382 on 2023/11/22.
//

#ifndef HTTP_TOOL_H
#define HTTP_TOOL_H

#include "Request.h"


std::string ContentType(const std::string&  file);
void SendData(RequestMethod::HttpRequest& sock, const std::string& ct, std::string& fileName);
void SendErrorMSG(RequestMethod::HttpRequest& sock);
void ErrorHandling(const std::string& message);


namespace RequestMethod
{
    unsigned RequestHandler(void* arg);
    int GETHandler(HttpRequest& hClnt, std::stringstream& ss);
    int POSTHandler(HttpRequest& hClnt, std::stringstream& ss);
    int HEADHandler(HttpRequest& hClnt, std::stringstream& ss);
    // 目前仅考虑http1.0所有方法，日后扩展
}


#endif //HTTP_TOOL_H
