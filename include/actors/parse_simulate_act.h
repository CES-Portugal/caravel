#ifndef PARSE_SIMULATE_ACT_H_
#define PARSE_SIMULATE_ACT_H_

#include <iostream>

#include "caf/all.hpp"

#include <linux/can.h>

caf::behavior parse_simulate(caf::event_based_actor* self, const caf::group& grp, const int& skt);

#endif  // PARSE_SIMULATE_ACT_H_