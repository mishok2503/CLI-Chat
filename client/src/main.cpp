#include <iostream>

#include "client.h"

int main() {
    Client c("ws://localhost:8239");
    c.send("sign-up mishok asd");
    sleep(1);
    return 0;
}
