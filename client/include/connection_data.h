#pragma once

#include <websocketpp/common/connection_hdl.hpp>

#include <string>
#include <iostream>

#include "client.h"

class ConnectionData {
public:
    
    static constexpr char ST_CONN[] = "Connecting";
    static constexpr char ST_FAIL[] = "Failed";
    static constexpr char ST_OPEN[] = "Open";
    static constexpr char ST_CLSD[] = "Closed";

    ConnectionData(websocketpp::connection_hdl hdl, std::string uri);
 
    void on_open(Client::client* client, websocketpp::connection_hdl hdl);
    void on_fail(Client::client* client, websocketpp::connection_hdl hdl);
    void on_close(Client::client* client, websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl, Client::client::message_ptr msg);
 
    websocketpp::connection_hdl get_hdl() const;
    bool is_connecting() const;

    friend std::ostream& operator<< (std::ostream& out, const ConnectionData& data);
private:
    websocketpp::connection_hdl hdl;
    std::string status;
    std::string uri;
    std::string server;
    std::string error_reason;
};
