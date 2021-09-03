#include "../../include/actors/send_msg_act.h"

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