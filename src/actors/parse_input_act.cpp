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

void parse_input(event_based_actor* self, const actor& file_parser, const group& send_grp){
    string line;

    aout(self) << "\nActor nº: "<< self->id() <<". Is waiting for input:\n" << endl;

    while (getline(cin, line)) {

        stringstream str_stream(line);

        string command;
        str_stream >> command;
        
        if(command == "parse") {
            string path;
            str_stream >> path;
            self->request(file_parser, chrono::seconds(3), path)
                .await(
                    [=](int res) {
                        if(!res) aout(self) << "File parsed successfully!" << endl;
                    }
                );
            continue;
        } 
        
        if(command == "send") {
            struct can_frame frame;
            string id, msgAscii;

            str_stream >> id >> msgAscii;

            if(!valid_hex_str(msgAscii)) {
                aout(self) << "Invalid message input!" << endl;
                continue;
            }

            frame.can_id = stoi(id, 0, 16);
            msgAscii = hex_to_ascii(msgAscii);
            frame.can_dlc = msgAscii.size();
            strcpy((char*)frame.data,msgAscii.c_str());
            
            self->send(send_grp, frame);
            continue;
        }
        
        if(command == "simulate") {
            string path;
            str_stream >> path;
            self->request(file_parser, chrono::seconds(3), path)
                .await(
                    [=](int res) {
                        if(!res) aout(self) << "File parsed successfully!" << endl;
                    },
                    [&](error& err) {
                        aout(self) << to_string(err) << endl;
                    });
            continue;
        }
        
        if(command == "exit"){
            break;
        }
        else
            aout(self) << "Invalid command!" << endl;
    }
}