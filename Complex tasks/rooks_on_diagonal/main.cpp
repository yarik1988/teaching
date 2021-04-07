#include <iostream>
#include <cstring>
#include <unordered_set>
#include <map>

int solve(int N,int M)
{
    std::map<int,int> X_from_Y;
    for (int i=0;i<M;i++)
    {
        int x,y;
        std::cin>>x>>y;
        if (x!=y) X_from_Y[y]=x;
    }
    int res=0;
    while (!X_from_Y.empty())
    {
        auto it=X_from_Y.begin();
        int first=it->first;
        int next=first;
        while (true)
        {
            auto it=X_from_Y.find(next);
            if (it==X_from_Y.end()) break;
            next=it->second;
            X_from_Y.erase(it);
            res++;
        }
        if (next==first) res++;
    }

return res;
}


int main() {
    int T=0;
    std::cin>>T;
    for (int t=0;t<T;t++)
    {
        int N,M;
        std::cin>>N>>M;
        std::cout<<solve(N,M);
    }
    return 0;
}
