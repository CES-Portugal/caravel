#ifndef PARSE_SIMULATION_ACT_H_
#define PARSE_SIMULATION_ACT_H_

#include <iostream>
#include <fstream>
#include <string>

#include "../utils.h"
#include "./cyclic_msg_act.h"
#include "./receive_msg_act.h"
#include "./exit_act.h"

caf::behavior parse_simulation(caf::event_based_actor* self, const int& skt);

#endif  // PARSE_SIMULATION_ACT_H_