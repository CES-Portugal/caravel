#include "../../include/actors/cyclic_msg_act.h"

using namespace std;
using namespace caf;

behavior send_cyclic_message(event_based_actor* self,const int& skt,const int& delay){
    aout(self) << "\nSending cyclic messages every " << delay << " miliseconds!" << endl;
    return {
        [=](const can_frame& frame) {
            self->send(self->spawn(send_message, skt), frame);
            //self->send(self->spawn(output_message), frame);
            
            std::chrono::microseconds timeout{delay};
            self->delayed_send(self, timeout, frame);
        },
        [&](exit_msg& em) {
            self->quit(exit_reason::user_shutdown);
        },
    };
}