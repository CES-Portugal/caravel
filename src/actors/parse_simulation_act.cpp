#include "../../include/actors/parse_simulation_act.h"

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

behavior parse_simulation(event_based_actor* self, const int& skt){
    return {
        [=](const string& file_path) {

            ifstream file;

            file.open(file_path);

            if (!file.is_open()){
                aout(self) << "\nInvalid file path!" << endl;
                self->quit();
                return;
            }

            struct can_frame frame;
            string line;
            bool receiving = false;

            while (getline(file, line)) {
                string aux=line.substr(0, line.find(" "));

                if(aux=="#") continue;

                if(aux=="send"){
                    if(str_to_frame(line, frame)) {
                        aout(self) << "Invalid message input!" << endl;
                        continue;
                    }

                    self->send(self->spawn(send_message, skt), frame);
                    self->send(self->spawn(output_message), frame);
                }
                else if(aux=="receive"){
                    if(receiving){
                        aout(self) << "\nAlready reading from socket!\n" << endl;
                        continue;
                    }
                    auto receiver = self->spawn(receive_msg, skt);
                    receiving=true;
                }
            }

            file.close();
            self->quit();
        },
    };
}