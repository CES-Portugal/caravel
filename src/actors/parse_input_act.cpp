#include "../../include/actors/parse_input_act.h"

using namespace std;
using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(custom_types_1, first_custom_type_id)

  CAF_ADD_TYPE_ID(custom_types_1, (can_frame))

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

int setup_socket(int& skt){
    int ret;
    struct ifreq ifr;
    struct sockaddr_can addr;

    //Create socket
    skt = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (skt < 0) {
        perror("socket PF_CAN failed");
        return 1;
    }

    //Specify can0 device
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(skt, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        perror("ioctl failed");
        return 1;
    }

    int status = fcntl(skt, F_SETFL, fcntl(skt, F_GETFL, 0) | O_NONBLOCK);
    if (status == -1){
        perror("calling fcntl");
        return 1;
    }

    //Bind the socket to can0
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(skt, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return 1;
    }

    return 0;
}

void parse_input(event_based_actor* self, const group& send_grp){
    int skt;
    string line;
    bool receiving=false;
    actor receiver;

    if (setup_socket(skt)) return;

    aout(self) << "\nActor nº: "<< self->id() <<". Is ready for input:\n" << endl;

    while (getline(cin, line)) {

        stringstream str_stream(line);

        string command;
        str_stream >> command;
        
        if(command == "receive"){
            if(receiving){
                aout(self) << "\nAlready reading from socket!\n" << endl;
                continue;
            }
            receiver = self->spawn(receive_msg, skt);
            receiving=true;
        } 
        else if(command == "send") {
            struct can_frame frame;

            if(str_to_frame(str_stream.str(), frame)) {
                aout(self) << "Invalid message input!" << endl;
                continue;
            }

            self->spawn_in_groups({send_grp}, send_message, skt);
            self->spawn_in_groups({send_grp}, output_message);
            self->send(send_grp, frame);
        }
        else if(command == "simulate") {
            string path;
            str_stream >> path;

            if (setup_socket(skt)) return;

            auto simulation_parser = self->spawn(parse_simulation, send_grp, skt);
            self->send(simulation_parser, path);
            break;
        }
        else if(command == "exit"){
            self->spawn(exit_act, skt);
            break;
        }
        else
            aout(self) << "Invalid command!" << endl;
    }
}