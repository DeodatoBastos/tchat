#include "users.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Users::Users(double p_msg, double p_u, int n, int port) : p_message(p_msg), p_user(p_u), active(false) {
    users.reserve(n);
    for (int i = 0; i < n; i++) {
        users.push_back("user_" + std::to_string(i));
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    server_address = address;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

void Users::create_connection() {
    int err = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (err != 0) {
        perror("Unable to estabelish a connection. Try again later.");
        return;
    }

    active = true;
    std::cout << "starting simulation...\n";
}

void Users::close_connection() {
    int err = close(client_socket);

    if (err != 0) {
        perror("Unable to close the connection.");
        return;
    }

    active = false;
    std::cout << "ending simulation...\n";
}

void Users::send_message(int src_id, int dest_id, std::string message) {
    std::string encoded_msg = encode_message(src_id, dest_id, message);
    send(client_socket, encoded_msg.c_str(), encoded_msg.size(), 0);
}

void Users::handle_message() {
    int src_id, dest_id;
    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    std::string message(buffer);
    if (!message.empty()) {
        std::string msg = decode_message(message, src_id, dest_id);
        std::cout << "from: " << src_id << "; to: " << dest_id << "; msg: " << msg << "\n";
    }
}

void Users::create_user(std::string name) {
    if (name == "") {
        name = "user_" + std::to_string(users.size());
    }
    users.push_back(name);
}

void Users::simulate() {
    create_connection();
    int i = 0;
    while (true) {
        srand((unsigned)time(NULL));
        if ((double)std::rand() / RAND_MAX > p_message) {
            int src_id = std::rand() % users.size();
            int dest_id = std::rand() % users.size();
            send_message(src_id, dest_id, "hello there " + std::to_string(i++));
            handle_message();
        }
        if ((double)std::rand() / RAND_MAX > p_user) {
            std::string name = std::to_string(users.size()) + "_user";
            users.push_back(name);
        }
    }
    close_connection();
}

Users::~Users() {
    if (active)
        close_connection();
}
