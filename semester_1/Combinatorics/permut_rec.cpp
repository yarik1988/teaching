#include <iostream>

using namespace std;

void Gen(int k,int *a,int n)
{
if (k<n-1)
for (int i=k;i<n;i++)
    {
    swap<int>(a[i],a[k]);
    Gen(k+1,a,n);
    swap<int>(a[i],a[k]);
    }
else
    {
    for (int i=0;i<n;i++) cout<<a[i]<<" ";
    cout<<endl;
    }
}

int main()
{
    int n;cin>>n;
    int *a=new int[n];
    for (int i=0;i<n;i++) a[i]=i+1;
    Gen(0,a,n);
    return 0;
}
