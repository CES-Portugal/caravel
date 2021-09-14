#include "../../include/actors/log_msg_act.h"

using namespace std;
using namespace caf;


behavior log_message(event_based_actor* self){
    time_t t = time(0);
    struct tm * now = localtime( &t );
    char time_str[100];
    strftime (time_str,50,"./logs/%F_%T.pcap",now);
    auto pcapWriter = make_shared<pcpp::PcapFileWriterDevice>(time_str, pcpp::LINKTYPE_CAN_SOCKETCAN);

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