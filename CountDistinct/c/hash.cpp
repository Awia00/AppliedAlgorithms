#include <iostream>
#include <string>
#include <stdexcept>
#include "hashlib.h"

using namespace std;

int main() {
    int number;
    while (scanf("%d", &number) == 1) {
        cout << h(number) << endl;
    }
    return 0;
}