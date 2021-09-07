#include "../include/utils.h"

using namespace std;

string hex_to_ascii(const string& hex){
    string ascii = "";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string part = hex.substr(i, 2);
        char ch = stoul(part, nullptr, 16);
        ascii += ch;
    }
    return ascii;
}

bool valid_hex_str(const string& str){   
    if (str.length()>16 || str.length()%2!=0) return false;
    for (size_t i = 0; i < str.length(); i++)
    {
        if(!isxdigit(str[i])) return false;
    }
    return true;
}

void elapsed_time(chrono::steady_clock::time_point start, chrono::steady_clock::time_point end,timespec& time){
    auto elapsed_nsec = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    auto elapsed_sec = chrono::duration_cast<chrono::seconds>(end - start).count();

    time.tv_sec = elapsed_sec;
    time.tv_nsec = elapsed_nsec - (elapsed_sec*pow(10.0,9.0));
}

int str_to_frame(const string& str, struct can_frame& frame){
    stringstream str_stream(str);

    string aux, id, msgAscii;
    str_stream >> aux >> id >> msgAscii;

    if(!valid_hex_str(msgAscii)) return 1;

    frame.can_id = stoi(id, 0, 16);
    msgAscii = hex_to_ascii(msgAscii);
    frame.can_dlc = msgAscii.size();
    strcpy((char*)frame.data,msgAscii.c_str());
    return 0;
}

void interval_from_str(const string& str, int& interval){
    size_t found = str.find("every");
    
    if(found==string::npos) return;

    string new_str=str.substr(found);

    string aux, units;
    double value, multiplier = pow(10.0,6.0);;

    stringstream str_stream(new_str);
    str_stream >> aux >> value >> units;

    if(units=="sec") multiplier = pow(10.0,6.0);
    else if (units=="mili") multiplier = pow(10.0,3.0);
    
    interval = value*multiplier;
}