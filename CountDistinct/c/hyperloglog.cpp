#include <iostream>
#include <string>
#include <stdexcept>
#include <math.h>
#include "hashlib.h"

using namespace std;

int max(int x, int y) {
    return x < y ? y : x;
}

int rho(int hx) {
    // ρ(h(x)) = min{i | h(x)k−i = 1}
    int rho = 0;
    for (int j = 1 << 30; j > 0; j >>= 1) {
        rho++;
        if ((j & hx) > 0) {
            return rho;
        }
    }
    return rho;
}

double hyperLogLog(int m) {
    int M[m];

    // for i:=0 to m-1 do M[i]:=0
    for (int i = 0; i < m; i++) {
        M[i] = 0;
    }

    // for i:=1 to n do
    //     j := f(y[i])
    //     M[j] := max(M[j],rho(h(y[i])))
    // end
    try {
        for (string line; getline(cin, line);) {
            int yi = stoi(line);
            int j = f(yi);
            M[j] = max(M[j], __builtin_clz(h(yi)) + 1);
        }
    }
    catch (const invalid_argument& ia) {
        // Ignore.
    }

    // Z := 1/(2^(-M[0])+...+2^(-M[m-1]))
    // V := |{i | M[i]=0|}|
    double Z = 0;
    double V = 0;
    for (int i = 0; i < m; i++) {
        Z+= (pow(2, -M[i]));
        if (M[i] == 0) V++;
    }
    Z = 1/Z;

    // E := m*m*Z*0.7213/(1 + 1.079/m)
    double E = m*m*Z*0.7213/(1 + 1.079/m);

//        if (E < 2.5*m and V > 0) then E:= m * ln(m/V)
    if (E < 2.5*m && V > 0) {
        E = m * log(m/V);
    }

//        return E
    return E;
}

int main() {
    string line;
    getline(cin, line);
    int threshold = stoi(line);
    
    double result = hyperLogLog(1024);

    if (result < threshold) {
        cout << "below" << endl;
    } else {
        cout << "above" << endl;
    }

    return 0;
}