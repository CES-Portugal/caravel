#include "../include/utils.h"

using namespace std;

string hex_to_ascii(const string& hex){
    string ascii = "";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string part = hex.substr(i, 2);
        cout << part << endl;
        char ch = stoul(part, nullptr, 16);
        ascii += ch;
    }
    return ascii;
}

bool valid_hex_str(const std::string& str){

    if (str.length()>16 || str.length()%2!=0) return false;
    for (size_t i = 0; i < str.length(); i++)
    {
        if(!isxdigit(str[i])) return false;
    }
    return true;
}