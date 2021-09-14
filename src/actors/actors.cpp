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
    
    auto input_act = sys.spawn(parse_input);
    
    /* auto& mm = sys.middleman();
    scoped_actor self{sys}; */
   
    //Middleman example without default actor
    /* char letter;
    cin >> letter;
    if(letter=='s'){
        if (auto maybe_port = mm.open(1234)) {
            aout(self) << "Openned port successfully" << endl;
            getchar();
        } else {
            aout(self) << "Failed to open port: " << to_string(maybe_port.error()) << endl;
        }
    }
    if(letter=='c'){
        if (auto maybe_nid = mm.connect("raspberrypi.local", 1234)) {
            auto nid = *maybe_nid; // ID of the connected node.
            aout(self) << "Connected successfully" << endl;

            auto type = "basic_act";             // type of the actor we wish to spawn
            auto args = make_message();           // arguments to construct the actor
            auto tout = std::chrono::seconds(30); // wait no longer than 30s
            auto worker = mm.remote_spawn<basic_act>(nid, type, args, tout);

            if (!worker) {
                cerr << "*** remote spawn failed: " << to_string(worker.error()) << endl;
                return;
            }

        } else {
            aout(self) << "Failed to connect: " << to_string(maybe_nid.error()) << endl;
        }
    } */

    //Middleman example with default actor
    /* string letter;
    cin >> letter;

    if(letter=="local"){
        auto outputter = sys.spawn(output_message);
        if (auto maybe_port = mm.publish(outputter, 1234)) {
            auto port = *maybe_port;
        } 
        else {
            aout(self) << "Failed to open port: " << to_string(maybe_port.error()) << endl;
        }
    }

    if(letter=="remote"){
        if( auto actor = mm.remote_actor("raspberrypi.local", 1234) ){
            auto act = *actor;
            
            int skt;

            if (setup_socket(skt)) return;

            sys.spawn(receive_msg2, skt, act);
        }
        else{
            aout(self) << "Failed to connect: " << to_string(actor.error()) << endl;
        }
    } */
   
    /* scoped_actor self{sys};
    self->await_all_other_actors_done();
    aout(self) << "All actors done!" << endl;
    self->send_exit(calc_actor, exit_reason::kill); */
}

int run_interactive(){
    caf::exec_main_init_meta_objects<id_block::custom_types_1>();                           
    caf::core::init_global_meta_objects();
    caf::io::middleman::init_global_meta_objects();
    return caf::exec_main<io::middleman,id_block::custom_types_1>(caf_main, 0, {});
}