#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;

int main(int ac, char* av[])
{
    try {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("compression", po::value<double>(), "set compression level")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("compression")) {
            cout << "Compression level was set to " 
                 << vm["compression"].as<double>() << ".\n";
        } else {
            cout << "Compression level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}




/* #include "../include/main.h"
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

main.c 
int main(int ac, char* av[])
{

     try {

        options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("compression", value<double>(), "set compression level")
        ;

        variables_map vm;        
        store(parse_command_line(ac, av, desc), vm);
        notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("compression")) {
            cout << "Compression level was set to " 
                 << vm["compression"].as<double>() << ".\n";
        } else {
            cout << "Compression level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }


    int input;
    bool leave = false;

    while (!leave)
    {
        cout << "Welcome to Caravel!\n";
        cout << "\t1. Send messages\n";
        cout << "\t2. Receive messages\n";
        cout << "\t3. Exit\n";
        
        cout << "$ ";

        cin >> input;

        switch (input)
        {
        case 1:
            cout << "Sending messages";
            break;
        case 2:
            cout << "Receiving messages";
            break;
        case 3:
            cout << "Bye!\n";
            leave = true;
        default:
            break;
        }
    }


}

 */