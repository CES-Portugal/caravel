#ifndef ACTORS_H_
#define ACTORS_H_

#include <iostream>

#include "caf/io/all.hpp"

#include "./parse_input_act.h"
#include "./ethernet_act.h"
#include "./middleman_act.h"

void caf_main(caf::actor_system& sys);

int run_interactive();

#endif  // ACTORS_H_