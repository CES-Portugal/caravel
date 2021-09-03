#include "../../include/actors/exit_act.h"

using namespace std;
using namespace caf;

void exit_act(event_based_actor* self, const int& skt, const group& grp) {
    self->send_exit(grp, exit_reason::normal);
    close(skt);
    aout(self) << "\nEnding Caravel!"<< endl;
}