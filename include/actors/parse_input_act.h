#ifndef PARSE_INPUT_ACT_H_
#define PARSE_INPUT_ACT_H_

#include <iostream>
#include <chrono>

#include <linux/can.h>

#include "caf/all.hpp"

#include "../utils.h"

void parse_input(caf::event_based_actor* self, const caf::actor& file_parser, const caf::group& send_grp);

#endif  // PARSE_INPUT_ACT_H_