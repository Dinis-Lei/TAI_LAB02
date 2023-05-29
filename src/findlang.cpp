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

static string createScript(const string r);

map <string, float> global_acc_info;
string r_Dir = "examples/language/";
string preprocessFolder = "examples/preprocess/";

string t_path;

bool is_preprocessed = false;
float alpha = 1;
float threshold = 0.5;
float max_size = INFINITY; 
int min_size = 0;
int n_anchors = 1;
long unsigned int k = 4;

int main(int argc, char** argv) {

    parse_command_line(argc, argv);



    // cout << "t_path " << t_path << endl;
    // cout << "r_Dir " << r_Dir << endl;
    for (auto &p : fs::recursive_directory_iterator(r_Dir))
    {
        string r =  p.path().stem().string();
        // std::cout << r << '\n';
        string target_name = t_path.substr( 9, t_path.size()-13);
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
        // cout << "preProcessFileName " << preProcessFileName ;
        if (!is_preprocessed){
            // cout << " is not preprocessed" << endl;
            string script = createScript(r);
            // cout << "script :" << script ;
            const char* c = script.c_str();
            system( c );
        }
        // cout << endl;
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
        // cout << "asize " << asize << endl;
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
    // cout << "acc_information total: " << acc_information << endl;
    // cout << "Accuracy: " << hits/total << endl;
    return hits/total; //acc_information;
}


static string createScript(const string r){
    string base = "./bin/lang";
    if (max_size == INFINITY)
        return  "./bin/lang -k "+std::to_string(k) +"  -t "+std::to_string(threshold)+" -m "+std::to_string(min_size) + " -a "+ std::to_string(alpha) +" -n " + std::to_string(n_anchors) +" -s " + r_Dir + r + ".utf8 "+ t_path;
    else
        return  "./bin/lang -k "+std::to_string(k) +"  -t "+std::to_string(threshold)+" -m "+std::to_string(min_size) + " -M "+ std::to_string(max_size) + " -a "+ std::to_string(alpha) +" -n " + std::to_string(n_anchors) +" -s " + r_Dir + r + ".utf8 "+ t_path;
}

static void parse_command_line(int argc, char** argv) {
    int c;                          // Opt process
    while ((c = getopt(argc, argv, "k:t:a:m:M:n:ip")) != -1) {
        switch (c)
        {
            case 'p':
                is_preprocessed = true;
                break;
            case 'n':
                try {
                    n_anchors = stoi(optarg);
                }
                catch (exception &err) {
                    cout << "Invalid n argument" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'M':
                try {
                    max_size = stoi(optarg);
                }
                catch (exception &err) {
                    cout << "Invalid m argument" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                try {
                    min_size = stoi(optarg);
                }
                catch (exception &err) {
                    cout << "Invalid m argument" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'a':
                try {
                    alpha = stof(optarg);
                }
                catch (exception &err) {
                    cout << "Invalid a argument" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 't':
                try {
                    threshold = stof(optarg);
                }
                catch (exception &err) {
                    cout << "Invalid t argument" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'k':
                try {
                    k = stoi(optarg);
                }
                catch (exception &err) {
                    cout << "Invalid K argument" << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                cout << "Got unknown option." << std::endl; 
                break;
            default:
                cout << "Got unknown parse returns: " << c << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    // cout << "Nº of Anchors = " << n_anchors << endl;
    // cout << "Max Size = " << max_size << endl;
    // cout << "Min Size = " << min_size << endl;
    // cout << "Alpha = " << alpha << endl;
    // cout << "Threshold = " << threshold << endl;
    // cout << "K = " << k << endl;
    // cout << "Is preprocessed = " << is_preprocessed << endl;


    t_path = argv[optind];
}
