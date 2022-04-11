#include "server.h"

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <filesystem>

#include "user.h"

Server::Server(uint16_t port, std::string path) : path(std::move(path)) {
    this->path += '/';

    std::filesystem::create_directory(this->path);
    users_file.open(this->path + "users.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!users_file.is_open()) {
        std::cerr << "Can't open files by parh: " << this->path << std::endl;
        std::abort();
    }

    endpoint.set_error_channels(websocketpp::log::elevel::all);
    endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

    endpoint.init_asio();

    endpoint.set_open_handler(websocketpp::lib::bind(
        &Server::on_open,
        this,
        websocketpp::lib::placeholders::_1
    ));

    endpoint.set_close_handler(websocketpp::lib::bind(
        &Server::on_close,
        this,
        websocketpp::lib::placeholders::_1
    ));

    endpoint.set_message_handler(websocketpp::lib::bind(
        &Server::on_message,
        this,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2
    ));

    endpoint.listen(port);
    endpoint.start_accept();
    endpoint.run();
}


void Server::on_message(handle hdl, server::message_ptr msg) {
    const auto& txt = websocketpp::frame::opcode::text;
    if (msg->get_opcode() != txt) {
        endpoint.send(hdl, "erorr received a non-text message", txt);
        return;
    }

    std::stringstream ss(msg->get_payload());
    std::string cmd;
    ss >> cmd;

    auto send = [&](auto str) {
        endpoint.send(hdl, str, txt);
    };

    if (cmd == "sign-up") {
        send(sign(users[hdl], ss, true));
        return;
    }
    if (cmd == "sign-in") {
        send(sign(users[hdl], ss, false));
        return;
    }
    endpoint.send(hdl, "erorr unsupported command was received", txt);
}


void Server::on_open(handle hdl) {
    users[hdl] = User{"", {}, User::Status::CONNECTED};
}


void Server::on_close(handle hdl) {
    users.erase(hdl);
}

std::vector<size_t> Server::get_hash(std::string str) {
    std::vector<size_t> res;
    constexpr int block_size = 3;
    for (size_t i=0; i < str.size(); i += block_size) {
        res.push_back(std::hash<std::string>{}(str.substr(i, std::max(str.size(), i + block_size))));
    }
    return res;
}

std::string Server::sign(User& usr, std::istream& is, bool sign_up) {
    if (usr.status != User::Status::CONNECTED) {
        return "error invalid user status";
    }
    std::string nick, pswd;
    is >> nick >> pswd;
    auto p_hash = get_hash(std::move(pswd));
    if (!sign_up) {
        if (usr.nick != nick || usr.password_hash != p_hash) {
            return "response fail username or password is incorrect";
        }
    } else {
        users_file.seekg(0);
        bool exists = false;
        for (std::string line; !exists && (users_file >> line); ) {
            exists = line == nick;
            std::cerr << line << '\n';
        }
        if (exists) {
            return "response fail user already exists";
        }
        usr.nick = std::move(nick);
        usr.password_hash = std::move(p_hash);
        users_file.seekp(0,  std::ios_base::end);
        users_file << usr.nick << std::endl;
    }
    usr.status = User::Status::SIGNED;
    return "response success";
}
