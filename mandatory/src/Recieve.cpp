#include "../inc/Receive.hpp"

Receive::Receive() : buffer(""), request(""), body(""), isrequest(false)
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
    buffer = "";
    ret = recv(fd, buf, MAX_MSG_SIZE, 0);
    std::cout << "buff " << buf << std::endl;
    std::string buffer(buf, ret);
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
    if (isrequest == true)
    {
        body += buffer;
    }
    else if (buffer.find("\r\n\r\n") != std::string::npos)
    {
        request = buffer.substr(0, buffer.find("\r\n\r\n"));
        body = buffer.substr(buffer.find("\r\n\r\n") + 4);
        this->isrequest = true;
        // return true;
    }    
    return false;
}