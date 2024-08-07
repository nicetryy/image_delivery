#include "server.h"

int main() {
    server::Server server(PORT);

    std::string path_to_image = "/home/vadim/image_delivery/source_img/bladerunner82.jpg";

    std::string message = server.GetStartMessage();
    std::cerr << message << "\n";
    
    server.SendImage(path_to_image);

    return 0;
}