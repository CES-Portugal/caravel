#include "../../include/actors/actors.h"

using namespace std;
using namespace caf;
using namespace caf::io;

/* 
behavior send_cyclic_messages(event_based_actor* self,int num, int delay) {
    aout(self) << "Hi there! This is actor nr. " << num << "!" << std::endl;
    std::chrono::seconds timeout{delay};
    self->delayed_send(self, timeout, timeout_atom_v);
    return {
        [=](timeout_atom) {
        aout(self) << "Actor nr. " << num << " says goodbye after waiting for "
                    << delay << "ms!" << std::endl;
        //self->delayed_send(self, timeout, timeout_atom_v);
        },
        after(chrono::seconds(3)) >> [] { cout << "Cyclic messaging!\n"; }
    };
}
 */


CAF_BEGIN_TYPE_ID_BLOCK(custom_types_1, first_custom_type_id)

  CAF_ADD_TYPE_ID(custom_types_1, (can_frame))
  //CAF_ADD_TYPE_ID(custom_types_1, (pcpp::PcapFileWriterDevice))

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

/* template <class Inspector>
bool inspect(Inspector& f, PcapFileWriterDevice& x) {
	return f.object(x).fields(f.field("linkLayerType", x.linkLayerType),
							  f.field("fileName", x.fileName));
} */
/* template <class Inspector>
bool inspect(Inspector& f, pcpp::PcapFileWriterDevice& x) {
    auto get_stats = [&x]() -> decltype(auto) { PcapStats stats; x.getStatistics(stats); return stats; };
    auto set_stats = [&x]() {
        return true;
    };
    return f.object(x).fields(f.field("stats", get_stats, set_stats));
} */
/* template <class Inspector>
bool inspect(Inspector& f, chrono::time_point& x) {
  return f.object(x).fields(f.field("duration", x.duration),
                                f.field("rep", x.rep),
                                f.field("period", x.period),
                                f.field("clock", x.clock));
                                
} */
behavior basic_act(event_based_actor* self){
    return {
        [=](const string& value) {
            aout(self) << "Received message with value: " << value << endl;
            
        },
    };
}

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
    //May not be necessary!!!!!!!!
    //auto send_grp = sys.groups().get_local("sender");
    
    auto input_act = sys.spawn(parse_input);
    
    /* auto& mm = sys.middleman();
    scoped_actor self{sys};
 */
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
            auto *port = maybe_port;
        } 
        else {
            aout(self) << "Failed to open port: " << to_string(maybe_port.error()) << endl;
        }
    }

    if(letter=="remote"){
        if( auto actor = mm.remote_actor("raspberrypi.local", 1234) ){
            auto act = *act;
            
            int skt;

            if (setup_socket(skt)) return;

            sys.spawn(receive_msg2, skt, );
            receive_msg2
            self->send(basic, "hello");
            self->send_exit(basic, exit_reason::kill);
        }
        else{
            aout(self) << "Failed to connect: " << to_string(act.error()) << endl;
        }
    }
    */
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