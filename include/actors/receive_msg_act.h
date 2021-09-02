#ifndef RECEIVE_MSG_ACT_H_
#define RECEIVE_MSG_ACT_H_

#include <iostream>
#include <unistd.h>

#include <linux/can.h>

#include "caf/all.hpp"

void receive_msg(caf::event_based_actor* self, const int& skt, const caf::group& grp);

#endif  // RECEIVE_MSG_ACT_H_