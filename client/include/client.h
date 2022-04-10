#pragma once

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <string>
#include <iostream>

class ConnectionData;

class Client {
public:
    using client = websocketpp::client<websocketpp::config::asio_client>;

private:

    client endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread;
    websocketpp::lib::shared_ptr<ConnectionData> connection_data;

    bool connect(const std::string& uri);
    void close(websocketpp::close::status::value code);

public:

    Client(const std::string& uri);

    void print_status(std::ostream& os);

    ~Client();
};
