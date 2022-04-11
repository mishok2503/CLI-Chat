#include <iostream>

#include "client.h"

int main() {
    Client c("ws://localhost:9002");
    //Client c("asd");
    //Client c("ws://echo.websocket.events");
    //c.print_status(std::cout);
//
//    if (!c.is_connected()) {
//        return 1;
//    }

    c.send("ASD");
    int a;
std::cin >> a;
    return 0;
}
