#include "../include/application.h"

using namespace std;

#define RECEIVE_CMD 1
#define SEND_CMD 0

int skt;
pcpp::PcapFileWriterDevice pcapWriter("./logs/output.pcap", pcpp::LINKTYPE_CAN_SOCKETCAN);

void run(int cmd,int id, string message, double interval){

    if(!valid_hex_str(message)) {
        cout << "Invalid message input!" << endl;
        return;
    }

    if (setup_socket()) return;
    setup_signals();

    if(cmd == SEND_CMD) run_send(id, message, interval);
    else if(cmd == RECEIVE_CMD) run_receive();

    //Close the socket and can0
    close(skt);
    return;
}

int setup_socket(){

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

void run_send(const int& id,const string& message, const int& interval){
    int nbytes;
    int n_msg = 0;
    struct can_frame frame;
    stringstream ss;
    string str, msgAscii;

    //Set send data  
    ss << id;
    ss >> str;
    frame.can_id = stoi(str, 0, 16);
    msgAscii = hex_to_ascii(message);
    frame.can_dlc = msgAscii.size();

    strcpy((char*)frame.data,msgAscii.c_str());

    printf("Message details:\n");
    printf("can_id  = 0x%X\r\n", frame.can_id);
    printf("can_dlc = %d\r\n", frame.can_dlc);
    printf("can_data = %s\r\n", frame.data);
    
    //Send messages
    while (n_msg >= 0)
    {
        nbytes = write(skt, &frame, sizeof(struct can_frame)); 
        if(nbytes != sizeof(struct can_frame)) {
            printf("Send Error frame[0]!\r\n");
            return;
        }
        cout << "Sent message: " << n_msg << endl;

        interval != -1 ? usleep(interval*pow(10.0,6)) : n_msg=-2;
        n_msg++;
    }
}

void run_receive(){
    int nbytes;
    int n_msg = 0;
    struct can_frame frame;
    struct timespec time;
    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    if (!pcapWriter.open())
	{
		printf("Cannot open output.pcap for writing\n");
		exit(1);
	}


    cout << "Receiving Data:\n";

    while (1)
    {
        nbytes = read(skt, &frame, sizeof(frame));
        
        if (nbytes > 0)
        {
            if(n_msg==0) {
                elapsed_time(start, start, time);
                start = chrono::steady_clock::now();
            }
            else{
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                elapsed_time(start, end, time);
            }
            
            pcpp::RawPacket rawPacket(frame.data, frame.can_dlc, time,false, pcpp::LINKTYPE_CAN_SOCKETCAN);
            pcapWriter.writePacket(rawPacket);

            //Output message
            /* printf("can_id = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
            int i = 0;
            for (i = 0; i < 8; i++)
                printf("data[%d] = %d\r\n", i, frame.data[i]); */
            nbytes=0;
        }
        n_msg++;
    } 
    pcapWriter.close();
}

