#ifndef ACTORS_H_
#define ACTORS_H_

#include <iostream>

#include "caf/all.hpp"

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "Packet.h"
#include "PcapFileDevice.h"
#include <time.h>
#include <chrono>
#include <math.h>

#include "./utils.h"

void caf_main(caf::actor_system& sys);

int run_interactive();

#endif  // ACTORS_H_