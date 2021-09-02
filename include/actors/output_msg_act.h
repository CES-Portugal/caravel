#ifndef OUTPUT_MSG_ACT_H_
#define OUTPUT_MSG_ACT_H_

#include <iostream>

#include <linux/can.h>

#include "caf/all.hpp"

caf::behavior output_message(caf::event_based_actor* self);

#endif  // OUTPUT_MSG_ACT_H_