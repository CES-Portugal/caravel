#ifndef PARSE_INPUT_ACT_H_
#define PARSE_INPUT_ACT_H_

#include <iostream>

#include "caf/all.hpp"

#include "./parse_file_act.h"
#include "./parse_simulate_act.h"
#include "./receive_msg_act.h"
#include "./exit_act.h"

void parse_input(caf::event_based_actor* self, const caf::group& rcv_grp, const caf::group& send_grp, const int& skt);

void parse_input_exit_handler(caf::scheduled_actor* self, const caf::group& grp, const int& skt);

#endif  // PARSE_INPUT_ACT_H_