#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <json/json.h>

using namespace std;

static
void test(const std::string &inFile)
{
    ifstream ifs(inFile, ios::in);
    ostringstream oss;

    oss << ifs.rdbuf() << flush;

    Json::Reader    reader;
    Json::Value     root;
    string          encoding;

    if (!reader.parse(oss.str(), root)) {
        cerr << "Invalid json!" << endl;
        return;
    } // if

    Json::Value &jv = root["encoding1"];
    if (!jv) {
        cerr << "No attr \"encoding\" found!" << endl;
    } else {
        encoding = jv.asString();
    } // if

    cout << encoding << endl;
}


int main(int argc, char **argv)
{
    test(argv[1]);

    return 0;
}



#if 0
// Configuration options
{
    // Default encoding for text
    "encoding" : "UTF-8",
    
    // Plug-ins loaded at start-up
    "plug-ins" : [
        "python",
        "c++",
        "ruby"
        ],
        
    // Tab indent size
    "indent" : { "length" : 3, "use_space": true }
}
#endif

