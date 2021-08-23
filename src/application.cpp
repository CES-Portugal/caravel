#include "../include/application.h"

using namespace std;

#define RECEIVE_CMD 1
#define SEND_CMD 0

void run(int cmd,int id, std::string message){

    int s;
    
    if(!valid_hex_str(message)) {
        cout << "Invalid message input!" << endl;
        return;
    }

    if (setup_socket(s)) return;    

    //4.Disable filtering rules, do not receive packets, only send
    //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    //4.Define receive rules
    //struct can_filter rfilter[1];
    //rfilter[0].can_id = 0x123;
    //rfilter[0].can_mask = CAN_SFF_MASK; 
    //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));  


    if(cmd == SEND_CMD) run_send(s, id, message);
    else if(cmd == RECEIVE_CMD) run_receive(s);

    //7.Close the socket and can0
    close(s);
    return;
}


int setup_socket(int& fd){

    int ret;
    struct ifreq ifr;
    struct sockaddr_can addr;

    //1.Create socket
    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (fd < 0) {
        perror("socket PF_CAN failed");
        return 1;
    }

     //2.Specify can0 device
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(fd, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        perror("ioctl failed");
        return 1;
    }

     //3.Bind the socket to can0
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return 1;
    }

    return 0;
}

void run_send(const int& fd,const int& id,const std::string& message){
    int nbytes;
    struct can_frame frame;
    stringstream ss;
    string str, msgAscii;

    //5.Set send data  
    ss << id;  
    ss >> str; 
    frame.can_id = stoi(str, 0, 16);
    msgAscii = hex_to_ascii(message);
    frame.can_dlc = msgAscii.size();

    strcpy((char*)frame.data,msgAscii.c_str()); //Different function
    //sprintf(frame.data,"%u",msgAscii.c_str());

    printf("can_id  = 0x%X\r\n", frame.can_id);
    printf("can_dlc = %d\r\n", frame.can_dlc);
    printf("can_data = %s\r\n", frame.data);
    
    //6.Send message
    nbytes = write(fd, &frame, sizeof(struct can_frame)); 
    if(nbytes != sizeof(struct can_frame)) {
        printf("Send Error frame[0]!\r\n");
    }
}

void run_receive(const int& fd){
    int nbytes;
    struct can_frame frame;
    struct timespec time;

    pcpp::PcapFileWriterDevice pcapWriter("output.pcap", pcpp::LINKTYPE_CAN_SOCKETCAN);
    if (!pcapWriter.open())
	{
		printf("Cannot open output.pcap for writing\n");
		exit(1);
	}


    cout << "Receiving Data:\n";
    
    auto start = chrono::steady_clock::now();
    int n_msg = 0;

    while (n_msg<2)
    {
        nbytes = read(fd, &frame, sizeof(frame));
        
        if (nbytes > 0)
        {
            if(n_msg==0) {
                start = chrono::steady_clock::now();
                time.tv_sec = 0;
                time.tv_nsec = 0;
            }
            else{
                auto end = chrono::steady_clock::now();
                auto elapsed_nsec = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                auto elapsed_sec = chrono::duration_cast<chrono::seconds>(end - start).count();

                time.tv_sec = elapsed_sec;
                time.tv_nsec = elapsed_nsec - (elapsed_sec*pow(10.0,9.0));
            }
            
            pcpp::RawPacket rawPacket(frame.data, frame.can_dlc, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            pcapWriter.writePacket(rawPacket);

            //Output message
            printf("can_id = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
            int i = 0;
            for (i = 0; i < 8; i++)
                printf("data[%d] = %d\r\n", i, frame.data[i]);
            nbytes=0;
        }
        n_msg++;
    } 
    pcapWriter.close();
}

