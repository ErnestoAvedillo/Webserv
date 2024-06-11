#include "../inc/Receive.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "../inc/Header.hpp"

Receive::Receive() : buffer(""), request(""), body(""), isbody(false), maxSize(0), sizeSent(0), isform(false)
{
    // std::cerr << "Receive created" << std::endl;
}

Receive::~Receive()
{
    // std::cerr << "Receive destroyed" << std::endl;
}

Receive::Receive(Receive const &copy)
{
    *this = copy;
}

Receive &Receive::operator=(Receive const &copy)
{
    if (this != &copy)
    {
        this->buffer = copy.buffer;
        this->request = copy.request;
        this->body = copy.body;
    }
    return *this;
}

 /* Setters */
void Receive::setBuffer(std::string buffer) { this->buffer = buffer; }
void Receive::setRequest(std::string request) { this->request = request; }
void Receive::setBody(std::string body) { this->body = body; }
void Receive::setmaxSize(size_t size) { this->maxSize = size; }

 /* Getters */
std::string Receive::getBuffer() { return this->buffer; }
std::string Receive::getRequest() { return this->request; }
std::string Receive::getBody() { return this->body; }
bool Receive::getisform() { return this->isform; }

bool Receive::receive(int fd)
{
    if (!this->isbody)
        return this->receiveHeader(fd);
    else
        return this->receiveBody(fd);
    return false;
}

bool Receive::receiveHeader(int fd)
{
    char buf[MAX_MSG_SIZE] = {0};
    int ret;
   
    while ((ret = recv(fd, buf, MAX_MSG_SIZE, 0)) > 0)
    {
        this->buffer.clear();
        this->buffer.append(buf, ret);
        std::cout << "Buffer" << this->buffer.size() << std::endl;
        std::cout << "ret" << ret << std::endl;
        std::string tmp = request + this->buffer;
        if (tmp.find("\r\n\r\n") != std::string::npos || tmp.find("\n\n") != std::string::npos)
        {
            request += tmp.substr(0, tmp.find("\r\n\r\n"));
            std::cout << "Request" << request.size() << std::endl;
            Header header(request);
            std::map<std::string, std::string>  Attributes = header.getAttributes();

            std::string log = CHR_BLUE + header.getMethod() + RESET + "\t" + Attributes["Host"] + CHR_CYAN + header.getPath() + RESET;
            printLog("NOTICE", log);
            if (Attributes.find("Content-Length") != Attributes.end())
                 this->maxSize = std::atoi(Attributes["Content-Length"].c_str());
            else
                return true;
            this->body = tmp.substr(tmp.find("\r\n\r\n") + 4, tmp.size() - tmp.find("\r\n\r\n") - 4);
            std::cout << "TMP" << tmp.size() << std::endl;
            std::cout << "Body" << this->body.size() << std::endl;
            this->sizeSent += this->body.size();
            std::cout << "Maxsize" << this->maxSize << std::endl;
            std::cout << "SizeSent" << this->sizeSent << std::endl;
            if (this->sizeSent >= this->maxSize)
            {
                this->isform = true;
                this->isbody = false;
                return true;
            }
            this->isbody = true;
            return false;
        }
        else
            request += tmp;
        std::memset(buf, 0, MAX_MSG_SIZE);
    }
    
    if (ret < 0) // This is not handle as an error 
        return false;
    else if (ret == 0)
    {
        std::cerr << "Client disconnected" << std::endl;
        return false;
    }
    return false;
}

bool Receive::receiveBody(int fd)
{
    char buf[MAX_MSG_SIZE] = {0};
    int ret = 0;
    std::cout << "HOKA" << std::endl;
    while ((ret = recv(fd, buf, MAX_MSG_SIZE, 0)) > 0)
    {
        std::cout << "HOKA" << std::endl;
        this->sizeSent += ret;
        this->buffer.clear();
        this->buffer = std::string(buf, ret);
        if (this->sizeSent >= this->maxSize)
        {
            std::cout << "Maxsizeee" << this->maxSize << std::endl;
            std::cout << "SizeSenttt" << this->sizeSent << std::endl;
            this->body += this->buffer;
            this->isbody = false;
            return true;
        }
        else
            this->body += this->buffer;
        std::memset(buf, 0, MAX_MSG_SIZE); 
    }
    std::cout << "Maxsizeee" << this->maxSize << std::endl;
    std::cout << "SizeSenttt" << this->sizeSent << std::endl;
    std::cout << "Ret" << ret << std::endl;
    if (ret < 0) // This is not handle as an error 
        return false;
    else if (ret == 0)
    {
        std::cerr << "Client disconnected" << std::endl;
        return false;
    }
    return false;
}
