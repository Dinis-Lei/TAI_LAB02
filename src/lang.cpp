#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;


/**
 * @brief Reads the command line for arguments and sets the correct hyper parameters.
 * @param argc Number of arguments passed
 * @param argv Arguments passed
 */
static void parse_command_line(int argc, char** argv);

static void load_reference(string fname);

static void load_reference_preprocessed(string fname);

static void encode_target(string fname);


// Copy Model Parameters
bool ignore1 = false;
bool save = false;
bool is_preprocessed;
float alpha = 1;
float threshold = 0.5;
float max_size = INFINITY; 
int min_size = 0;
int n_anchors = 1;
int k = 4;

string reference_fname;
string target_fname;

map<string, vector<int>> positions;


int main(int argc, char** argv) {

    parse_command_line(argc, argv);

    if (is_preprocessed)
        load_reference_preprocessed(reference_fname);
    else
        load_reference(reference_fname);

    encode_target(target_fname);
}

static void load_reference(string fname) {

    // Open the input file
    ifstream infile(fname);
    if (!infile.is_open()) {
        cerr << "Failed to open input file: " << fname << endl;
        exit(EXIT_FAILURE);
    }

    string sequence = "";
    char c;
    int index = 0;
    while (infile.get(c)) {
        if (sequence.length() == k) {
            //append to the vector of the [sequence] the index of the next char
            positions[sequence].push_back(index);
            sequence.erase(0, 1);
        }
        index++;
        sequence += c;
    }
}


static void parse_command_line(int argc, char** argv) {
    int c;                          // Opt process
    while ((c = getopt(argc, argv, "k:st:a:m:M:n:ip")) != -1) {
        switch (c)
        {
            case 'p':
                is_preprocessed = true;
                break;
            case 'i':
                ignore1 = true;
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
            case 's':
                save = true;
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
    cout << "Nº of Anchors = " << n_anchors << std::endl;
    cout << "Max Size = " << max_size << std::endl;
    cout << "Min Size = " << min_size << std::endl;
    cout << "Alpha = " << alpha << std::endl;
    cout << "Threshold = " << threshold << std::endl;
    cout << "Save = " << save << std::endl;
    cout << "Ignore Last = " << ignore1 << std::endl;
    cout << "K = " << k << std::endl;

    reference_fname = argv[optind];
    target_fname = argv[optind+1];
}