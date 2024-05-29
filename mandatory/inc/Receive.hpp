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
        std::ofstream file;
        std::string boundary;
        bool isform;

    public:
        Receive();
        ~Receive();
        Receive(Receive const &copy);
        Receive &operator=(Receive const &copy);
        
        bool receiveHeader(int fd);
        bool receiveBody(int fd);
        bool receive(int fd);
        void        setBuffer(std::string buffer);
        void        setRequest(std::string request);
        void        setBody(std::string body);
        void        setmaxSize(size_t size);
        
        std::string getBuffer();
        std::string getRequest();
        std::string getBody();
        bool        getisform();
};