#include "../../include/actors/receive_msg_act.h"

using namespace std;
using namespace caf;

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

void rexit_handler(scheduled_actor* self) {
    self->attach_functor([=](const error& reason) {
        aout(self) << "\nEnding Caravel!"<< endl;
    });
}
//Receiving messages
void receive_msg(event_based_actor* self, const int& skt, const group& grp){
    rexit_handler(self);
    int nbytes;
    struct can_frame frame;
    self->spawn_in_groups({grp}, log_message, false, chrono::steady_clock::now());

    aout(self) << "Actor nº: "<< self->id() <<". Is reading data from socket!" << endl;
    while (1)
    {
        nbytes = read(skt, &frame, sizeof(frame));

        if (nbytes > 0)
        {
            self->spawn_in_groups({grp}, output_message);
            self->send(grp,frame);
            nbytes=0;
        }
    }
}