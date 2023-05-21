#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <map>
#include <filesystem>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>
#include <chrono>
#include <math.h>       /* pow */

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Reads the command line for arguments and sets the correct hyper parameters.
 * @param argc Number of arguments passed
 * @param argv Arguments passed
 */


static void parse_command_line(int argc, char** argv);

static float calc_acc_information(const string preProcessFilename);

map <string, float> global_acc_info;
string r_Dir = "examples/language/";
string preprocessFolder = "examples/preprocess/";

bool save = false;
string t_path;

int main(int argc, char** argv) {

    parse_command_line(argc, argv);



    cout << "t_path " << t_path << endl;
    cout << "r_Dir " << r_Dir << endl;
    for (auto &p : fs::recursive_directory_iterator(r_Dir))
    {
        string r =  p.path().stem().string();
        std::cout << r << '\n';
        string target_name = t_path.substr( 9, t_path.size()-13);
        string preprocessFileLocation = preprocessFolder + target_name+"/";
        string preProcessFileName = preprocessFileLocation+"ipb_" + r + "_" + target_name +".txt" ;
        cout << "preProcessFileName " << preProcessFileName ;
        ifstream f(preProcessFileName);
        if (!f.good()){
            cout << " is not processed" ;
            string script = "./bin/lang -s " + r_Dir + r + ".utf8 "+ t_path;
            const char* c = script.c_str();
            system( c );
        }
        cout << endl;
        global_acc_info.insert(std::make_pair(r,calc_acc_information(preProcessFileName )));
    }
    map<string, float>::iterator it;

    it = global_acc_info.begin();
    string predictLanguage = it->first;
    float max = it->second;
    for (it; it != global_acc_info.end(); it++)
    {
        if (it->second < max && it->second != 0 || it->second != 0 && max == 0){
            max = it->second;
            predictLanguage = it->first;
        }
        std::cout << it->first    // string (key)
                << ':'
                << it->second   // string's value 
                << std::endl;
    }
    cout << "I guess the sample was writen in " << predictLanguage << endl;

}


static float calc_acc_information(const string preProcessFilename) {
    string line;
    double acc_information = 0;
    ifstream input_file(preProcessFilename);
    int asize = 0;
    if( getline(input_file,line)){
        try {
            asize = stoi(line);
        }
        catch (exception &err) {
            cout << "Invalid n argument" << std::endl;
            exit(EXIT_FAILURE);
        }
        cout << "asize " << asize << endl;
        double maxInfo = log2(asize);
        while (getline(input_file,line)) {
            double acc = 0; 

            try {
                acc = stod(line);
            }
            catch (exception &err) {
                cout << "Invalid n argument" << std::endl;
                exit(EXIT_FAILURE);
            }
            // if (maxInfo < acc){
            //     acc_information += -log2(1-acc);
            // }
            // else
            //     acc_information += -log2(acc);
            acc_information += acc;
        }
    }
    cout << acc_information << endl;
    return acc_information;
}
static void parse_command_line(int argc, char** argv) {
    int c;                          // Opt process
    while ((c = getopt(argc, argv, "s")) != -1) {
        switch (c)
        {
            case 's':
                save = true;
                break;
            case '?':
                cout << "Got unknown option." << std::endl; 
                break;
            default:
                cout << "Got unknown parse returns: " << c << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    cout << "Save = " << save << std::endl;


    t_path = argv[optind];
}
