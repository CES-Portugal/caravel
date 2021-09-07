#ifndef CYCLIC_MSG_ACT_H_
#define CYCLIC_MSG_ACT_H_

#include <iostream>

#include "./send_msg_act.h"
#include "./output_msg_act.h"

caf::behavior send_cyclic_message(caf::event_based_actor* self,const int& skt,const int& delay);

#endif  // CYCLIC_MSG_ACT_H_