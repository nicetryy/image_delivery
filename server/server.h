#pragma once

#include <string>
#include <memory>

namespace server{

    class Server{
        
    public:

        Server(int port);

        std::string GetStartMessage();

        void SendImage(const std::string& filename);

        ~Server();

        
    private:

        struct Impl;
        std::unique_ptr<Impl> impl_;    

    };

} // namespace server