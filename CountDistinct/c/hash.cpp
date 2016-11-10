#include <iostream>
#include <string>
#include <stdexcept>
#include "hashlib.h"

using namespace std;

int main() {
    try {
        for (string line; getline(cin, line);) {
                cout << h(stoi(line)) << endl;
        }
    } catch (const invalid_argument& ia) {
        return 0;
    }
    return 0;
}