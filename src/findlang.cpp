// TODO: Add more parameters to the command line
// TODO: Use combination of faccuracy and quantity of information



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


//      log2($alfabeto)/2


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
        string target_name = t_path.substr( t_path.size()-7, 7);
        cout << "Target name " << target_name << endl;
        string preprocessFileLocation = preprocessFolder + target_name+"/";
        // check if folder preprocessFileLocation exists
        if (!fs::exists(preprocessFileLocation)){
            fs::create_directory(preprocessFileLocation);
        }
        else {
            // delete files in directory
            for (auto &p : fs::directory_iterator(preprocessFileLocation))
            {
                fs::remove(p);
            }
        }

        string preProcessFileName = preprocessFileLocation+"ipb_" + r + "_" + target_name +".txt" ;
        cout << "preProcessFileName " << preProcessFileName ;
        ifstream f(preProcessFileName);
        if (!f.good()){
            cout << " is not preprocessed"  << endl;
            string script = "./bin/lang -n 16 -r " + preProcessFileName + " " + r_Dir + r + ".utf8 "+ t_path;
            cout << "Script: " << script << endl;
            const char* c = script.c_str();
            system( c );
        }
        f.close();
        cout << endl;
        global_acc_info.insert(std::make_pair(r,calc_acc_information(preProcessFileName )));
    }
    map<string, float>::iterator it;

    float threshold = 0.6;
    it = global_acc_info.begin();
    map <string, float> provableLanguages;

    for (it; it != global_acc_info.end(); it++)
    {
        if (it->second > threshold){
            provableLanguages.insert(std::make_pair(it->first,it->second));
        }
        std::cout << it->first    // string (key)
                << ": "
                << it->second   // string's value 
                << std::endl;
    }

    cout << "The most provable languages are : " << endl;

    it = provableLanguages.begin();
    for (it; it != provableLanguages.end(); it++)
    {
        std::cout << "      " << it->first    // string (key)
                << " with a probability of "
                << it->second   // string's value 
                << std::endl;
    }

}


static float calc_acc_information(const string preProcessFilename) {
    string line;
    double acc_information = 0;
    ifstream input_file(preProcessFilename);

    if(!input_file.is_open()) {
        cerr << "Failed to open input file: " << preProcessFilename << endl;
        exit(EXIT_FAILURE);
    }

    int asize = 0;
    float total = 0.0;
    float hits = 0.0;
    if( getline(input_file,line)){
        try {
            asize = stoi(line);
        }
        catch (exception &err) {
            cout << "Invalid n argument" << std::endl;
            exit(EXIT_FAILURE);
        }
        cout << "asize " << asize << endl;
        double maxInfo = log2(asize)/2;

        while (getline(input_file,line)) {
            double acc = 0; 
            total++;
            try {
                acc = stod(line);
            }
            catch (exception &err) {
                cout << "Invalid n argument" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (maxInfo > acc){
                hits++;
            }
            acc_information += acc;
        }
    }
    else{
        cout << "Empty file" << endl;
    }
    cout << "acc_information total: " << acc_information << endl;
    cout << "Accuracy: " << hits/total << endl;
    return hits/total; //acc_information;
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
