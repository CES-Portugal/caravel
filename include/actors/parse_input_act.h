#ifndef PARSE_INPUT_ACT_H_
#define PARSE_INPUT_ACT_H_

#include <iostream>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "caf/all.hpp"

#include "./parse_simulation_act.h"

void parse_input(caf::event_based_actor* self, const caf::group& send_grp);

void parse_input_exit_handler(caf::scheduled_actor* self, const caf::group& grp, const int& skt);

#endif  // PARSE_INPUT_ACT_H_