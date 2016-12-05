#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

class element {
    public:
        int key;
        int value;
        element(int k, int v) {
            key = k; value = v;
        }
};

void pqsort() {
    auto cmpKey = [](const element a, const element b) { return b.key < a.key; };

    std::priority_queue<element, std::vector<element>, decltype(cmpKey)> pq(cmpKey);

    int number;
    cin >> number;

    int key, value;
    for(int i = 0; i < number; i++) {
        cin >> key; cin >> value;

        pq.emplace(key, value);
    }

    for(int i = 0; i < number; i++) {
        element x = pq.top();
#ifndef CODEJUDGE
        cout << x.key << " " << x.value << endl;
#else
        cout << x.value << endl;
#endif
        pq.pop();
    }
}

void stdsort() {
    auto cmpKey = [](const element a, const element b) { return b.key > a.key; };

    int number;
    cin >> number;

    vector<element> myVector;
    myVector.reserve(number);

    int key, value;
    for(int i = 0; i < number; i++) {
        cin >> key >> value;
        myVector.emplace_back(key, value);
    }
    sort(myVector.begin(), myVector.end(), cmpKey);

    for(element x : myVector) {
#ifndef CODEJUDGE
        cout << x.key << " " << x.value << endl;
#else
        cout << x.value << endl;
#endif
    }
}

int main(int argc, char** argv) {
    if (argc == 1) {
        //pqsort();
        stdsort();
    } else {
        if (strcmp(argv[1], "pq") == 0) pqsort();
        else if (strcmp(argv[1], "sort") == 0) stdsort();
        else cout << "Try 'pq' or 'sort' :-(" << endl;
    }
}
