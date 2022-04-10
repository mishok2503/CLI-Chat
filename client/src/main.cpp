#include <iostream>

#include "client.h"

int main() {
    std::cerr << "Start\n";

    Client c("ws://localhost:9002");
    //Client c("asd");
    //Client c("ws://echo.websocket.events");
sleep(1);
c.print_status(std::cout);

    std::cerr << "End\n";
    return 0;
}
