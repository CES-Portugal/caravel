#ifndef LOG_MSG_ACT_H_
#define LOG_MSG_ACT_H_

#include <iostream>
#include <time.h>

#include <linux/can.h>

#include "Packet.h"
#include "PcapFileDevice.h"

#include "caf/all.hpp"

#include "../utils.h"
#include "./inspector.h"

caf::behavior log_message(caf::event_based_actor* self);

#endif  // LOG_MSG_ACT_H_