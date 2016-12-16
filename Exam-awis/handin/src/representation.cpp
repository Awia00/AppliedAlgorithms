#include<queue>
#include<iostream>
#include<math.h>
#include<algorithm>
using namespace std;

class Interval{
    public:
        long start;
        long end;
        long id;
        Interval(long s, long e, long i) : start(s), end(e), id(i) {}
};

const auto cmpStartLambda = 
        [](const Interval a, const Interval b)
        {
            if (b.start == a.start)
                return a.id > b.id;
            return b.start > a.start;
        };

const auto cmpEndLambda = 
        [](const Interval a, const Interval b)
        {
            if (b.start == a.start)
                return a.id > b.id;
            return b.end < a.end;
        };

void pqsort()
{
    unsigned long n;
    cin >> n;

    vector<Interval> intervalVector;
    intervalVector.reserve(n);


    priority_queue<
        Interval, vector<Interval>,
        decltype(cmpEndLambda)>
        pq(cmpEndLambda);

    // Read in
    double s, e;
    for(unsigned long i = 0; i<n; i++)
    {
        cin>>s;
        cin>>e;
        long start = ceil(s);
        long end = (int)e;
        if(start <= end)
            intervalVector.emplace_back(start, end, i);
    }
    if (intervalVector.empty())
    {
        cout << "impossible" << endl;
                return;
    }
    sort(intervalVector.begin(), intervalVector.end(), cmpStartLambda);

    long resultArr[n];
    unsigned long index = 0;
    while( index < intervalVector.size())
    {
        Interval interval = intervalVector.at(index++);

        long currentTime = interval.start;
        
        pq.push(interval);
        while(!pq.empty())
        {
            for(unsigned long i = index; i<intervalVector.size(); i++)
            {
                Interval elem = intervalVector.at(index);
                if(elem.start == currentTime)
                {
                    pq.push(elem);
                    index++;
                }
                else
                    break;
            }
            Interval e = pq.top();
            pq.pop();
            if(e.start <= currentTime && currentTime <= e.end )
            {
                resultArr[e.id] = currentTime;
                currentTime++;
            }
            else
            {
                cout << "impossible" << endl;
                return;
            }
        }
    }
    
    for(unsigned long i = 0; i<n; i++)
    {
        cout << resultArr[i] << " ";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    pqsort();
}