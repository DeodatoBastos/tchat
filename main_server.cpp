#include <server.h>

int main() {
    Server server;
    server.listen_client();
    server.handle_message();
    return 0;
}
