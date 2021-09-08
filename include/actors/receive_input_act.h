#ifndef RECEIVE_INPUT_ACT_H_
#define RECEIVE_INPUT_ACT_H_

#include <iostream>
#include <string.h>

#include "caf/all.hpp"

void receive_input(caf::event_based_actor* self, const caf::actor& sender);

#endif  // RECEIVE_INPUT_ACT_H_