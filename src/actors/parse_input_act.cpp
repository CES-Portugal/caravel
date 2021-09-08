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

behavior parse_input(event_based_actor* self){

    int skt;
    string line;
    bool receiving=false;
    
    if (setup_socket(skt)) self->quit(exit_reason::kill);

    self->set_exit_handler([self, &receiving]
        (const exit_msg& msg){
            aout(self) << "receiver is down" << endl;
            receiving = false;
        }
    );

    self->spawn<detached>(receive_input, self);

    return{
        [=, &receiving](const string& line){
            stringstream str_stream(line);

            string command;
            str_stream >> command;
            
            if(command=="#") return;

            if(command == "receive"){
                if(receiving){
                    aout(self) << "\nAlready reading from socket!\n" << endl;
                    return;
                }

                int interval=-1;
                interval_from_str(line, interval, "for");

                auto receiver = self->spawn(receive_msg, skt, interval);
                self->link_to(receiver);  //Not working yet!!!
                receiving=true;
            } 
            else if(command == "send") {
                struct can_frame frame;

                if(str_to_frame(line, frame)) {
                    aout(self) << "Invalid message input!" << endl;
                    return;
                }
                
                int interval=-1;
                interval_from_str(line, interval, "every");

                if(interval!=-1){
                    //Cyclic message
                    auto cyclic_sender = self->spawn(send_cyclic_message, skt, interval);
                    self->link_to(cyclic_sender);
                    self->send(cyclic_sender, frame);
                    return;
                }
                
                //Normal message
                self->send(self->spawn(send_message, skt), frame); 
                self->send(self->spawn(output_message), frame);
            }
            else if(command == "simulate") {
                string path;
                str_stream >> path;

                //if (setup_socket(skt)) self->quit(exit_reason::kill);

                auto simulation_parser = self->spawn(parse_simulation, skt, self);
                self->send(simulation_parser, path);
            }
            else if(command == "exit"){
                self->spawn(exit_act, skt);
                self->quit(exit_reason::user_shutdown);
            }
            else
                aout(self) << "Invalid command!" << endl;
            },
    };
}