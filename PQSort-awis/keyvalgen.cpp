#include<queue>
#include<iostream>
#include<algorithm>
using namespace std;

int main(int argv, char **argc)
{
    if(argv < 4)
    {
        cout << "Provide: number, seed, limit.\n";
        return 1;
    }
    int n, seed, limit;
        n = atoi(argc[1]); 
        seed = atoi(argc[2]); 
        limit = atoi(argc[3]);

    cout << n << "\n";
    srand(seed);

    for(int i = 1; i<=n; i++)
    {
        cout << (rand() % limit) << " " << (rand() % limit) << "\n";
    }
}