#include "../include/actors.h"

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

//Receiving messages
void receive_msg(event_based_actor* self, const int& skt, const group& grp){
    int nbytes;
    struct can_frame frame;

    aout(self) << "Actor nº: "<< self->id() <<". Is reading data from socket!" << endl;
    while (1)
    {
        nbytes = read(skt, &frame, sizeof(frame));

        if (nbytes > 0)
        {
            self->send(grp,frame);
            nbytes=0;
        }
    }
    close(skt);
    self->send_exit(grp, exit_reason::normal);
}

behavior log_message(event_based_actor* self, const bool& rcv_msg, const chrono::steady_clock::time_point& start){

    //pcpp::PcapFileWriterDevice pcapWriter("./logs/output.pcap", pcpp::LINKTYPE_CAN_SOCKETCAN);

    auto setup_vars = [&, self] {
        /* if (!pcapWriter.open())
        {
            aout(self) << "Cannot open output.pcap for writing" << endl;
            exit(1);
        } */
    };

    return {
        [=](const can_frame& frame) {
            struct timespec time;
            chrono::steady_clock::time_point end = chrono::steady_clock::now();

            //setup_vars();

            if(!rcv_msg)
                end=start;

            elapsed_time(start, end, time);
                
            /* pcpp::RawPacket rawPacket(frame.data, frame.can_dlc, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            pcapWriter.writePacket(rawPacket); */
            aout(self) << time.tv_sec << endl;
            if(!rcv_msg) self->become(log_message(self, true, chrono::steady_clock::now()));
            //MISING CLOSE PCAPWRITER
        },
    };
}

behavior output_message(event_based_actor* self) {
    return {
        [&, self](const can_frame& frame) {
            stringstream sstream;
            sstream << hex << frame.can_id;
            
            //Output message details
            aout(self) << "\nMessage details:" << endl;
            aout(self) << "can_id  = 0x" << sstream.str() <<" \r" << endl;
            aout(self) << "can_dlc = " << frame.can_dlc << " \r" << endl;
            aout(self) << "can_data = " << frame.data << " \r" << endl;
        },
    };
}


//Sending messages
void delegate_send_msg(event_based_actor* self, const group& grp){

    aout(self) << "Actor nº: "<< self->id() <<". Is waitting for input!" << endl;
    
    struct can_frame frame;
    string line;

    while (getline(cin, line)) {
        stringstream str_stream(line);

        string aux, id, msgAscii;
        str_stream >> aux >> id >> msgAscii;

        if(!valid_hex_str(msgAscii)) {
            aout(self) << "Invalid message input!" << endl;
            continue;
        }

        frame.can_id = stoi(id, 0, 16);
        msgAscii = hex_to_ascii(msgAscii);
        frame.can_dlc = msgAscii.size();
        strcpy((char*)frame.data,msgAscii.c_str());
        
        self->send(grp, frame);
    }

}

behavior send_message(event_based_actor* self, const int& skt){
    return {
        [=](const can_frame& frame) {
            int nbytes;

            nbytes = write(skt, &frame, sizeof(struct can_frame)); 
            if(nbytes != sizeof(struct can_frame)) {
                aout(self) << "Send Error!\r" << endl;
                self->quit();
            }
            
            aout(self) << "Message sent with success!\n" << endl;
        },
    };
}


void caf_main(actor_system& sys) {
    int skt;

    if (setup_socket(skt)) return;

    auto rcv_grp = sys.groups().get_local("receiver");
    auto send_grp = sys.groups().get_local("sender");

    auto output_act = sys.spawn_in_groups({rcv_grp, send_grp}, output_message);
    auto log_act = sys.spawn_in_groups({rcv_grp}, log_message, false, chrono::steady_clock::now());

    auto send_act = sys.spawn_in_groups({send_grp}, send_message, skt);

    auto receiver = sys.spawn(receive_msg, skt, rcv_grp);
    auto sender = sys.spawn(delegate_send_msg, send_grp);

/*  scoped_actor self{sys};
    self->send_exit(calc_actor, exit_reason::kill); */
}

int run_interactive(){
    caf::exec_main_init_meta_objects<id_block::custom_types_1>();                           
    caf::core::init_global_meta_objects();                                     
    return caf::exec_main<id_block::custom_types_1>(caf_main, 0, {});
}