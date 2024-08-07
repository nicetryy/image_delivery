#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024
#define END_OF_FILE "EOF"

#define DEBUG

namespace server{

    class Server{
        
    public:

        Server(int port);

        std::string GetStartMessage();

    #ifdef DEBUG

        void SendImage(const std::string& filename);

    #else

        void SendImage(const std::string& filename);

    #endif

        ~Server();

        
    private:

        int global_port_;
        int sockfd_;
        struct sockaddr_in servaddr_, cliaddr_;

        void CreateSocket();

        void InitializeAddress();

        void BindingSocketToAddress();
    };

} // namespace server