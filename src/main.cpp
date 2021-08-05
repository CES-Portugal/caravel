#include "../include/main.h"

namespace po = boost::program_options;
using namespace std;


/* main.c  */
int main(int argc, char* argv[])
{

    //sudo ip link set can0 type can bitrate 500000
    //sudo ifconfig can0 up

    try {
        
        bool flag = false;

        // Declare command line (generic) options 
        po::options_description generic("Generic options");
        generic.add_options()
            ("help,h", "produce help message")    
        ;

        // Declare the supported options on the command line and file
        po::options_description config("Configuration");
        config.add_options()
            ("command", po::value<vector<string>>()->multitoken(), "Desired command (send, receive)")
            //("send,s", po::value<vector<string>>(), "Send messages <id><data> (eg: 123 DEADBEEF)")
            //("receive,r", po::value<int>()->default_value(10) ,"Receive messages")
            ("cyclic,C", po::bool_switch(&flag), "[opt] cyclic messaging")
        ;
        
        // Declare the positional options
        po::positional_options_description p;
        p.add("command", -1);


        po::options_description visible("Allowed options");
        visible.add(generic).add(config);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                    options(visible).positional(p).run(), vm);
        po::notify(vm);  
    
        if (vm.count("help")) {
            cout << visible << endl;
            return 0;
        }
        if (vm.count("command")) {
            //verificar o tamanho do vec
            vector<string> vec = vm["command"].as<vector<string>>();
            if(vec[0]=="send"){
                //send message    
                cout << "Sending message " << vec[1] << " to " << vec[0] << endl;
                if(flag) cout << "Sending more";
                return 0;
            }
            if(vec[0]=="receive"){
                //receive message
                return 0;
            }
            
            cout << "Invalid command option" << endl;
            return 1;
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }

    return 0;

    //sudo ifconfig can0 down
}
