#ifndef ACTORS_H_
#define ACTORS_H_

#include <iostream>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "./receive_msg_act.h"
#include "./send_msg_act.h"
#include "./output_msg_act.h"
#include "./log_msg_act.h"
#include "./parse_file_act.h"
#include "./parse_input_act.h"

void caf_main(caf::actor_system& sys);

int run_interactive();

#endif  // ACTORS_H_