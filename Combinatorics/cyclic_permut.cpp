#include <iostream>

using namespace std;

void print_permutation_based_on_cycle(int*a,int n)
{
int* ans=new int[n+1];
for (int i=0;i<n;i++) ans[a[i]]=a[(i+1)%n]+1;
for (int i=0;i<n;i++) cout<<ans[i]<<" ";
cout<<endl;
delete[] ans;
}


void Gen(int k,int *a,int n)
{
if (k<n-1)
for (int i=k;i<n;i++)
    {
    swap<int>(a[i],a[k]);
    Gen(k+1,a,n);
    swap<int>(a[i],a[k]);
    }
else print_permutation_based_on_cycle(a,n);
}

int main()
{
    int n;
    cin>>n;
    int *a=new int[n];
    for (int i=0;i<n;i++) a[i]=i;
    Gen(1,a,n);
    return 0;
}
