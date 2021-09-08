#include "../../include/actors/parse_simulation_act.h"

using namespace std;
using namespace caf;

behavior parse_simulation(event_based_actor* self, const int& skt, const actor& master){
    return {
        [=](const string& file_path) {
            ifstream file;

            file.open(file_path);

            if (!file.is_open()){
                aout(self) << "\nInvalid file path!" << endl;
                self->quit(exit_reason::user_shutdown);
                return;
            }

            string line;
            while (getline(file, line)) {
                self->send(master, line);
            }

            file.close();
            self->quit();
        },
    };
}