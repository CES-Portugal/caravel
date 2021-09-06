#include "../../include/actors/exit_act.h"

using namespace std;
using namespace caf;

void exit_act(event_based_actor* self, const int& skt) {
    close(skt);
    aout(self) << "\nEnding Caravel!"<< endl;
}