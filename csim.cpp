#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;

string convertToBinary(int input) {
    string output;
    for (int i = 0; i < 32; i++) {
        output = (input % 2 == 0 ? "0":"1") + output; 
        input /= 2;
    }
    return output;
}

unsigned convertToDecimal(string binary) {
    if (binary == "") return 0;
    unsigned output = 0;
    for (int i = binary.size()-2; i >= 0; i--) {
        output += pow((stoi(binary.substr(i, 1)) == 1 ? 2:0), binary.size()-1 - i);
    }
    output += stoi(binary.substr(binary.size()-1, 1));
    return output;
}

string CacheToString(vector<string> cache) {
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

void RunAnalysis(vector<string> inputs, int b, int words, int sets) {
    int originaladdress;
    int hits = 0, blkoffs = words;
    vector<string> cache(sets);

    for (int z = 1; z <= 2; z++) {
        hits = 0;
        for (string i : inputs) {
            stringstream str;
            str << i;
            str >> hex >> originaladdress;
            int hit, set;
            string address = convertToBinary(originaladdress), tag;
            hit = Write(cache, address, log2(16/b), b, words, blkoffs, set, tag);
            hits += hit;
            cout << "Address:" << i << "\t\t"
                 << "BlkOfs:" << blkoffs << "\t\t"
                 << "Set:" << set << "\t"
                 << "Tag:" << tag << "\t"
                 << "Hit:" << (hit ? "true":"false") << "\t\t"
                 << "Cache:" << CacheToString(cache) << endl;
        }
        int hitsstr = (double)hits/inputs.size() * 100;
        cout << "Pass " << z << " hits:" << hits << " = " << to_string((int)((double)hits/inputs.size() * 100)) << "%" << endl;
        cout << "Pass " << z << " misses:" << (inputs.size()-hits) << " = " << (int)((double)(inputs.size()-hits)/inputs.size() * 100) << "%" << endl << endl;
    }
}

int main() {
    vector<string> s1 = {"40", "44", "48", "4c", "70", "74", "78", "7c", "80", "84", "88", "8c", "90", "94", "98", "9c", "0", "4", "8", "c", "10", "14", "18", "1c", "20"};
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9a" << endl;
    RunAnalysis(s1, 1, 0, 16);

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9b" << endl;
    RunAnalysis(s1, 16, 0, 1);

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9c" << endl;
    RunAnalysis(s1, 2, 0, 8);

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Problem 8.9d" << endl;
    RunAnalysis(s1, 1, 1, 8);

    vector<string> s2 = {"74", "A0", "78", "38C", "AC", "84", "88", "8C", "7C", "34", "38", "13C", "388", "18C"};

}