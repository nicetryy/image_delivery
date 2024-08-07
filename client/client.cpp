#include "../pch.h"
#include "client.h"

#define BUF_SIZE 1024
#define END_OF_FILE "EOF"

#define DEBUG

namespace client{

    struct Client::Impl{

        Impl(int port) : global_port_(port){

            CreateSocket();
            std::cerr << "Socket was created.\n";
            //добавить возможность создания кастомных сообщений?
            ConnectToServer();

        }
        
    #ifdef DEBUG

        void SaveImage(const std::string& filename) {

            bool eof_received = false;
            char buffer[BUF_SIZE];
            
            std::ofstream file;
            OpenFile(file, filename);

            int count = 0;
            while (true) {
                tv.tv_sec = 3;
                tv.tv_usec = 0;

                FD_ZERO(&readfds);
                FD_SET(sockfd_, &readfds);

                int retval = select(sockfd_ + 1, &readfds, NULL, NULL, &tv);

                if (retval == -1) {
                    perror("select()");
                    break;
                } 
                else if (retval == 0) {
                    std::cerr << "Timeout occurred! No data after 3 seconds.\n";
                    if (!eof_received) {
                        std::cerr << "Data reception incomplete, exiting.\n";
                    }
                    break;
                } 
                else {
                    socklen_t len = sizeof(servaddr_);
                    ssize_t n = recvfrom(sockfd_, buffer, BUF_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr_, &len);
                    if (n < 0) {
                        perror("recvfrom failed");
                        break;
                    }

                    if (n == strlen(END_OF_FILE) && strncmp(buffer, END_OF_FILE, strlen(END_OF_FILE)) == 0) {
                        std::cerr << "End of file signal received.\n";
                        eof_received = true;
                        break;
                    }

                    SaveImageChunk(file, buffer, n);
                    ++count;
                    std::cerr << "Chunk " << count << " received with size " << n << " bytes.\n";
                }
            }

            file.close();
        }

    #else

        void SaveImage(const std::string& filename) {

            bool eof_received = false;
            char buffer[BUF_SIZE];
            
            std::ofstream file;
            OpenFile(file, filename);

            while (true) {
                tv.tv_sec = 3;
                tv.tv_usec = 0;

                FD_ZERO(&readfds);
                FD_SET(sockfd_, &readfds);

                int retval = select(sockfd_ + 1, &readfds, NULL, NULL, &tv);

                if (retval == -1) {
                    perror("select()");
                    break;
                } 
                else if (retval == 0) {
                    std::cerr << "Timeout occurred! No data after 3 seconds.\n";
                    if (!eof_received) {
                        std::cerr << "Data reception incomplete, exiting.\n";
                    }
                    break;
                } 
                else {
                    socklen_t len = sizeof(servaddr_);
                    ssize_t n = recvfrom(sockfd_, buffer, BUF_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr_, &len);
                    if (n < 0) {
                        perror("recvfrom failed");
                        break;
                    }

                    if (n == strlen(END_OF_FILE) && strncmp(buffer, END_OF_FILE, strlen(END_OF_FILE)) == 0) {
                        std::cerr << "End of file signal received.\n";
                        eof_received = true;
                        break;
                    }

                    SaveImageChunk(file, buffer, n);
                    ++count;
                }
            }

            file.close();
        }


    #endif

        void CreateSocket(){

            if ((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("Socket creation failed");
                exit(EXIT_FAILURE);
            }
        }

        void ConnectToServer(){

            InitializeServerAddress();

            std::cerr << "Serever address was initialized.\n";

            SendConectionMessage();
        }

        void InitializeServerAddress(const std::string& server_ip = "127.0.0.1"){
            
            memset(&servaddr_, 0, sizeof(servaddr_));
            
            servaddr_.sin_family = AF_INET;
            servaddr_.sin_port = htons(global_port_);
            servaddr_.sin_addr.s_addr = inet_addr(server_ip.data());
        }

        void SendConectionMessage(){

            const std::string init_msg = "Client ready to receive image";
            std::cerr << "Client send this conection message: " << init_msg << "\n";
            sendto(sockfd_, init_msg.data(), init_msg.size(), MSG_CONFIRM, (const struct sockaddr *)&servaddr_, sizeof(servaddr_));

        }

        void OpenFile(std::ofstream& file, const std::string& save_image_path){

            file.open(save_image_path, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Unable to open file to save image.\n";
                close(sockfd_);
                exit(EXIT_FAILURE);
            }

        }

        void SaveImageChunk(std::ofstream &file, const char* data, size_t size) {

            if (file.is_open()) {
                file.write(data, size);
            } 
            else {
                std::cerr << "Unable to open file to save image.\n";
            }

        }


        ~Impl(){
            close(sockfd_);
        }

    private:

        int global_port_;
        int sockfd_;
        struct sockaddr_in servaddr_;

        struct timeval tv;
        fd_set readfds;

    };

    Client::Client(int port) : impl_(std::make_unique<Impl>(port)){

    }

    void Client::SaveImage(const std::string& filename) {
        impl_->SaveImage(filename);
    }

    Client::~Client() = default;

}// namespace client

