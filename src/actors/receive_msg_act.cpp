#include "../../include/actors/receive_msg_act.h"

using namespace std;
using namespace caf;

#define DELAY 50000


void receive_msg(event_based_actor* self, const int& skt, const int& interval){
    int nbytes;
    struct can_frame frame;
    auto start = chrono::steady_clock::now();
    auto end = start;
    auto msg_logger = self->spawn(log_message);

    aout(self) << "Actor nº: "<< self->id() <<". Is reading data from socket!" << endl;
    while ((interval==-1) || (chrono::duration_cast<chrono::microseconds>(end - start).count() < interval))
    {

        nbytes = read(skt, &frame, sizeof(frame));
        end = chrono::steady_clock::now();

        if (nbytes < 0){
            if(errno == EWOULDBLOCK) {
                usleep(DELAY);
                continue;
            }
            break;
        }

        aout(self) << "Received message!\n" << endl;
        
        auto msg_output = self->spawn(output_message);
        
        self->send(msg_output,frame);
        self->send(msg_logger,frame);

        nbytes=0;
    }
    aout(self) << "Finished reading data from socket!" << endl;
    self->send_exit(msg_logger, exit_reason::kill);
    self->quit(exit_reason::user_shutdown);
}