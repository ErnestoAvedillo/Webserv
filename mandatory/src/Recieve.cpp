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
std::string Receive::getPostHeader() { return this->postHeader; }
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
        std::string tmp = request + this->buffer;
        if (tmp.find("\r\n\r\n") != std::string::npos || tmp.find("\n\n") != std::string::npos)
        {
            request = tmp.substr(0, tmp.find("\r\n\r\n"));
            Header header(request);
            std::string log = CHR_BLUE + header.getMethod() + RESET + "\t" + header.getAttribute("Host") + CHR_CYAN + header.getPath() + RESET;
            printLog("NOTICE", log);
            if (header.getAttribute("Content-Length") != "")
                this->maxSize = std::atoi(header.getAttribute("Content-Length").c_str());
            if (header.getAttribute("Content-Type") != "")
            {
                if (header.getAttribute("Content-Type").find("boundary") != std::string::npos)
                    this->boundary = header.getAttribute("Content-Type").substr(header.getAttribute("Content-Type").find("boundary=") + 9);
                // std::cout << "BOUNDARY $" << this->boundary << "$" << std::endl;
            }
            else
                return true;
            this->body = tmp.substr(tmp.find("\r\n\r\n") + 4, tmp.size() - tmp.find("\r\n\r\n") - 4);
            this->sizeSent += this->body.size();
            if (this->sizeSent >= this->maxSize)
            {
                if (this->boundary.empty())
                    this->isform = true;
                else
                {
                     this->body += this->buffer.substr(0, this->buffer.rfind(this->boundary) - 4);
                    this->body = this->body.substr(0, this->body.find(this->boundary) - 4);
                    this->postHeader = this->body.substr(this->body.find(this->boundary) + this->boundary.size() + 4, this->body.find("\r\n\r\n"));
                    this->body = this->body.substr(this->body.find("\r\n\r\n") + 4);
                }
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
        printLog("NOTICE", "Client disconnected");
        return false;
    }
    return false;
}

bool Receive::receiveBody(int fd)
{
    char buf[MAX_MSG_SIZE] = {0};
    int ret = 0;

    while ((ret = recv(fd, buf, MAX_MSG_SIZE, 0)) > 0)
    {
        this->sizeSent += ret;
        this->buffer.clear();
        this->buffer = std::string(buf, ret);
        if (this->sizeSent >= this->maxSize)
        {
            if (!this->boundary.empty())
            {
                this->body += this->buffer.substr(0, this->buffer.rfind(this->boundary) - 4);
                this->body = this->body.substr(0, this->body.find(this->boundary) - 4);
                this->postHeader = this->body.substr(this->body.find(this->boundary) + this->boundary.size() + 4, this->body.find("\r\n\r\n"));
                this->body = this->body.substr(this->body.find("\r\n\r\n") + 4);
            }
            else
                this->body += this->buffer;
            this->isbody = false;
            return true;
        }
        else
            this->body += this->buffer;
        std::memset(buf, 0, MAX_MSG_SIZE); 
    }
    if (ret < 0) // This is not handle as an error 
        return false;
    else if (ret == 0)
    {
        printLog("NOTICE", "Client disconnected");
        return false;
    }
    return false;
}
