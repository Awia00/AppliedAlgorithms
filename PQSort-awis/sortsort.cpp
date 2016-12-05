#include<iostream>
#include<algorithm>
using namespace std;

class keyvalue{
    public:
        int key;
        int value;
        keyvalue(int k, int v) : key(k), value(v) {}
};
auto const cmpLambda = 
        [](const keyvalue a, const keyvalue b)
        {
            return b.key > a.key;
        };

void sortsort()
{
    int n, k, v;
    cin>>n;
    vector<keyvalue> keyValueVector;
    keyValueVector.reserve(n);

    for(int i=1; i<=(n>>1); i++) // from 1 to n/2 since we do two every round
    {
        cin>>k; 
        cin>>v;
        keyValueVector.emplace_back(k,v);
        cin>>k; 
        cin>>v;
        keyValueVector.emplace_back(k,v);
    }

    sort(keyValueVector.begin(), keyValueVector.end(), cmpLambda);

    for(keyvalue kv: keyValueVector)
    {
        cout << kv.value << endl;
    }
}


int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    sortsort();
}