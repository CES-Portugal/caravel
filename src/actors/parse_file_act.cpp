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

behavior parse_send_file(event_based_actor* self, const group& grp, const int& skt){
    return {
        [=](const string& file_path) {
            ifstream file;

            file.open(file_path);

            if (!file.is_open()){
                aout(self) << "\nInvalid file path!" << endl;
                return;
            }

            struct can_frame frame;
            string line;

            while (getline(file, line)) {

                if(str_to_frame(line, frame)) {
                    aout(self) << "Invalid message input!" << endl;
                    continue;
                }

                self->spawn_in_groups({grp}, send_message, skt);
                self->spawn_in_groups({grp}, output_message);
                self->send(grp, frame);
            }

            file.close();
            self->quit();
        },
    };
}