#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <chrono>
#include <math.h>

std::string hex_to_ascii(const std::string& hex);

bool valid_hex_str(const std::string& str);

void elapsed_time(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end,timespec& time);

#endif  // UTILS_H_