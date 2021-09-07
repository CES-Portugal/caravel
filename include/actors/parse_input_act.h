#ifndef PARSE_INPUT_ACT_H_
#define PARSE_INPUT_ACT_H_

#include <iostream>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "caf/all.hpp"

#include "./parse_simulation_act.h"

void parse_input(caf::event_based_actor* self);

#endif  // PARSE_INPUT_ACT_H_