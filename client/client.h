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

namespace client{

    class Client{
    
    public:

        Client(int port);

    #ifdef DEBUG

        void SaveImage(const std::string& filename);

    #else

        void SaveImage(const std::string& filename);


    #endif

        ~Client();
        
    private:

        int global_port_;
        int sockfd_;
        struct sockaddr_in servaddr_;

        struct timeval tv;
        fd_set readfds;
        bool eof_received = false;

        void CreateSocket();

        void ConnectToServer();

        void InitializeServerAddress(const std::string& server_ip = "127.0.0.1");

        void SendConectionMessage();

        void OpenFile(std::ofstream& file, const std::string& save_image_path);

        void SaveImageChunk(std::ofstream &file, const char* data, size_t size);
    };

}// namespace client
