#ifndef SEND_MSG_ACT_H_
#define SEND_MSG_ACT_H_

#include <iostream>
#include <unistd.h>

#include <linux/can.h>

#include "caf/all.hpp"

#include "./inspector.h"

caf::behavior send_message(caf::event_based_actor* self, const int& skt);

#endif  // SEND_MSG_ACT_H_