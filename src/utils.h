#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string encode_message(int src_id, int dest_id, std::string msg);
std::string decode_message(std::string msg, int &src_id, int &dest_id);
void sleep_max_ms(int max_ms);

#endif // UTILS_H
