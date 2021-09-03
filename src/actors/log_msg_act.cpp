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



void lexit_handler(scheduled_actor* self) {
    self->attach_functor([=](const error& reason) {
        aout(self) << "\nEnding Caravel!"<< endl;
    });
}
behavior log_message(event_based_actor* self, bool rcv_msg, const chrono::steady_clock::time_point& start){
    lexit_handler(self);
    //pcpp::PcapFileWriterDevice pcapWriter("./logs/output.pcap", pcpp::LINKTYPE_CAN_SOCKETCAN);

    auto setup_vars = [&, self] {
        /* if (!pcapWriter.open())
        {
            aout(self) << "Cannot open output.pcap for writing" << endl;
            exit(1);
        } */
    };

    return {
        [=](const can_frame& frame) {
            struct timespec time;
            chrono::steady_clock::time_point end = chrono::steady_clock::now();

            //setup_vars();

            if(!rcv_msg)
                end=start;

            elapsed_time(start, end, time);
                
            pcpp::RawPacket rawPacket(frame.data, frame.can_dlc, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            //pcapWriter.writePacket(rawPacket);
            aout(self) << "Pcap logger" << endl;
            if(!rcv_msg) self->become(log_message(self, true, chrono::steady_clock::now()));
            //MISING CLOSE PCAPWRITER
        },
    };
}