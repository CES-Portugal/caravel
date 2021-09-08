#ifndef PARSE_INPUT_ACT_H_
#define PARSE_INPUT_ACT_H_

#include <iostream>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "../utils.h"
#include "./cyclic_msg_act.h"
#include "./receive_msg_act.h"
#include "./exit_act.h"
#include "./parse_simulation_act.h"
#include "./receive_input_act.h"

caf::behavior parse_input(caf::event_based_actor* self);

#endif  // PARSE_INPUT_ACT_H_