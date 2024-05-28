#include "../inc/Receive.hpp"
#include <iostream>
#include <fstream>

Receive::Receive() : buffer(""), request(""), body(""), isbody(false), maxSize(0), sizeSent(0), bodyStart(-1)
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

void Receive::setBuffer(std::string buffer)
{
    this->buffer = buffer;
}

std::string Receive::getBuffer()
{
    return this->buffer;
}

void Receive::setRequest(std::string request)
{
    this->request = request;
}

std::string Receive::getRequest()
{
    return this->request;
}

void Receive::setBody(std::string body)
{
    this->body = body;
}

std::string Receive::getBody()
{
    return this->body;
}

bool Receive::receive(int fd)
{
    if (!this->isbody)
    {
        return this->receiveHeader(fd);
    }
    else
    {
        this->file.open("/Users/josephcheel/GITHUB/42-Webserv/file.png", std::ios::out | std::ios::binary | std::ios::app);
        return this->receiveBody(fd);
    }
    return false;
}

bool Receive::receiveHeader(int fd)
{
    std::cout << "Receive::receive" << std::endl;
    char buf[MAX_MSG_SIZE] = {0};
    int ret;
    std::memset(buf, 0, MAX_MSG_SIZE);
    this->buffer = "";
    ret = recv(fd, buf, MAX_MSG_SIZE, 0);
    std::cout << "ret: " << ret << std::endl;
    if (ret < 0)
    {
        std::cerr << "Failed to read from client" << std::endl;
        return false;
    }
    else if (ret == 0)
    {
        std::cerr << "Client disconnected" << std::endl;
        return true;
    }
    if (ret)
    {
        this->buffer.append(buf, ret);
        if (buffer.find("\r\n\r\n") != std::string::npos)
        {
            request += this->buffer.substr(0, this->buffer.find("\r\n\r\n") );
            std::cout << "request: " << request << std::endl;
            if (request.find("Content-Length: ") != std::string::npos)
            {
                std::string contentLength = request.substr(request.find("Content-Length: ") + 16, request.find("\r\n", request.find("Content-Length: ")));
                this->maxSize = std::stoi(contentLength) + request.size();
            }
            else
                return true;
            body = this->buffer.substr(this->buffer.find("\r\n\r\n") + 4, this->buffer.at(this->buffer.size() - 1));
            std::cout << "body restas: " << body << std::endl;
            this->isbody = true;
            return false;
        }
        else
        {
            std::cout << "IS NOT BODY" << std::endl;
            request += this->buffer;
        }
    }
    return false;
}

bool Receive::receiveBody(int fd)
{
    // std::cout << "Receive::receiveBody" << std::endl;
    char buf[MAX_MSG_SIZE] = {0};
    this->buffer = "";
    // this->sizeSent = this->body.size();
    int ret = recv(fd, buf, MAX_MSG_SIZE, 0);
    if (ret < 0)
    {
        std::cerr << "Failed to read from client" << std::endl;
        return false;
    }
    else if (ret == 0)
    {
        std::cerr << "Client disconnected" << std::endl;
        return true;
    }
    if (ret)
    {
        this->buffer.append(buf, ret);
        if (this->sizeSent + ret >= this->maxSize)
        {
            if (bodyStart == -1)
            {
                this->body = "";
                bodyStart = this->buffer.at(this->buffer.find_first_of("\n")) - this->buffer.at(0);
                this->body = this->buffer.substr(0 + bodyStart);
                 bodyStart = 0;
            }
            else
                this->body += this->buffer.substr(0, this->maxSize - this->sizeSent);
            if (file.is_open())
            {
                file.write(this->body.c_str(), this->body.size());
                file.close();
            }
            else
            {
                std::cerr << "Failed to create file.png" << std::endl;
            }
            return true;
        }
        else
        {
            this->body += this->buffer;
        }
    }
    this->sizeSent += ret;
    return false;
}

void Receive::setSize(size_t size)
{
    this->maxSize = size;
}

