#include <iostream>
#include <math.h>
#include <stdexcept>
#include "hashlib.h"

using namespace std;

double hyperLogLog(int m) {
    // for i:=0 to m-1 do M[i]:=0
    int *M = new int[m];

    // for i:=1 to n do
    //     j := f(y[i])
    //     M[j] := max(M[j],rho(h(y[i])))
    // end
    int yi, j, count;

    try {
        while (cin >> yi) {
            j = f(yi);
            count = h_first(yi);
            if (M[j] < count) {
                M[j] = count;
            }
        }
    }
    catch (const invalid_argument& ia) {
        // ignore
    }

    // Z := 1/(2^(-M[0])+...+2^(-M[m-1]))
    // V := |{i | M[i]=0|}|
    double Z = 0.0;
    double V = 0.0;
    for (int i = 0; i < m; i++) {
        Z+= (pow(2.0, -M[i]));
        if (M[i] == 0) V++;
    }
    Z = 1/Z;

    // E := m*m*Z*0.7213/(1 + 1.079/m)
    double E = m*m*Z*0.7213/(1.0 + 1.079/m);

//        if (E < 2.5*m and V > 0) then E:= m * ln(m/V)
    if (E < 2.5*m && V > 0.0) {
        E = m * log(m/V);
    }

//        return E
    return E;
}

int main() {
    std::ios::sync_with_stdio(false);
    int threshold;
    cin >> threshold;
    

    if (hyperLogLog(1024) < threshold) {
        cout << "below" << endl;
    } else {
        cout << "above" << endl;
    }

    return 0;
}