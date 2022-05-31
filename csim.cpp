#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;

string convertToBinary(int input) { //converts int to 32 bit binary string
    string output;
    for (int i = 0; i < 32; i++) {
        output = (input % 2 == 0 ? "0":"1") + output; 
        input /= 2;
    }
    return output;
}

unsigned convertToDecimal(string binary) { //converts binary string to unsigned int
    if (binary == "") return 0;
    unsigned output = 0;
    for (int i = binary.size()-2; i >= 0; i--) {
        output += pow((stoi(binary.substr(i, 1)) == 1 ? 2:0), binary.size()-1 - i);
    }
    output += stoi(binary.substr(binary.size()-1, 1));
    return output;
}

string CacheToString(vector<string> cache) { //Converts the array of strings to a single output string
    string output = "", temp;
    for (string i : cache) {
        output += "[";
        stringstream stream(i);
        getline(stream, temp, ',');
        output += temp;
        while(stream.good()) {
            getline(stream, temp, ',');
            if (temp != "") output += ", " + temp;
        }
        output += "]";
    }
    return output;
}

// This does all the data analysis to find the set, tag, and block offset
int Write(vector<string> &cache, string address, int setBits, int maxSetSize, int offset, int &blockOffset, int &set, string &tag) {
    blockOffset = convertToDecimal(address.substr(30-offset, offset));
    set = convertToDecimal(address.substr(30-setBits, setBits-offset));
    tag = to_string(convertToDecimal(address.substr(0, 30-setBits)));
    if (("," + cache[set]).find("," + tag + ",") != -1) return 1;
    if (count(cache[set].begin(), cache[set].end(), ',') < maxSetSize) cache[set] += tag + ",";
    else {
        cache[set].erase(0, cache[set].find(',')+1);
        cache[set] += tag + ",";
    }
    return 0;
}

// Runs through all the data twice and properly uses the other functions
void RunAnalysis(vector<string> inputs, int b, int words, int sets) {
    int originaladdress;
    int hits = 0, blkoffs = words;
    vector<string> cache(sets);

    for (int z = 1; z <= 2; z++) {
        hits = 0;
        for (string i : inputs) {
            stringstream str;
            str << i;
            str >> hex >> originaladdress; //convert from hex to dec
            int hit, set;
            string address = convertToBinary(originaladdress), tag;
            hit = Write(cache, address, log2(16/b), b, words, blkoffs, set, tag);
            hits += hit;
            cout << "Address:" << i << "\t\t" //Prints all the data
                 << "BlkOfs:" << blkoffs << "\t\t"
                 << "Set:" << set << "\t"
                 << "Tag:" << tag << "\t"
                 << "Hit:" << (hit ? "true":"false") << "\t\t"
                 << "Cache:" << CacheToString(cache) << endl;
        }
        int hitsstr = 0.5 + (double)hits/inputs.size() * 100; //calculates the hit percentage
        cout << "Pass " << z << " hits:" << hits << " = " << hitsstr << "%" << endl; //Prints the passes and percentages
        cout << "Pass " << z << " misses:" << (inputs.size()-hits) << " = " << (100 - hitsstr) << "%" << endl << endl;
    }
}

int main() {
    // All the data from 8.9
    vector<string> s1 = {"40", "44", "48", "4c", "70", "74", "78", "7c", "80", "84", "88", "8c", "90", "94", "98", "9c", "0", "4", "8", "c", "10", "14", "18", "1c", "20"};
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9a" << endl;
    RunAnalysis(s1, 1, 0, 16); //direct mapped cache, b = 1 word

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9b" << endl;
    RunAnalysis(s1, 16, 0, 1); //fully associative cache, b = 1 word

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9c" << endl;
    RunAnalysis(s1, 2, 0, 8); //two-way set associative cache, b = 1 word

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9d" << endl;
    RunAnalysis(s1, 1, 1, 8); //direct mapped cache, b = 2 words

    // All the data from 8.10
    vector<string> s2 = {"74", "A0", "78", "38C", "AC", "84", "88", "8C", "7C", "34", "38", "13C", "388", "18C"};
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.10a" << endl;
    RunAnalysis(s2, 1, 0, 16); //direct mapped cache, b = 1 word

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.10b" << endl;
    RunAnalysis(s2, 8, 1, 1); //fully associative cache, b = 2 words

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.10c" << endl;
    RunAnalysis(s2, 2, 1, 4); //two-way set associative cache, b = 2 words

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.10d" << endl;
    RunAnalysis(s2, 1, 2, 4); //direct mapped cache, b = 4 words
}