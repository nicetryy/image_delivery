#include "../pch.h"
#include "server.h"

#define BUF_SIZE 1024
#define END_OF_FILE "EOF"

#define DEBUG

namespace server {

    struct Server::Impl{

        Impl(int port) : global_port_(port){

            CreateSocket();
            InitializeAddress();
            BindingSocketToAddress();
        }
        
        std::string GetStartMessage(){

            char buffer[BUF_SIZE];
            socklen_t len = sizeof(cliaddr_);
            recvfrom(sockfd_, buffer, BUF_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr_, &len);
            return (buffer);
            //RVO? std::move(std::string(buffer));
            // + "/0"?
        }

    #ifdef DEBUG

        void SendImage(const std::string& filename) {
            std::ifstream file(filename, std::ios::binary);
            if (file.is_open()) {
                char buffer[BUF_SIZE];
                int count = 0; 
                while (file.read(buffer, BUF_SIZE) || file.gcount() > 0) {
                    size_t size = file.gcount();
                    sendto(sockfd_, buffer, size, MSG_CONFIRM, (const struct sockaddr *)&cliaddr_, sizeof(cliaddr_));
                    ++count;
                    std::cerr << "Packet " << count << " sent with size " << size << " bytes.\n";
                }

                sendto(sockfd_, END_OF_FILE, strlen(END_OF_FILE), MSG_CONFIRM, (const struct sockaddr *)&cliaddr_, sizeof(cliaddr_));
                std::cout << "Image sent.\n";
                file.close();
            } 
            else {
                std::cerr << "Unable to open image file.\n";
            }
        }

    #else

        void SendImage(const std::string& filename) {
            std::ifstream file(filename, std::ios::binary);
            if (file.is_open()) {
                char buffer[BUF_SIZE];
                while (file.read(buffer, BUF_SIZE) || file.gcount() > 0) {
                    size_t size = file.gcount();
                    sendto(sockfd_, buffer, size, MSG_CONFIRM, (const struct sockaddr *)&cliaddr_, sizeof(cliaddr_));
                }

                sendto(sockfd_, END_OF_FILE, strlen(END_OF_FILE), MSG_CONFIRM, (const struct sockaddr *)&cliaddr_, sizeof(cliaddr_));
                std::cout << "Image sent.\n";
                file.close();
            } 
            else {
                std::cerr << "Unable to open image file.\n";
            }
        }

    #endif
    
        void CreateSocket(){

            if ((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("Socket creation failed");
                exit(EXIT_FAILURE);
            }
        }

        void InitializeAddress(){
            
            memset(&servaddr_, 0, sizeof(servaddr_));
            memset(&cliaddr_, 0, sizeof(cliaddr_));

            servaddr_.sin_family = AF_INET;
            servaddr_.sin_addr.s_addr = INADDR_ANY;
            servaddr_.sin_port = htons(global_port_);

        }

        void BindingSocketToAddress(){
            
            if (bind(sockfd_, (const struct sockaddr *)&servaddr_, sizeof(servaddr_)) < 0) {
                perror("Bind failed");
                close(sockfd_);
                exit(EXIT_FAILURE);
            }

        }

        ~Impl(){
            close(sockfd_);
        }

    private:

        int global_port_;
        int sockfd_;
        struct sockaddr_in servaddr_, cliaddr_;

    };
    

    Server::Server(int port) : impl_(std::make_unique<Impl>(port)){

    }

    std::string Server::GetStartMessage(){
        return impl_->GetStartMessage();
    }

    void Server::SendImage(const std::string& filename) {
        impl_->SendImage(filename);
    }

    Server::~Server() = default;

} //namespace server

