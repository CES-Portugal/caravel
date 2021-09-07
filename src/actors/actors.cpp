#include "../../include/actors/actors.h"

using namespace std;
using namespace caf;

/* 
void blocking_calculator_fun(blocking_actor* self) {
  bool running = true;
  self->receive_while(running)( //
    [](add_atom, int32_t a, int32_t b) { return a+b; },
    [](sub_atom, int32_t a, int32_t b) { return a - b; },
    [&](exit_msg& em) {
      if (em.reason) {
        self->fail_state(std::move(em.reason));
        running = false;
      }
    });
}

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

void caf_main(actor_system& sys) {
    //May not be necessary!!!!!!!!
    auto send_grp = sys.groups().get_local("sender");
    
    auto input_act = sys.spawn(parse_input);


/*  scoped_actor self{sys};
    self->send_exit(calc_actor, exit_reason::kill); */
}

int run_interactive(){
    caf::exec_main_init_meta_objects<id_block::custom_types_1>();                           
    caf::core::init_global_meta_objects();                                     
    return caf::exec_main<id_block::custom_types_1>(caf_main, 0, {});
}