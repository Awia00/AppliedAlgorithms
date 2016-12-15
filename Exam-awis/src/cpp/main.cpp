#include<queue>
#include<iostream>
using namespace std;

class keyvalue{
    public:
        int key;
        int value;
        keyvalue(int k, int v) : key(k), value(v) {}
};

void question1()
{
    int inputInt;
    while (cin >> inputInt)
    {
        cout << inputInt << endl;
    }
}

void question3()
{
    string inputString;
    while(cin >> inputString)
    {
        cout << inputString << endl;
    }
}





int main(int argc, char **argv)
{
    question1();
    question3();
}