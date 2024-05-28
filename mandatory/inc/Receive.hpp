#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>

#define MAX_MSG_SIZE 1024
class Receive {
    private:
        std::string buffer;
        std::string request;
        std::string body;
        bool isbody;
        size_t maxSize;
        size_t sizeSent;
        int bodyStart;
        std::ofstream file;
        std::string boundary;
        bool isform;

    public:
        Receive();
        ~Receive();
        Receive(Receive const &copy);
        Receive &operator=(Receive const &copy);
        void        setBuffer(std::string buffer);
        std::string getBuffer();
        void        setRequest(std::string request);
        std::string getRequest();
        void        setBody(std::string body);
        std::string getBody();
        bool receive(int fd);
        bool receiveHeader(int fd);
        bool receiveBody(int fd);
        void setSize(size_t size);
        bool getisform();
};