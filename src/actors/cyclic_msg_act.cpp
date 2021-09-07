#include "../../include/actors/cyclic_msg_act.h"

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

behavior send_cyclic_message(event_based_actor* self,const int& skt,const int& delay){
    aout(self) << "\nSending cyclic messages every " << delay << " miliseconds!" << endl;
    return {
        [=](const can_frame& frame) {
            self->send(self->spawn(send_message, skt), frame);
            //self->send(self->spawn(output_message), frame);
            
            std::chrono::microseconds timeout{delay};
            self->delayed_send(self, timeout, frame);
            //self->quit();
        },
        [&](exit_msg& em) {
            self->quit();
        },
    };
}