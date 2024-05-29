#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

class Users {
    std::vector<std::string> users;
    double p_message, p_user;
    int client_socket;
    sockaddr_in server_address;
public:
    Users(double p_message, double p_user, int num_users);
    Users(double p_message, double p_user);
    void send_message(int src_id, int dest_id, std::string message);
    void create_connection();
    void close_connection();
    void create_user(std::string name = "");
    void simulate();
};

#endif // USERS_H
