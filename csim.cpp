#include <iostream>
#include <vector>

using namespace std;

string convertToBinary(int input) {
    string output;
    for (int i = 0; i < 32; i++) {
        output = (input % 2 == 0 ? "0":"1") + output; 
        input /= 2;
    }
    return output;
}

int main() {
    int originaladdress;
    cin >> hex >> originaladdress;
    string address = convertToBinary(originaladdress);
    vector<vector<unsigned>> cache;
    cout << address << endl;

    
}