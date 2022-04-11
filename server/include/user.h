#pragma once

#include <string>
#include <vector>

struct User {
    enum Status {
        CONNECTED, SIGNED
    };

    std::string nick;
    std::vector<size_t> password_hash;

    Status status;
};
