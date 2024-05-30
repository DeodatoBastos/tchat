#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <vector>

class Server {
  std::vector<std::string> users;
  int server_socket;
  sockaddr_in server_address;
  int n_requests;
  bool active;

public:
  Server(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0,
         int port = 8080, int addres = INADDR_ANY, int num_requests = 5);
  ~Server();
  void listen_client();
  void close_connection();
  void handle_message();
};

#endif // SERVER_H
