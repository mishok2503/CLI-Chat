#include "client.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "connection_data.h"

Client::Client(const std::string& uri) {
    endpoint.clear_access_channels(websocketpp::log::alevel::all);
    endpoint.clear_error_channels(websocketpp::log::elevel::all);

    endpoint.init_asio();
    endpoint.start_perpetual();

    thread.reset(new websocketpp::lib::thread(&client::run, &endpoint));

    is_conn = connect(uri);
}

bool Client::connect(const std::string& uri) {
    websocketpp::lib::error_code ec;
    const auto& con = endpoint.get_connection(uri, ec);

    if (ec) {
        std::cerr << "> Connect initialization error: " << ec.message() << std::endl;
        return false;
    }

    connection_data = websocketpp::lib::make_shared<ConnectionData>(con->get_handle(), uri);

    con->set_open_handler(websocketpp::lib::bind(
        &ConnectionData::on_open,
        connection_data,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_fail_handler(websocketpp::lib::bind(
        &ConnectionData::on_fail,
        connection_data,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_close_handler(websocketpp::lib::bind(
        &ConnectionData::on_close,
        connection_data,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_message_handler(websocketpp::lib::bind(
        &ConnectionData::on_message,
        connection_data,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2
    ));

    endpoint.connect(con);

    std::cerr << "> Waitng connection" << std::endl;
    for (int i=0; connection_data->is_connecting(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (i == 20) {
            std::cerr << "> Time out" << std::endl;
            return false; 
        }
    }
    if (connection_data->get_status() == ConnectionData::ST_FAIL) {
        std::cerr << "> Connection fail\n";
        print_status(std::cerr);
        return false;
    }
    std::cerr << "> Conncted" << std::endl;
    return true;
}

void Client::send(const std::string& message) {
    websocketpp::lib::error_code ec;
    endpoint.send(connection_data->get_hdl(), message, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
    }
}

void Client::close(websocketpp::close::status::value code) {
    if (!is_connected()) {
        return;
    }

    websocketpp::lib::error_code ec;
    
    endpoint.close(connection_data->get_hdl(), code, "", ec);
    if (ec) {
        std::cerr << "> Error initiating close: " << ec.message() << std::endl;
    } else {
        std::cerr << "> Connection closed" << std::endl;
    }
}

void Client::print_status(std::ostream& os) {
    if (connection_data) {
        os << *connection_data << std::endl;
    } else {
        os << "> No connection" << std::endl;
    }
}

bool Client::is_connected() const {
    return is_conn;
}

Client::~Client() {
    endpoint.stop_perpetual();
    close(websocketpp::close::status::going_away);
    thread->join();
}
