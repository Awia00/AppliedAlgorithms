#include <iostream>
#include <string>
#include <stdexcept>
#include "hashlib.h"

using namespace std;

int main() {
    std::ios::sync_with_stdio(false);
    int number;
    while (cin >> number) {
        cout << h(number) << endl;
    }
    return 0;
}