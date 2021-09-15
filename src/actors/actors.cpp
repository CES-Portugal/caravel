#include "../../include/actors/actors.h"

using namespace std;
using namespace caf;
using namespace caf::io;


void receive_msg2(event_based_actor* self, const int& skt, const actor& buddy){
    int nbytes;
    struct can_frame frame;

    aout(self) << "Actor nº: "<< self->id() <<". Is reading data from socket!" << endl;
    while (1)
    {

        nbytes = read(skt, &frame, sizeof(frame));

        if (nbytes < 0){
            if(errno == EWOULDBLOCK) {
                usleep(50000);
                continue;
            }
            break;
        }

        aout(self) << "Received message!\n" << endl;
    
        self->send(buddy,frame);

        nbytes=0;
    }
    aout(self) << "Finished reading data from socket!" << endl;
    self->quit(exit_reason::user_shutdown);
}


void caf_main(actor_system& sys) {
    
	//Normal mode
    //auto input_act = sys.spawn(parse_input);
    
	//Ethernet test
	sys.spawn(ethernet_act);
    
	//Middleman test
    /* auto& mm = sys.middleman();
	sys.spawn(middleman_act, &mm); */
    
	//scoped_actor self{sys};
}

int run_interactive(){
    caf::exec_main_init_meta_objects<id_block::custom_types_1>();                           
    caf::core::init_global_meta_objects();
    caf::io::middleman::init_global_meta_objects();
    return caf::exec_main<io::middleman,id_block::custom_types_1>(caf_main, 0, {});
}