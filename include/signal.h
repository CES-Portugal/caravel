#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "Packet.h"
#include "PcapFileDevice.h"

extern int skt;
extern pcpp::PcapFileWriterDevice pcapWriter;

void setup_signals();

#endif  // SIGNAL_H_