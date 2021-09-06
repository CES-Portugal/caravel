#ifndef EXIT_ACT_H_
#define EXIT_ACT_H_

#include <iostream>
#include <unistd.h>

#include "caf/all.hpp"

void exit_act(caf::event_based_actor* self, const int& skt);

#endif  // EXIT_ACT_H_