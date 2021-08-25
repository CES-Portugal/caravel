#include "../include/actors.h"

using namespace std;
using namespace caf;


/* behavior mirror(event_based_actor* self) {
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
}
 */

CAF_BEGIN_TYPE_ID_BLOCK(custom_types_1, first_custom_type_id)

  CAF_ADD_TYPE_ID(custom_types_1, (can_frame))

CAF_END_TYPE_ID_BLOCK(custom_types_1)

namespace caf{
    template<>
    struct inspector_access<can_frame> : inspector_access_base<can_frame> {
    template <class Inspector>
    static bool apply(Inspector& f, can_frame& x) {
        return f.object(x).fields(f.field("can_id", x.can_id),
                                f.field("can_dlc", x.can_dlc),
                                f.field("__pad", x.__pad),
                                f.field("__res0", x.__res0),
                                f.field("__res1", x.__res1),
                                f.field("data", x.data));
    }
    };
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
void receive_msg(event_based_actor* self, const int& skt, const actor& buddy){
    int nbytes;
    int n_msg = 0;
    struct can_frame frame;
    struct timespec time;
    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    cout << "Receiving Data:\n";
    while (1)
    {
        nbytes = read(skt, &frame, sizeof(frame));
        
        if (nbytes > 0)
        {
            if(n_msg==0) {
                auto elapsed_nsec = chrono::duration_cast<chrono::nanoseconds>(start - start).count();
                auto elapsed_sec = chrono::duration_cast<chrono::seconds>(start - start).count();

                time.tv_sec = elapsed_sec;
                time.tv_nsec = elapsed_nsec - (elapsed_sec*pow(10.0,9.0));
                start = chrono::steady_clock::now();
            }
            else{
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto elapsed_nsec = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                auto elapsed_sec = chrono::duration_cast<chrono::seconds>(end - start).count();

                time.tv_sec = elapsed_sec;
                time.tv_nsec = elapsed_nsec - (elapsed_sec*pow(10.0,9.0));
                
            }
            
            self->request(buddy,chrono::seconds(3),frame);
            nbytes=0;
        }
        n_msg++;
    }
    close(skt);
}

behavior log_message(event_based_actor* self) {
    return {
        [](can_frame frame) {
            //Output message
            printf("can_id = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
            int i = 0;
            for (i = 0; i < 8; i++)
                printf("data[%d] = %d\r\n", i, frame.data[i]);
            
        },
    };
}


void caf_main(actor_system& sys) {
    int skt;

    if (setup_socket(skt)) return;
    
    auto logger = sys.spawn(log_message);
    auto receiver = sys.spawn(receive_msg, skt, logger); 

    /* auto cyclic_actor = sys.spawn(send_cyclic_messages, 1, 1);
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