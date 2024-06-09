#include "../inc/Receive.hpp"
#include <iostream>
#include <fstream>

Receive::Receive() : buffer(""), request(""), body(""), isbody(false), maxSize(0), sizeSent(0), isform(false)
{
    std::cerr << "Receive created" << std::endl;
}

Receive::~Receive()
{
    std::cerr << "Receive destroyed" << std::endl;
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
    this->buffer.clear();
    ret = recv(fd, buf, MAX_MSG_SIZE, 0);
    if (ret < 0)
    {
        std::cerr << "Failed to read from client" << std::endl;
        return false;
    }
    else if (ret == 0)
    {
        std::cerr << "Client disconnected 1" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "Received from fd = " << fd << std::endl;
    }
    if (ret)
    {
        this->buffer.append(buf, ret);
        if (buffer.find("\r\n\r\n") != std::string::npos)
        {
            request += this->buffer.substr(0, this->buffer.find("\r\n\r\n"));
            if (request.find("Content-Length: ") != std::string::npos)
            {
                std::string contentLength = request.substr(request.find("Content-Length: ") + 16, request.find("\r\n", request.find("Content-Length: ")));
                this->maxSize = std::stoi(contentLength);
            }
            else
                return true;
            this->body = this->buffer.substr(this->buffer.find("\r\n\r\n") + 4, this->buffer.at(this->buffer.size() - 1));
            this->sizeSent += this->body.size();
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
            request += this->buffer;
    }
    return false;
}

bool Receive::receiveBody(int fd)
{
    char buf[MAX_MSG_SIZE] = {0};
    this->buffer.clear();
    int ret = recv(fd, buf, MAX_MSG_SIZE, 0);
    if (ret < 0)
    {
        std::cerr << "Failed to read from client" << std::endl;
        return false;
    }
    else if (ret == 0)
    {
        std::cerr << "Client disconnected 2" << std::endl;
        return true;
    }
    if (ret)
    {
        this->sizeSent += ret;
        this->buffer = std::string(buf, ret);
        if (this->sizeSent >= this->maxSize)
        {
            this->body += this->buffer;
            this->isbody = false;
            return true;
        }
        else
            this->body += this->buffer;
    }
    return false;
}
