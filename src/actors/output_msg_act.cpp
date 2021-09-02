#include "../../include/actors/output_msg_act.h"

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

behavior output_message(event_based_actor* self) {
    return {
        [=](const can_frame& frame) {
            stringstream sstream;
            sstream << hex << frame.can_id;
            
            //Output message details
            aout(self) << "\nMessage details:" << endl;
            aout(self) << "can_id  = 0x" << sstream.str() <<" \r" << endl;
            aout(self) << "can_dlc = " << frame.can_dlc << " \r" << endl;
            aout(self) << "can_data = " << frame.data << " \r" << endl;
        },
    };
}