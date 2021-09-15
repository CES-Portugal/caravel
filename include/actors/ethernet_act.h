#ifndef ETHERNET_ACT_H_
#define ETHERNET_ACT_H_

#include <iostream>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <net/if.h>
#include <net/ethernet.h>	  //For ETH_P_*
#include <arpa/inet.h>        //For htons(), inet_addr()
#include <linux/if_packet.h>  //For sockaddr_ll

#include <netinet/ip_icmp.h>	//Provides declarations for icmp header
#include <netinet/udp.h>	//Provides declarations for udp header
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header
#include <netinet/if_ether.h>    // for ethernet header

#include "caf/all.hpp"


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <argp.h>
#include <time.h>
#include <map>
#include <sys/stat.h>

int setup_tcp_socket(int& skt);

void ethernet_act(caf::event_based_actor* self);

void log_summary();
void buf_dump(unsigned char *data, int Size);
void print_ethernet_header(unsigned char* buffer,int buflen);
void print_ip_header(unsigned char* buffer,int buflen);
void print_arp_packet(unsigned char* buffer, int buflen);
void print_icmp_packet(unsigned char *buffer, int buflen);
void print_tcp_packet(unsigned char* buffer,int buflen);
void print_udp_packet(unsigned char* buffer, int buflen);
void data_process(unsigned char* buffer,int buflen);


#endif  // ETHERNET_ACT_H_