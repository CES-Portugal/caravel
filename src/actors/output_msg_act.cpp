#include "../../include/actors/output_msg_act.h"

using namespace std;
using namespace caf;

behavior output_message(event_based_actor* self) {
    return {
        [=](const can_frame& frame) {
            stringstream sstream;
            sstream << hex << frame.can_id;
            
            //Output message details
            aout(self) << "Message details:" << endl;
            aout(self) << "can_id  = 0x" << sstream.str() <<" \r" << endl;
            aout(self) << "can_dlc = " << frame.can_dlc << " \r" << endl;
            aout(self) << "can_data = " << frame.data << " \r\n" << endl;

            self->quit();
        },
    };
}