//Bell's number
#include <iostream>
#include <vector>
using namespace std;
int solver(vector<vector<int> > part, int curr_elem,int N)
{

if (curr_elem<=N)
{
int res=0;
for (int i=0;i<part.size();i++)
    {
    vector<vector<int> > temp=part;
    temp[i].push_back(curr_elem);
    res+=solver(temp,curr_elem+1,N);
    }
vector<vector<int> > temp=part;
vector<int> next_part;
next_part.push_back(curr_elem);
temp.push_back(next_part);
res+=solver(temp,curr_elem+1,N);
return res;
}
else
{
    for (int i=0;i<part.size();i++)
    {
    cout<<"{";
    for (int j=0;j<part[i].size();j++)
    cout<<" "<<part[i][j]<<" ";
    cout<<"}";
    }
    cout<<endl;
    return 1;
}

}

int main()
{
    int N;
    cout<<"enter N:";
    cin>>N;
    vector<vector<int> > part;
    int Part_count=solver(part,1,N);
    cout<<"Count of partititions="<<Part_count<<endl;
    return 0;
}
