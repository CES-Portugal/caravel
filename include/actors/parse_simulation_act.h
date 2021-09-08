#ifndef PARSE_SIMULATION_ACT_H_
#define PARSE_SIMULATION_ACT_H_

#include <iostream>
#include <fstream>
#include <string>

#include "caf/all.hpp"

caf::behavior parse_simulation(caf::event_based_actor* self, const int& skt, const caf::actor& master);

#endif  // PARSE_SIMULATION_ACT_H_