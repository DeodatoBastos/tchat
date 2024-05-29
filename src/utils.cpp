#include "utils.h"
#include <string>


std::string encode_message(int src_id, int dest_id, std::string msg) {
    return std::to_string(src_id) + "." + std::to_string(dest_id) + "." + msg;
}

std::string decode_message(std::string msg, int& src_id, int& dest_id) {
    int idx_src = msg.find(".");
    src_id = std::stoi(msg.substr(0, idx_src));
    int idx_dest = msg.find(".", idx_src+1);
    dest_id = std::stoi(msg.substr(idx_src+1, idx_dest-1));
    return msg.substr(idx_dest+1, msg.size()-idx_dest);
}
