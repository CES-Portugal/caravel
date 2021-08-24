#include "../include/signal.h"

using namespace std;

void sigint_handler(int signal) {
    
    //Close the socket and can0
    close(skt);
    
    //Close the pcap file
    pcapWriter.close();
    
    cout << "\n\nTerminating Caravel..." << endl;
    exit(1);
}

void setup_signals(){
    struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = sigint_handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);
}