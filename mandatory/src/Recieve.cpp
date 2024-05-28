#include "../inc/Receive.hpp"
#include <iostream>
#include <fstream>

Receive::Receive() : buffer(""), request(""), body(""), isbody(false), maxSize(0), sizeSent(0), bodyStart(-1), isform(false)
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
                // if (request.find("Content-Type: multipart/form-data; boundary=") != std::string::npos)
                // {
                //     this->boundary = request.substr(request.find("boundary=") + 9, request.find("\r\n", request.find("boundary=")) - 9);
                //     std::cout << "boundary: " << boundary << std::endl;
                // }
                std::string contentLength = request.substr(request.find("Content-Length: ") + 16, request.find("\r\n", request.find("Content-Length: ")));
                this->maxSize = std::stoi(contentLength); //+ request.size();
                std::cout << "maxSize: " << this->maxSize << std::endl;
            }
            else
                return true;
            this->body = this->buffer.substr(this->buffer.find("\r\n\r\n") + 4, this->buffer.at(this->buffer.size() - 1));
            std::cout << "body restas: $" << body << "$" << std::endl;
            std::cout << "body size: " << body.size() << std::endl;
            std::cout << "size sent: " << this->sizeSent << std::endl;
            std::cout << "maxSize: " << this->maxSize << std::endl;
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
        {
            std::cout << "IS NOT BODY" << std::endl;
            request += this->buffer;
        }
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
        std::cerr << "Client disconnected" << std::endl;
        return true;
    }
    if (ret)
    {
        this->sizeSent += ret;
        this->buffer = std::string(buf, ret);
        // std::cout << "buffer: $" << this->buffer << "$" << std::endl;
        if (this->sizeSent >= this->maxSize)
        {
            (void)bodyStart;
            this->body += this->buffer;
            // if (bodyStart == -1)
            // {
            //     // std::cout << "HOLA BODYSTART" << std::endl;
            //     // if (body.empty())
            //     //     this->body.clear();
            //     // bodyStart = this->buffer.at(this->buffer.find_first_of("\n"));
            //     // this->body += this->buffer.substr(0 + bodyStart);
            //     this->body += this->buffer;
            //     bodyStart = 0;
            // }
            // else
                // this->body += this->buffer.substr(0, this->maxSize - this->sizeSent);
            this->isbody = false;
            return true;
        }
        else
        {
            this->body += this->buffer;
        }
    }
    std::cout << "sizeSent: " << this->sizeSent << std::endl;
    std::cout << "maxSize: " << this->maxSize << std::endl;
    return false;
}

void Receive::setSize(size_t size)
{
    this->maxSize = size;
}


bool Receive::getisform()
{
    return this->isform;
}