//https://codeforces.com/problemset/problem/1465/C
/*
You are given a n×n chessboard. Rows and columns of the board are numbered from 1 to n. Cell (x,y)
lies on the intersection of column number x and row number y.
Rook is a chess piece, that can in one turn move any number of cells vertically or horizontally. There are m rooks (m<n) placed on the chessboard in such a way that no pair of rooks attack each other. I.e. there are no pair of rooks that share a row or a column.
In one turn you can move one of the rooks any number of cells vertically or horizontally. Additionally, it shouldn't be attacked by any other rook after movement. What is the minimum number of moves required to place all the rooks on the main diagonal?
*/
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
