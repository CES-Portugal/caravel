#ifndef RECEIVE_MSG_ACT_H_
#define RECEIVE_MSG_ACT_H_

#include <iostream>
#include <unistd.h>
#include <errno.h>

#include <linux/can.h>

#include "caf/all.hpp"

#include "./log_msg_act.h"
#include "./output_msg_act.h"

void receive_msg(caf::event_based_actor* self, const int& skt, const int& interval);

#endif  // RECEIVE_MSG_ACT_H_