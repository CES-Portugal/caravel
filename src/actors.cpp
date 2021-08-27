#include "../include/actors.h"

using namespace std;
using namespace caf;

/* 
behavior mirror(event_based_actor* self) {
    // return the (initial) actor behavior
    return {
        // a handler for messages containing a single string
        // that replies with a string
        [=](const string& what) -> string {
            cout << "Actor " << self->id() << " is waitting for input:\n";
            cin.get();
            // prints "Hello World!" via aout (thread-safe cout wrapper)
            aout(self) << "I am actor " << self->id() << ", sending " << what << std::endl;
            // reply "!dlroW olleH"
            return string{what.rbegin(), what.rend()};
        },
    };
}

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

void hello_world(event_based_actor* self, const actor& buddy, const actor& calculator) {
    //Wait for input

    // send "Hello World!" to our buddy ...
    self->request(buddy, chrono::seconds(10), "Hello World!")
        .then(
        // ... wait up to 10s for a response ...
        [=](const string& what) {
            // ... and print it
            aout(self) << "Received response: " << what << endl;
        });
    cout << "Going for calculator" << endl;
    self->request(calculator, chrono::seconds(10), add_atom_v, 1, 2)
        .then(
          [&](int32_t what) {
            // ... and print it
            cout << "Received response: " << what << endl;  
        },
        [&](error& err) {
          aout(self) << "cell #" << to_string(err) << endl;
        }
        );
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

void change_v(event_based_actor* self,const actor& buddy) {
    //Wait for input
    int n = cin.get()-'0';
    // send "Hello World!" to our buddy ...
    self->send_exit(buddy, exit_reason::kill);
    self->spawn(send_cyclic_messages, 2, n);
} */


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
void receive_msg(event_based_actor* self, const int& skt, const group& g1){
    int nbytes;
    struct can_frame frame;

    aout(self) << "Actor nº: "<< self->id() <<". Is reading data from socket!\n";
    //while (1)
    {
        nbytes = read(skt, &frame, sizeof(frame));

        if (nbytes > 0)
        {
            self->send(g1,frame);
            //self->send(g1,12);  //Test purposes
            nbytes=0;
        }
    }
    close(skt);
    self->send_exit(g1, exit_reason::normal);
}

behavior output_message(event_based_actor* self) {
    return {
        [&](const can_frame& frame) {
            //Output message
            printf("can_id  = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
            //aout(self) << "can_id = 0x" << setbase(16) <<  frame.can_id << "\r\ncan_dlc = "<< frame.can_dlc << " \r\n";
            int i = 0;
            for (i = 0; i < 8; i++)
                aout(self) << "data[" << i << "] = "<< frame.data[i] <<"\r\n";
            
        },
        [](const int& frame) {
            //Output message
            printf("number = %d \r\n", frame);
        },
    };
}

behavior log_message(event_based_actor* self,int n_msg){
    
    struct timespec time;
    chrono::steady_clock::time_point start=chrono::steady_clock::now();

    pcpp::PcapFileWriterDevice pcapWriter("./logs/output.pcap", pcpp::LINKTYPE_CAN_SOCKETCAN);

    auto setup_vars = [&] {
        if (!pcapWriter.open())
        {
            printf("Cannot open output.pcap for writing\n");
            exit(1);
        }
    };
    return {
        [&](const can_frame& frame) {
            if (n_msg == 0){
                setup_vars();
                elapsed_time(start, start, time);
                start = chrono::steady_clock::now();
            }
            else{
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                elapsed_time(start, end, time);
            }
            pcpp::RawPacket rawPacket(frame.data, frame.can_dlc, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            pcapWriter.writePacket(rawPacket);
            log_message(self, n_msg+1);
            //MISING CLOSE PCAPWRITER
        },
        [&](const int& frame) {
            printf("number = %d \r\n", frame);
            if (n_msg == 0){
                setup_vars();
                elapsed_time(start, start, time);
                start = chrono::steady_clock::now();
            }
            else{
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                elapsed_time(start, end, time);
            }
            const uint8_t* aptr;
            pcpp::RawPacket rawPacket(aptr, 2, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            pcapWriter.writePacket(rawPacket);
            log_message(self, n_msg+1);
            //MISING CLOSE PCAPWRITER
        },
    };
}

//Sending messages
void delegate_send_msg(event_based_actor* self, const actor& send_act){

    aout(self) << "Actor nº: "<< self->id() <<". Is waitting for input!\n";
    
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

        //Output message details
        printf("\nMessage details:\n");
        printf("can_id  = 0x%X\r\n", frame.can_id);
        printf("can_dlc = %d\r\n", frame.can_dlc);
        printf("can_data = %s\r\n", frame.data);

        self->send(send_act, frame);
    }

}

behavior send_message(event_based_actor* self, const int& skt){
    return {
        [&](const can_frame& frame) {
            int nbytes;
            nbytes = write(skt, &frame, sizeof(struct can_frame)); 
            if(nbytes != sizeof(struct can_frame)) {
                printf("Send Error!\r\n");
                self->quit();
            }
            //aout(self) << "Message sent with success!\n";
            cout  << "Message sent with success!\n\n";;
        },
    };
}


void caf_main(actor_system& sys) {
    int skt;

    if (setup_socket(skt)) return;

    auto grp = sys.groups().get_local("receiver");

    auto output_act = sys.spawn_in_groups({grp}, output_message);
    auto log_act = sys.spawn_in_groups({grp}, log_message, 0);
    auto send_act = sys.spawn(send_message, skt);

    auto receiver = sys.spawn(receive_msg, skt, grp);
    auto sender = sys.spawn(delegate_send_msg, send_act);


/*     auto cyclic_actor = sys.spawn(send_cyclic_messages, 1, 1);
    sys.spawn(change_v, cyclic_actor);

    // create a new actor that calls 'mirror()'
    auto mirror_actor = sys.spawn(mirror);
    
    auto calc_actor = sys.spawn(blocking_calculator_fun);
    
    // create another actor that calls 'hello_world(mirror_actor)';
    sys.spawn(hello_world, mirror_actor, calc_actor);
    // the system will wait until both actors are done before exiting the program
    scoped_actor self{sys};
    self->send_exit(calc_actor, exit_reason::kill); */
}

int run_interactive(){
    caf::exec_main_init_meta_objects<id_block::custom_types_1>();                           
    caf::core::init_global_meta_objects();                                     
    return caf::exec_main<id_block::custom_types_1>(caf_main, 0, {});
}