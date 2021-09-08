#include "../../include/actors/receive_input_act.h"

using namespace std;
using namespace caf;

void receive_input(event_based_actor* self, const actor& sender){
    aout(self) << "\nActor nº: "<< self->id() <<". Is ready for input:\n" << endl;
    
    string line;
    while (getline(cin, line)) {
        self->send(sender, line);
        
        if(line.substr(0, line.find(" ")) == "exit") break;
    }
}