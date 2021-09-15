#include "../../include/actors/middleman_act.h"

using namespace std;
using namespace caf;
using namespace caf::io;

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

void receive_msg_remote(event_based_actor* self, const int& skt, const actor& buddy){
    int nbytes;
    struct can_frame frame;

    aout(self) << "Actor nº: "<< self->id() <<". Is reading data from socket!" << endl;
    int n_msg = 0;
    while (n_msg < 1)
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
        n_msg++;

        nbytes=0;
    }
    aout(self) << "Finished reading data from socket!" << endl;
}

void middleman_act(event_based_actor* self, middleman* mm){
    //Middleman example with default actor
    string letter;
    cin >> letter;

    if(letter=="local"){
        auto outputter = self->spawn(output_message);
        if (auto maybe_port = mm->publish(outputter, 1234)) {
            auto port = *maybe_port;
        } 
        else {
            aout(self) << "Failed to open port: " << to_string(maybe_port.error()) << endl;
        }
    }

    if(letter=="remote"){
        if( auto actor = mm->remote_actor("raspberrypi.local", 1234) ){
            auto act = *actor;
            
            int skt;

            if (setup_socket(skt)) return;

            self->spawn(receive_msg_remote, skt, act);
        }
        else{
            aout(self) << "Failed to connect: " << to_string(actor.error()) << endl;
        }
    }

    //Middleman example without default actor
    /* char letter;
    cin >> letter;
    if(letter=='s'){
        if (auto maybe_port = mm-<open(1234)) {
            aout(self) << "Openned port successfully" << endl;
            getchar();
        } else {
            aout(self) << "Failed to open port: " << to_string(maybe_port.error()) << endl;
        }
    }
    if(letter=='c'){
        if (auto maybe_nid = mm->connect("raspberrypi.local", 1234)) {
            auto nid = *maybe_nid; // ID of the connected node.
            aout(self) << "Connected successfully" << endl;

            auto type = "basic_act";             // type of the actor we wish to spawn
            auto args = make_message();           // arguments to construct the actor
            auto tout = std::chrono::seconds(30); // wait no longer than 30s
            auto worker = mm->remote_spawn<basic_act>(nid, type, args, tout);

            if (!worker) {
                cerr << "*** remote spawn failed: " << to_string(worker.error()) << endl;
                return;
            }

        } else {
            aout(self) << "Failed to connect: " << to_string(maybe_nid.error()) << endl;
        }
    } */
}