#ifndef PARSE_FILE_ACT_H_
#define PARSE_FILE_ACT_H_

#include <iostream>
#include <fstream>

#include "caf/all.hpp"

#include "../utils.h"
#include "./output_msg_act.h"
#include "./send_msg_act.h"

caf::behavior parse_send_file(caf::event_based_actor* self, const caf::group& grp, const int& skt);

#endif  // PARSE_FILE_ACT_H_