#include "../include/main.h"

namespace po = boost::program_options;
using namespace std;

#define RECEIVE_CMD 1
#define SEND_CMD 0

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
            ("help,h", "Produce help message")    
        ;

        // Declare the supported options on the command line and file
        po::options_description config("Configuration");
        config.add_options()
            ("command", po::value<string>(), "Desired command (send, receive)")
            ("id", po::value<int>(), "Message id (eg: 123)")
            ("message,m", po::value<string>() ,"Message data in hex (eg: \"DEADBEEF\")")
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
            string cmd = vm["command"].as<string>();

            if(cmd=="send"){
                 
                if(vm.count("id")&&vm.count("message")){
                    run(SEND_CMD, vm["id"].as<int>(), vm["message"].as<string>());
                    return 0;
                }
                cout << "Missing id or message data parameter." << endl;
                return 1;
            }

            if(cmd=="receive"){                
                run(RECEIVE_CMD);
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