#include "client.h"

int main() {

    client::Client client(PORT);

    std::string path_to_saved_image = "../client_img/received_image_bladerunner.jpg";
    client.SaveImage(path_to_saved_image);

    return 0;
}