#include "../../include/actors/send_msg_act.h"

using namespace std;
using namespace caf;

behavior send_message(event_based_actor* self, const int& skt){
    return {
        [=](const can_frame& frame) {
            int nbytes;

            nbytes = write(skt, &frame, sizeof(struct can_frame)); 
            if(nbytes != sizeof(struct can_frame)) {
                aout(self) << "Send Message Error!\r" << endl;
                self->quit();
                return;
            }
            
            aout(self) << "Message sent with success!\n" << endl;
            self->quit();
        },
    };
}