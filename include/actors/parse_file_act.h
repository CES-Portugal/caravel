#ifndef PARSE_FILE_ACT_H_
#define PARSE_FILE_ACT_H_

#include <iostream>
#include <fstream>

#include <linux/can.h>

#include "caf/all.hpp"

#include "../utils.h"

caf::behavior parse_send_file(caf::event_based_actor* self, const caf::group& grp);

#endif  // PARSE_FILE_ACT_H_