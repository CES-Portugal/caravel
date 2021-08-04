#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>




/**
 * @brief Main function that executes the fundamental operations for the client
 */
int main(int ac, char* av[]);

#endif  // MAIN_H_