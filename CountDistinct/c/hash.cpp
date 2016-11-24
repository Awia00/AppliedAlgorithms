#include <iostream>
#include <string>
#include <stdexcept>
#include "hashlib.h"

using namespace std;

int main() {
    std::ios::sync_with_stdio(false);
    int na, nb;
    try {
        while (cin >> na >> nb) {
            cout << h(na) << endl;
            cout << h(nb) << endl;
        }
    } catch (const invalid_argument& ia) {
        // ignore
    }
    return 0;
}
