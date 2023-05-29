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

struct Anchor {
    bool is_active = false;                                     // Check if anchor is being used or not
    int position;                                               // Where the anchor is situated in the full sequence
    int hits = 0;                                               // Counter of hits
    int misses = 0;                                             // Counter of misses
    int length = 0;                                             // How many symbols has the anchor tested
    unordered_map<char, double> sum_info_per_symbol;            // Sum of all information gained for each symbol
    double sum_info = 0;                                        // Sum of all information gained by all the symbols  
    vector<double> info_per_byte;                               // Stores the history of information gained by each symbol encoded
};


/**
 * @brief Reads the command line for arguments and sets the correct hyper parameters.
 * @param argc Number of arguments passed
 * @param argv Arguments passed
 */
static void parse_command_line(int argc, char** argv);

/**
 * @brief Loads the reference training text and procecess it
 * @param fname 
 */
static void load_reference(string fname);

/**
 * @brief Loads the reference training text already processed
 * @param fname 
 */
static void load_reference_preprocessed(string reference_fname, string copy_model_fname, string context_model_fname);

/**
 * @brief Encodes the target text
 * @param target_fname  Name of the target file
 * @param ibp_fname     Name of the file to store the information per byte
 * @param fsize         Original file size
 * @param csize         Size of file after compression
 */
static void encode_target(string target_fname, string ibp_fname, double& fsize, double& csize);

static bool update_anchors(char symbol, int offset, bool& ignored_last, vector<Anchor>& anchors);

static void select_best_anchor(vector<Anchor>& anchors, char byte, bool ignored_last, double& csize, vector<double>& info_per_byte);

static void write_array_to_file(vector<double> &array, string fname);

static void generate_context_model(string fname, long unsigned int k);


// Copy Model Parameters
bool ignore1 = false;
bool save = false;
bool is_preprocessed;
float alpha = 1;
float threshold = 0.5;
float max_size = INFINITY; 
int min_size = 0;
int n_anchors = 1;
long unsigned int k = 4;

string reference_fname;
string target_fname;
string ipb_fname;
string copy_model_fname;
string context_model_fname;
vector<char> reference_string;

map<string, vector<int>> positions;
map<string, map<string, int>> context_table;

int count_copies = 0, count_not_copies = 0, global_hits = 0, global_misses = 0, tot_anchors = 0;
int default_symb_size;

set<char> alphabet;
int asize;

string context_window;
long unsigned int context_window_size = 2;

int context_hits = 0;

int main(int argc, char** argv) {

    parse_command_line(argc, argv);
    if (is_preprocessed)
        load_reference_preprocessed(reference_fname, copy_model_fname, context_model_fname);
    else
        load_reference(reference_fname);
        generate_context_model(reference_fname, context_window_size);
    }
        

    if (save)
        save_model(context_model_fname);

    generate_context_model(reference_fname, context_window_size);

    // iterate over the positions map and calculate the average length of the vectors
    int all_sum = 0;
    int count = 0;
    for (auto pair : positions) {
        int sum = 0;
        for (int i : pair.second) {
            sum += i;
        }
        all_sum = sum/pair.second.size();
        count++;
    }
    cout << "Average length: " << all_sum/count << endl;
<<<<<<<<< Temporary merge branch 1
    cout << "reference_fname: " << reference_fname << "|" << endl;
    cout << "target_fname: " << target_fname << "|" << endl;

    string t = target_fname.substr(9, target_fname.size() - 13);
    string preprocessFileLocation = "examples/preprocess/"+ t;
    cout << "preprocessFileLocation : " << preprocessFileLocation << endl;
    if (stat(preprocessFileLocation.c_str(), &sb) != 0){
        if(mkdir(preprocessFileLocation.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            cerr << " Error : " << strerror(errno) << endl;
        else
            cout << "Directory Created" << endl;
    }
    else
        cout << "Directory already exists" << endl;


    string ipb_fname = preprocessFileLocation + "/" + "ipb_" + 
                        reference_fname.substr( 18, reference_fname.size()-23) 
                        + "_" + 
                        t + ".txt";
=========
    
    if (ipb_fname.empty())
        ipb_fname = "ipb/ipb_" + 
                            reference_fname.substr(0, reference_fname.size() - 6) 
                            + "_" + 
                            target_fname.substr(0, target_fname.size() - 4) + ".txt";
>>>>>>>>> Temporary merge branch 2
    // clear the file
    cout << "ipb_fname: " << ipb_fname << "|" << endl;
    ofstream ofs;
    ofs.open(ipb_fname, std::ofstream::out | std::ofstream::trunc);
    ofs << asize << endl;
    ofs.close();

    auto tic = chrono::high_resolution_clock::now();
    double fsize = 0;
    double csize = 0;
    encode_target(target_fname, ipb_fname, fsize, csize);
    // cout << "INIT" << endl;
    auto toc = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::seconds>(toc - tic);

    // cout << "Alphabet size: " << asize << endl;
    // cout << "Default symbol size: " << default_symb_size << endl;
    // cout << "File size: " << fsize*default_symb_size << " bits" << endl;
    // cout << "Compressed size: " << csize << " bits" << endl;
    // cout << "File Size: " << fsize << " symbols" << endl;
    // cout << "Nº symbols not copied: " << count_not_copies << endl;
    // cout << "Nº symbols copied: " << count_copies << endl;
    // cout << "Average len of anchors: " << (double)count_copies/(double)tot_anchors << endl;
    // cout << "Total hits: " << global_hits << endl;
    // cout << "Total misses: " << global_misses << endl;
    // cout << "Nº of context hits: " << context_hits << endl;
    // cout << "Elapsed time: " << duration.count() << " seconds" << endl;
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
        if (c == '\n')
            c = ' ';
        c = tolower(c);
        alphabet.insert(c); 

        if (sequence.size() == k) {
            //append to the vector of the [sequence] the index of the next char
            positions[sequence].push_back(index);
            sequence.erase(0, 1);
        }
        index++;
        sequence += c;
        reference_string.push_back(c);
    }

    asize = alphabet.size();
    default_symb_size = ceil(log2(asize));

}

static void load_reference_preprocessed(string fname, string copy_model_fname, string context_model_fname) {
    ifstream infile(fname);
    if (!infile.is_open()) {
        cerr << "Failed to open input file: " << fname << endl;
        exit(EXIT_FAILURE);
    }
    // read file into reference_string
    char c;
    while (infile.get(c)) {
        reference_string.push_back(c);
    }
    infile.close();

    // load copy model
    ifstream infile2(copy_model_fname);
    if (!infile2.is_open()) {
        cerr << "Failed to open input file: " << copy_model_fname << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    getline(infile2, line);
    k = stoi(line);
    getline(infile2, line);
    asize = stoi(line);
    default_symb_size = ceil(log2(asize));
    while (getline(infile2, line)) {
        stringstream ss(line);
        string sequence;
        ss >> sequence;
        int index;
        while (ss >> index) {
            positions[sequence].push_back(index);
        }
    }
    infile2.close();

    // load context model
    ifstream infile3(context_model_fname);
    if (!infile3.is_open()) {
        cerr << "Failed to open input file: " << context_model_fname << endl;
        exit(EXIT_FAILURE);
    }
    getline(infile3, line);
    context_window_size = stoi(line);
    while (getline(infile3, line)) {
        stringstream ss(line);
        string sequence;
        ss >> sequence;
        char c;
        int sum = 0;
        while (ss >> c) {
            int freq;
            sum += freq;
            ss >> freq;
            context_table[sequence][string(1,c)] = freq;
        }
        context_table[sequence]["sum"] = sum;
    }
    infile3.close();
}

static void save_model(string fname) {
    ofstream ofs;
    ofs.open("models/copy/" + fname, std::ofstream::out | std::ofstream::trunc);
    ofs << k << endl;
    ofs << asize << endl;
    for (auto pair : positions) {
        ofs << pair.first << " ";
        for (int i : pair.second) {
            ofs << i << " ";
        }
        ofs << endl;
    }
    ofs.close();

    ofs.open("models/context/" + fname, std::ofstream::out | std::ofstream::trunc);
    ofs << context_window_size << endl;
    for (auto pair : context_table) {
        ofs << pair.first << " ";
        for (auto pair2 : pair.second) {
            ofs << "(" << pair2.first << " " << pair2.second << ") ";
        }
        ofs << endl;
    }
    ofs.close();
}

static void generate_context_model(string fname, long unsigned int k) {
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
        if (c == '\n')
            c = ' ';
        c = tolower(c);
        if (sequence.size() == k) {
            //append to the vector of the [sequence] the index of the next char
            if (context_table.count(sequence) < 1) {
                context_table[sequence] = map<string, int>();
                for (char c: alphabet) {
                    context_table[sequence][string(1,c)] = 1;
                }
            }
            context_table[sequence][string(1, c)] += 1;
            sequence.erase(0, 1);
        }
        index++;
        sequence += c;
    }

    // iterate through the context_table and calculate the sum of all the counts
    for (auto pair1 : context_table) {
        int sum = 0;
        for (auto pair2 : pair1.second) {
            sum += pair2.second;
        }
        context_table[pair1.first]["sum"] = sum;
    }

}

static void encode_target(string target_fname, string ipb_fname, double& fsize, double& csize) {
    ifstream infile(target_fname);
    if (!infile.is_open()) {
        cerr << "Failed to open input file: " << target_fname << endl;
        exit(EXIT_FAILURE);
    }
    // cout << "Encoding target..." << endl;

    // cout << context_table.size() << endl;
    // cout << positions.size() << endl;
    // cout << n_anchors << endl;

    string window = "";
    string testing_seq = "";
    vector<Anchor> anchors;   
    vector<double> info_per_byte;
    int offset = 0;

    char symbol;
    while (infile.get(symbol)) {
        symbol = tolower(symbol);
        fsize++;
        if (testing_seq.length() != 0) {
            bool ignored_last = false;              // If current symbol should be ignored by the copy model and be encoded normally
            if (update_anchors(symbol, offset, ignored_last, anchors)) {
                select_best_anchor(anchors, symbol, ignored_last, csize, info_per_byte);
                offset = 0;
                testing_seq = "";
            }
            offset++;
        }
        else {
            count_not_copies++;
            double info;
            if (context_table.count(context_window) > 0 && context_table[context_window].count(string(1,symbol)) > 0) {
                double prob = (double)context_table[context_window][string(1,symbol)]/(double)context_table[context_window]["sum"];
                //cout << symbol << " " << prob << " " <<  context_table[context_window][string(1,symbol)] << " " << context_table[context_window]["sum"] <<  endl;
                //cout << prob << endl;
                info = -log2(prob);
                context_hits++;
            }
            else {
                info = default_symb_size;
            }

            csize += info;
            info_per_byte.push_back(info);
        }
        
        if (info_per_byte.size() >= 100) {
            write_array_to_file(info_per_byte, ipb_fname);
            info_per_byte.clear();
        }


        // Slide window
        window += symbol;
        context_window += symbol;

        if (context_window.length() > context_window_size) {
            context_window.erase(0, 1);
        }

        if (window.size() == k) {
            if (positions.count(window) > 0 && testing_seq.length() == 0) {
                // Create Anchor

                testing_seq = window;
                auto it = positions.find(testing_seq);
                if (positions.find(testing_seq) != positions.end()) {
                    // Iterate through the vector in reverse order
                    vector<int> position = it->second;
                    // Save the n-th most recent anchors
                    int i = 0;
                    for (auto rit = position.rbegin(); rit != position.rend(); ++rit) {
                        // Update the value of each element in the struct
                        if (i == n_anchors) {
                            break;
                        }
                        // Initialize a new anchor
                        Anchor new_anchor;
                        new_anchor.position = *rit;
                        new_anchor.is_active = true;
                        for (char c: alphabet) {
                            new_anchor.sum_info_per_symbol.insert(make_pair(c, 0));
                        }
                        anchors.push_back(new_anchor);
                        i++;
                    }
                }
            }
            // Erase last symbol
            window.erase(0, 1);
        }
    }
    //ofs.close();
    if (testing_seq.length() > 0) {
        int active_count = 0;
        for (Anchor& anchor: anchors) {
            if (anchor.is_active && anchor.length > 0) {
                active_count++;
                break;
            }
        }
        if (active_count > 0)
            select_best_anchor(anchors, symbol, false, csize, info_per_byte);
    }
    write_array_to_file(info_per_byte, ipb_fname);
}

static double estimate_probability(int hits, int misses, double alpha) {
    return (hits + alpha)/(hits+misses+2*alpha);
}

static bool update_anchors(char symbol, int offset, bool& ignored_last, vector<Anchor>& anchors) {
    int active_count = 0;                                          // Count the number of anchor active

    for (Anchor& anchor : anchors) {
        if (!anchor.is_active) {
            continue;
        }
        active_count++;
        anchor.length++;
        char predicted_symb = reference_string[anchor.position + offset];                      // Next symbol in the anchor sequence
        double p_curr_symb;
        p_curr_symb = estimate_probability(anchor.hits, anchor.misses, alpha);  // Probability of the predicted symbol

        double info;
        if (predicted_symb == symbol) {
            info = -log2(p_curr_symb);
            anchor.hits++;
        }
        else {
            double prob;
            // cout << context_window << " " << context_table.count(context_window) << endl;
            //if (false) {
            if (context_table.count(context_window) > 0 && context_table[context_window].count(string(1,symbol)) > 0) {
                prob = (double)context_table[context_window][string(1,symbol)]/(double)context_table[context_window]["sum"];
                //cout << symbol << " " << prob << " " <<  context_table[context_window][string(1,symbol)] << " " << context_table[context_window]["sum"] <<  endl;
                context_hits++;
            }
            else {
                prob = (double)(1 - p_curr_symb)/ (double)(asize - 1);
            }
            info = -log2(prob);
            anchor.misses++;
        }
        anchor.sum_info += info;
        if (info < 0) {
            cerr << "NEGATIVE INFO GAIN??" << endl;
            // cout << "!!!! INFO: " << info << " " << anchor.hits << " " << anchor.misses << endl;
            exit(EXIT_FAILURE);
        }
        anchor.sum_info_per_symbol[predicted_symb] += info;
        anchor.info_per_byte.emplace_back(info);

        if (anchor.length > min_size) {
            // Check if the anchor has a hit/miss ratio that passes the threshold
            double acc = (double)anchor.hits/(double)(anchor.hits + anchor.misses);
            // cout << acc << " " << threshold << endl;
            if (acc < threshold) {
                anchor.is_active = false;
                active_count--;
                // Remove the last guess
                if (ignore1) {
                    anchor.misses--;
                    anchor.length--;
                    anchor.info_per_byte.pop_back();
                    anchor.sum_info_per_symbol[predicted_symb] -= info;
                    anchor.sum_info -= info;
                    ignored_last = true; 
                }
                
            }
        }

        // Deactivate anchor because of max_size
        if (anchor.length >= max_size) {
            anchor.is_active = false;
            active_count--;
        }
    }

    return active_count == 0;
}

static void select_best_anchor(vector<Anchor>& anchors, char byte, bool ignored_last, double& csize, vector<double>& info_per_byte) {
    double max_acc = 0;                                 // best accuracy
    int max_len = 0;                                    // longest sequence of symbols encoded
    double best_accumulative_info = 0;                  // accumulative information gain for the best sequence
    unordered_map<char, double> best_info;              // information gain of each symbol for the best sequence
    vector<double> best_info_per_byte;
    int best_hits, best_misses;
    for(Anchor& anchor: anchors) {
        if (anchor.length < 1) {
            continue;
        }
        int hits = anchor.hits;
        int misses = anchor.misses;
        double ps = estimate_probability(hits, misses, alpha);
        // Select the sequence who encode the most and with better accuracy
        if (anchor.length > max_len || (anchor.length == max_len && ps > max_acc)) {
            max_acc = ps;
            best_info = anchor.sum_info_per_symbol;
            max_len = anchor.length;
            best_accumulative_info = anchor.sum_info;
            best_info_per_byte = anchor.info_per_byte;
            best_hits = hits;
            best_misses = misses;
        }
    }

    // Anchor encoded some bytes
    if (max_len > 0) {
        // sum_acc += max_acc;
        // sum_len_copies += max_len;
        count_copies += max_len;
        csize += best_accumulative_info;
        global_hits += best_hits;
        global_misses += best_misses;
        tot_anchors++;
        // copies += max_len;
        // for (const auto &it : best_info) {
        //     auto it2 = global_info.find(it.first);
        //     if (it2 != global_info.end()) {
        //         it2->second += it.second;
        //     } 
        //     else {
        //         global_info.emplace_hint(it2, it.first, it.second);
        //     }
        // }
        info_per_byte.insert(info_per_byte.end(), best_info_per_byte.begin(), best_info_per_byte.end());


        // Update 
        if (ignored_last) {
            // info_per_byte.push_back(default_symb_size);
            // global_info[byte] += default_symb_size;
            csize += default_symb_size;
            count_not_copies++;        
        }
    }
    else {
        count_not_copies++;
        csize += default_symb_size;
        // global_info[byte] += default_symb_size;
        info_per_byte.push_back(default_symb_size);
    }

    anchors.clear();
}


static void write_array_to_file(vector<double>& array, string fname) {
    ofstream file;
    file.open(fname, ios::out | ios::app); 
    if (!file.is_open()) {
        cerr << "Error opening file " << fname << endl;
        exit(EXIT_FAILURE);
    }

    for (double val : array) {
        file << val << endl;
    }
    file.close();
}



static void parse_command_line(int argc, char** argv) {
    bool has_r = false;
    int c;                          // Opt process
    while ((c = getopt(argc, argv, "k:st:a:m:M:n:ip")) != -1) {
        switch (c)
        {
            case 'r':
                ipb_fname = optarg;
                ipb_fname = "ipb/" + ipb_fname;
                break;
            case 'p':
                is_preprocessed = true;
                context_model_fname = optarg;
                copy_model_fname = optarg;
                context_model_fname = "models/context/" + context_model_fname;
                copy_model_fname = "models/copy/" + copy_model_fname;
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
                context_model_fname = optarg;
                copy_model_fname = optarg;
                context_model_fname = "models/context/" + context_model_fname;
                copy_model_fname = "models/copy/" + copy_model_fname;
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
    cout << "Nº of Anchors = " << n_anchors << endl;
    cout << "Max Size = " << max_size << endl;
    cout << "Min Size = " << min_size << endl;
    cout << "Alpha = " << alpha << endl;
    cout << "Threshold = " << threshold << endl;
    cout << "Save = " << save << endl;
    cout << "Ignore Last = " << ignore1 << endl;
    cout << "K = " << k << endl;

    if (argc - optind < 2) {
        // cout << "Insert reference and target files" << endl;
        exit(EXIT_FAILURE);
    }

    reference_fname = argv[optind];
    target_fname = argv[optind+1];

    cout << "Reference file = " << reference_fname << endl;
    cout << "Target file = " << target_fname << endl;
}