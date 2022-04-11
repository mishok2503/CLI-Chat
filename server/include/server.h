#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

#include "user.h"

class Server {
public:
    using server = websocketpp::server<websocketpp::config::asio>;

private:
    using handle = websocketpp::connection_hdl;

    static std::vector<size_t> get_hash(std::string str);

    server endpoint;

    std::map<handle, User, std::owner_less<handle>> users;
    std::string path;
    std::fstream users_file;

    void on_message(handle hdl, server::message_ptr msg);
    void on_open(handle hdl);
    void on_close(handle hdl);

    std::string sign(User& usr, std::istream& is, bool sign_up);

public:

    Server(uint16_t port, std::string path = "data");

    void print_status(std::ostream& os);
};
