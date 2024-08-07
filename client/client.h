#pragma once

#include <string>
#include <memory>

namespace client{

    class Client{
    
    public:

        Client(int port);

        void SaveImage(const std::string& filename);

        ~Client();
        
    private:

        struct Impl;
        std::unique_ptr<Impl> impl_;    

    };

}// namespace client

