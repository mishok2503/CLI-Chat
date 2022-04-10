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

    connect(uri);
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

    endpoint.connect(con);

    return true;
}

void Client::close(websocketpp::close::status::value code) {
    websocketpp::lib::error_code ec;
    
    for (int i=0; connection_data->is_connecting(); ++i) {
        std::cerr << "> Waitng connection for close it" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (i == 3) {
            std::cerr << "Time out" << std::endl;
            std::abort();
        }
    }
        

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

Client::~Client() {
    endpoint.stop_perpetual();
    close(websocketpp::close::status::going_away);
    thread->join();
}
