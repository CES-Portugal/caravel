#include "../../include/actors/actors.h"

using namespace std;
using namespace caf;
using namespace caf::io;


void caf_main(actor_system& sys) {
    
	//Normal mode
    auto input_act = sys.spawn(parse_input);
    
	//Ethernet test
	//sys.spawn(ethernet_act);
    
	//Middleman test
    /* auto& mm = sys.middleman();
	sys.spawn(middleman_act, &mm) */;
}

int run_interactive(){
    caf::exec_main_init_meta_objects<id_block::custom_types_1>();                           
    caf::core::init_global_meta_objects();
    caf::io::middleman::init_global_meta_objects();
    return caf::exec_main<io::middleman,id_block::custom_types_1>(caf_main, 0, {});
}