#ifndef LOG_MSG_ACT_H_
#define LOG_MSG_ACT_H_

#include <iostream>

#include <linux/can.h>

#include "Packet.h"
#include "PcapFileDevice.h"

#include "caf/all.hpp"

#include "../utils.h"

caf::behavior log_message(caf::event_based_actor* self, bool rcv_msg, const std::chrono::steady_clock::time_point& start);

#endif  // LOG_MSG_ACT_H_