#include "../inc/Receive.hpp"

Receive::Receive() : buffer(""), request(""), body(""), isbody(false), maxSize(0), sizeSent(0)
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

    std::cout << "Receive::receive" << std::endl;
    char buf[MAX_MSG_SIZE] = {0};
    int ret;
    std::memset(buf, 0, MAX_MSG_SIZE);
    this->buffer = "";
    ret = recv(fd, buf, MAX_MSG_SIZE, 0);

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
        sizeSent += ret;
        this->buffer += std::string(buf, ret);
        // std::cout << "buffer: " << this->buffer << std::endl;
        std::cout << "sizeSent: " << maxSize << std::endl;
        std::cout << "sent until now: " << sizeSent << std::endl;
    
        if (isbody == true)
        {
            std::cout << "ISREQUEST" << std::endl;
            body += this->buffer;
        }
        else
        {
            std::cout << "ISNOTREQUEST" << std::endl;
            request += this->buffer;
        }
        if (sizeSent >= maxSize)
        {
            std::cerr << "Max size reached" << std::endl;
            std::cout << "request: " << this->request << std::endl;
            return true;
        }
        if (buffer.find("\r\n\r\n") != std::string::npos)
        {
            request = this->buffer.substr(0, this->buffer.find("\r\n\r\n"));
            body = this->buffer.substr(this->buffer.find("\r\n\r\n") + 4);
            this->isbody = true;
            // return true;
        }    
    }
    return false;
}

void Receive::setSize(size_t size)
{
    this->maxSize = size;
}

