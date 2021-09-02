#include "../../include/actors/parse_file_act.h"

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

behavior parse_send_file(event_based_actor* self, const group& grp){
    return {
        [=](const string& file_path) {
            ifstream file;

            file.open(file_path);

            if (!file.is_open()){
                aout(self) << "\nInvalid file path!" << endl;
                return 1;
            }

            struct can_frame frame;
            string line;

            while (getline(file, line)) {
                stringstream str_stream(line);

                string aux, id, msgAscii;
                str_stream >> aux >> id >> msgAscii;

                if(!valid_hex_str(msgAscii)) {
                    aout(self) << "Invalid message input!" << endl;
                    continue;
                }

                frame.can_id = stoi(id, 0, 16);
                msgAscii = hex_to_ascii(msgAscii);
                frame.can_dlc = msgAscii.size();
                strcpy((char*)frame.data,msgAscii.c_str());
                
                self->send(grp, frame);
            }

            file.close();
            return 0;
        },
    };
}