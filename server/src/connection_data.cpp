#include "connection_data.h"

#include <iostream>
#include <sstream>

#include "client.h"

namespace {
    constexpr char RH_SERER[] = "Server";
}

ConnectionData::ConnectionData(websocketpp::connection_hdl hdl, std::string uri)
      : hdl(hdl), status(ST_CONN), uri(uri), server("N/A") {}

void ConnectionData::on_open(Client::client* client, websocketpp::connection_hdl hdl) {
    status = ST_OPEN;
    const auto& con = client->get_con_from_hdl(hdl);
    server = con->get_response_header(RH_SERER);
}

void ConnectionData::on_fail(Client::client* client, websocketpp::connection_hdl hdl) {
    status = ST_FAIL;
    const auto& con = client->get_con_from_hdl(hdl);
    server = con->get_response_header(RH_SERER);
    error_reason = con->get_ec().message();
}

void ConnectionData::on_close(Client::client* client, websocketpp::connection_hdl hdl) {
    status = ST_CLSD;
    const auto& con = client->get_con_from_hdl(hdl);
    std::stringstream ss;
    ss << "close code: " << con->get_remote_close_code() << " (" 
      << websocketpp::close::status::get_string(con->get_remote_close_code()) 
      << "), close reason: " << con->get_remote_close_reason();
    error_reason = ss.str();
}

void ConnectionData::on_message(websocketpp::connection_hdl, Client::client::message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {
        std::cout << "< " << msg->get_payload() << std::endl;
    } else {
        std::cerr << "> Get non text message";
    }
}

websocketpp::connection_hdl ConnectionData::get_hdl() const {
    return hdl;
}

bool ConnectionData::is_connecting() const {
    return status == ST_CONN;
}

std::ostream& operator<< (std::ostream& os, const ConnectionData& data) {
    os << "> URI: " << data.uri << "\n"
        << "> Status: " << data.status << "\n"
        << "> Remote Server: " << (data.server.empty() ? "None Specified" : data.server) << "\n";
    if (!data.error_reason.empty()) {
        os << "> Error/close reason: " << data.error_reason;
    } 

    return os;
}
