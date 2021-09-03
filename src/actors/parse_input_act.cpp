#include "../../include/actors/parse_input_act.h"

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

void parse_input(event_based_actor* self, const group& rcv_grp, const group& send_grp, const int& skt){
    string line;
    bool receiving=false;
    aout(self) << "\nActor nº: "<< self->id() <<". Is ready for input:\n" << endl;

    while (getline(cin, line)) {

        stringstream str_stream(line);

        string command;
        str_stream >> command;
        
        if(command == "receive"){
            if(receiving){
                aout(self) << "\nAlready reading from socket!\n" << endl;
                continue;
            }
            auto receiver = self->spawn(receive_msg, skt, rcv_grp);
            self->link_to(receiver);
            receiving=true;
        } 
        else if(command == "send") {
            struct can_frame frame;

            if(str_to_frame(str_stream.str(), frame)) {
                aout(self) << "Invalid message input!" << endl;
                continue;
            }

            self->spawn_in_groups({send_grp}, send_message, skt);
            self->spawn_in_groups({send_grp}, output_message);
            self->send(send_grp, frame);
        }
        else if(command == "parse") {
            string path;
            str_stream >> path;

            auto file_parser = self->spawn(parse_send_file, send_grp, skt);
            self->send(file_parser, path);
        }
        /* else if(command == "simulate") {
            string path;
            str_stream >> path;
        } */
        else if(command == "exit"){
            self->spawn(exit_act, skt, rcv_grp);
            break;
        }
        else
            aout(self) << "Invalid command!" << endl;
    }
}