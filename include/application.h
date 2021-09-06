#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "./utils.h"
#include "./signal.h"

void run(int cmd,int id=0, std::string message="", double interval=-1);

void run_send(const int& id,const std::string& message, const int& interval);

void run_receive();

int setup_socket();

#endif  // APPLICATION_H_