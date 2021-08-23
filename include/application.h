#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include <chrono>
#include <math.h>

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "Packet.h"
#include "PcapFileDevice.h"


#include "./utils.h"

void run(int cmd,int id=0, std::string message="");

void run_send(const int& fd,const int& id,const std::string& message);

void run_receive(const int& fd);

int setup_socket(int& fd);

#endif  // APPLICATION_H_