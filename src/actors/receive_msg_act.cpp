#include "../../include/actors/receive_msg_act.h"

using namespace std;
using namespace caf;

#define DELAY 50000

CAF_BEGIN_TYPE_ID_BLOCK(custom_types_1, first_custom_type_id)

  CAF_ADD_TYPE_ID(custom_types_1, (can_frame))

CAF_END_TYPE_ID_BLOCK(custom_types_1)

template <class Inspector>
bool inspect(Inspector& f, can_frame& x) {
  return f.object(x).fields(f.field("can_id", x.can_id),
                                f.field("can_dlc", x.can_dlc),
                                f.field("__pad", x.__pad),
                                f.field("__res0", x.__res0),
                                f.field("__res1", x.__res1),
                                f.field("data", x.data));
                                
}

//Receiving messages
void receive_msg(event_based_actor* self, const int& skt, const int& interval){
    int nbytes;
    struct can_frame frame;
    auto start = chrono::steady_clock::now();
    auto end = start;
    auto msg_logger = self->spawn(log_message, false, start);

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