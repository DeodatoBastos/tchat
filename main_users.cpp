#include "users.h"

int main() {
    Users client(0.9, 0.05, 10);
    client.simulate();
    return 0;
}
