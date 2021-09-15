#ifndef MIDDLEMAN_ACT_H_
#define MIDDLEMAN_ACT_H_

#include <iostream>

#include "caf/io/all.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "./output_msg_act.h"

void middleman_act(caf::event_based_actor* self, caf::io::middleman* mm);

#endif  // MIDDLEMAN_ACT_H_