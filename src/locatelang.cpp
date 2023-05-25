#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>
#include <chrono>
#include <dirent.h>
#include <stdio.h>
#include <map>
#include <math.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Reads the command line for arguments and sets the correct hyper parameters.
 * @param argc Number of arguments passed
 * @param argv Arguments passed
 */


struct Anchor {
    bool is_active = false;         // Check if anchor is being used or not
    int position;                   // Where the anchor is situated in the full sequence
    int hits = 0;                   // Counter of hits
    int misses = 0;                 // Counter of misses
    int length = 0;                 // How many symbols has the anchor tested
    map<char, float> info;          // Accumulative information for each symbol thoughout the anchor test
    float acc_info = 0;
};


static void parse_command_line(int argc, char** argv);

static void load_reference(const string fname);

static void load_reference_preprocessed(const string fname);

static void encode_target(const string fname);

static double estimate_probability(int hits, int misses, double alpha, int asize);

static float calc_acc_information(const string fname,const string e_fname);

static int get_alphabet(const string filename, unordered_set<char> &alphabet);

static map<string, vector<Anchor>> get_example_model(const string filename);

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



string r_Dir;               // Path to the directory with a sets of examples from several languages (The ri). Each example is called 'r'
string t_path;              // Path to the text t that we want to guess the language 

map <string, float> global_acc_info;



int main(int argc, char** argv) {

    parse_command_line(argc, argv);



    for (auto &p : fs::recursive_directory_iterator(r_Dir))
    {
        string r =  p.path().stem().string();
        std::cout << r << '\n';
        if (is_preprocessed)
            load_reference_preprocessed(r);
        else{
            global_acc_info.insert(std::make_pair(r,calc_acc_information(t_path,r_Dir+r+".utf8")));
            //load_reference(r);
            
        }
    }
    map<string, float>::iterator it;

    it = global_acc_info.begin();
    string predictLanguage = it->first;
    float max = it->second;
    for (it; it != global_acc_info.end(); it++)
    {
        if (it->second < max){
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

static float calc_acc_information(const string fname, const string e_fname) {

    map<string, vector<Anchor>> dict = get_example_model(e_fname);

    unordered_set<char> alphabet;
    map<char, float> global_info;                           // table of symbol - amount of information 

    int fsize = get_alphabet(fname, alphabet);           // Size of the file (number of symbols)
    int asize = 0;                                          // Size of the alphabet
    for (char c: alphabet){
        //cout << asize << " " << (c) << endl;
        global_info.insert(make_pair(c, 0));
        asize++;
    }
    cout << "Alphabet size: " << asize << endl;
    // Create a text string, which is used to output the text file
    char byte = 0;

    string sequence = "";
    string full_seq = "";

    int i = 0;
    int offset = 1;
    string testing_seq;

    double sum_acc = 0;
    int count_copies = 0;
    int sum_len_copies = 0;

    // Use a while loop together with the getline() function to read the file line by line
    cout << endl;
    int not_copy = 0;
    int n_symbols = 0;

    vector<Anchor> testing_anchors;

    double acc_information = 0;
    // Create and open a text file
    ifstream input_file(fname);
    while (input_file.get(byte)) {
        // Test sequence accuracy 
        if (testing_seq.length() > 0) {
            bool has_active = false;
            int active_count = 0;                                                           // Count the number of anchor active
            for (Anchor& s : testing_anchors) {
                if (!s.is_active) {
                    continue;
                }

                active_count++;
                s.length++;
                char predicted_symb = full_seq[s.position + offset];                        // Next symbol in the anchor sequence
                double p_curr_symb = estimate_probability(s.hits, s.misses, alpha, asize);  // Probability of the predicted symbol
                if (predicted_symb == byte) {
                    s.acc_info += -log2(p_curr_symb);
                }
                else {
                    s.acc_info += -log2(1 - p_curr_symb);
                }

                // Update the hit/miss ratio
                if (predicted_symb == byte) {
                    s.hits++;
                }
                else {
                    s.misses++;
                }

                // Check if the anchor has a hit/miss ratio that passes the threshold
                if (s.length > min_size) {
                    double acc = estimate_probability(s.hits, s.misses, alpha, asize);
                    if (acc < threshold) {
                        s.is_active = false;
                        active_count--;
                        // Don't encode the last failure
                        //s.misses--;
                    }
                }
                if (s.length >= max_size) {
                    s.is_active = false;
                    active_count--;
                }
            }
            offset++;
            // No anchor available to test
            if (active_count < 1) {
                double max_acc = 0;
                int max_id = 0;
                int max_len = 0;
                double best_acc_info = 0;
                map<char, float> best_info;
                for(Anchor& s: testing_anchors) {
                    if (s.length > 0){
                        int hits = s.hits;
                        int misses = s.misses;
                        double ps = estimate_probability(hits, misses, alpha, asize);
                        // Select the sequence who encode the most and with better accuracy
                        if(ps > max_acc && s.length >= max_len) {
                            max_acc = ps;
                            max_id = i;
                            best_info = s.info;
                            max_len = s.length;
                            best_acc_info = s.acc_info;
                        }
                        // Reset Anchor
                        s.hits = 0;
                        s.misses = 0;
                        s.length = 0;
                        for(auto it = s.info.begin(); it != s.info.end(); ++it) {
                            it->second = 0;
                        }
                    }
                }

                testing_anchors.clear();

                if (max_len > 1) {
                    sum_acc += max_acc;
                    sum_len_copies += max_len;
                    count_copies++;
                    // cout << "\t" << "Info:" << endl; 
                    acc_information += best_acc_info;
                    for (auto it = global_info.begin(); it != global_info.end(); ++it) {
                        auto it2 = best_info.find(it->first);
                        if (it2 != best_info.end()) {
                            // Update the value of the pair
                            // Sum the cumulative information of the sequence and the information of the last symbol that wasn't encoded
                            it->second += it2->second + (-log2((float)1/asize));
                            // cout << "\t\t" << it->first << ": " << it->second/max_len << endl; 
                        }
                    }
                    not_copy++;
                }
                else {
                    not_copy += max_len;
                    acc_information += 2*max_len;
                    for (auto it = global_info.begin(); it != global_info.end(); ++it) {
                        it->second += max_len*-log2((float)1/asize);
                    }
                }
                
                offset = 1;
                testing_seq = "";
            }
        }
        else {
            not_copy++;
            acc_information += 2;
        }
        if (sequence.length() == k) {
            sequence.erase(0, 1);
        }

        sequence += byte;
        full_seq += byte;

        if (sequence.length()==k){
            // Check if there is no candidate sequence and it has appeared more than once
            if (testing_seq.length() == 0 && full_seq.length() > k && dict.count(sequence) > 0) {
                testing_seq = sequence;
                int i = 0;
                auto it = dict.find(testing_seq);
                if (it != dict.end()) {
                    // Iterate through the vector in reverse order
                    std::vector<Anchor>& myVec = it->second;
                    // Save the n-th most recent anchors
                    for (auto rit = myVec.rbegin(); rit != myVec.rend(); ++rit) {
                        // Update the value of each element in the struct
                        if (i == n_anchors) {
                            break;
                        }
                        rit->is_active = true;
                        testing_anchors.push_back(*rit);
                        i++;
                    }
                }
            }
        }
        i++;

    }
    
    return acc_information;
}


static map<string, vector<Anchor>> get_example_model(const string filename){
    unordered_set<char> alphabet;

    int fsize = get_alphabet(filename, alphabet);           // Size of the file (number of symbols)
    // Create a text string, which is used to output the text file
    char byte = 0;

    string sequence = "";
    string full_seq = "";


    // Create and open a text file
    ifstream input_file(filename);
    int i = 0;
    map<string, vector<Anchor>> dict;



    vector<Anchor> testing_anchors;
    double acc_information = 0;
    // Main loop
    while (input_file.get(byte)) {

        if (sequence.length() == k) {
            sequence.erase(0, 1);
        }

        sequence += byte;
        full_seq += byte;

        if (sequence.length()==k){

            // Save the new Anchor
            Anchor new_seq;
            new_seq.position = i;
            for (char c: alphabet) {
                new_seq.info.insert(make_pair(c, 0));
            }
            if (dict.count(sequence) == 0) {
                vector<Anchor> v;
                dict.insert( make_pair(sequence, v) );
            }
            dict[sequence].push_back(new_seq);
        }

        i++;
    }

    return dict;
}

static void parse_command_line(int argc, char** argv) {
    int c;                          // Opt process
    while ((c = getopt(argc, argv, "k:st:a:m:M:n:ipe:")) != -1) {
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

    r_Dir = argv[optind];
    t_path = argv[optind+1];
}


static int get_alphabet(const string filename, unordered_set<char> &alphabet) {
    char byte = 0;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return - 1;
    }
    int count = 0;
    while (input_file.get(byte)) {
        alphabet.insert(byte);
        count++;
    }
    input_file.close();
    return count;
}

static double estimate_probability(int hits, int misses, double alpha, int asize) {
    return (hits + alpha)/(hits+misses+2*alpha);
}

static void load_reference_preprocessed(const string fname){
    cout << "Sup" ;

}