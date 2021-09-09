#include "../../include/actors/log_msg_act.h"

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


behavior log_message(event_based_actor* self){
    auto pcapWriter = make_shared<pcpp::PcapFileWriterDevice>("./logs/output.pcap", pcpp::LINKTYPE_CAN_SOCKETCAN);
    bool rcv_msg = false;
    auto start = chrono::steady_clock::now();

    if (!(*pcapWriter).open(rcv_msg))
    {
        aout(self) << "Cannot open output.pcap for writing" << endl;
        self->quit(exit_reason::kill);
    }

    self->set_exit_handler([=] (const exit_msg& msg){
            (*pcapWriter).close();
        }
    );

    return {
        [=](const can_frame& frame) mutable {
            struct timespec time;
            chrono::steady_clock::time_point end = chrono::steady_clock::now();

            if(!rcv_msg)
                end=start;

            elapsed_time(start, end, time);
                
            pcpp::RawPacket rawPacket(frame.data, frame.can_dlc, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            (*pcapWriter).writePacket(rawPacket);

            if(!rcv_msg) {
                rcv_msg = true;
                start = chrono::steady_clock::now();
            }
        },
    };
}