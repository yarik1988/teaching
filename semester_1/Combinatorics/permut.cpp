#include <iostream>
#include <conio.h>
using namespace std;

void show(int* a, int n)
{
for (int i=0;i<n;i++) cout<<a[i];
cout<<", ";
}

int main()
{
    int n;
    cin>>n;
    int *a=new int[n];
    for (int i=0;i<n;i++) a[i]=i+1;
    show(a,n);
    bool ostanov=false;
    while (ostanov==false)
    {
    int     i=n-2;
    while (a[i]>a[i+1]&&i>=0) i--;
    if (i==-1) ostanov=true;
    else
        {
         int j=i;
         while (j<n-1&&a[i]<a[j+1]) j++;
         std::swap<int>(a[i],a[j]);
         int k=(n-i)/2;
         for (int j=1;j<=k;j++) std::swap<int>(a[i+j],a[n-j]);
        show(a,n);
        }
    }

   delete[] a;
   getch();
   return 0;

}
