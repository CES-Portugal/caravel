#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <chrono>
#include <math.h>
#include <sstream>
#include <string.h>

#include <linux/can.h>

std::string hex_to_ascii(const std::string& hex);

bool valid_hex_str(const std::string& str);

void elapsed_time(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end,timespec& time);

int str_to_frame(const std::string& str, struct can_frame& frame);

void interval_from_str(const std::string& str, int& interval, const std::string& delimiter);

#endif  // UTILS_H_