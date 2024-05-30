#ifndef USERS_H
#define USERS_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <vector>

class Users {
  std::vector<std::string> users;
  double p_message, p_user;
  int client_socket;
  sockaddr_in server_address;
  bool active;

public:
  Users(double p_message, double p_user, int num_users, sockaddr_in address,
        int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
  Users(double p_message, double p_user, sockaddr_in address,
        int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
  ~Users();
  void send_message(int src_id, int dest_id, std::string message);
  void create_connection();
  void close_connection();
  void create_user(std::string name = "");
  void simulate();
};

#endif // USERS_H
