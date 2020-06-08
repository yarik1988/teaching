#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <conio.h>


using namespace std;

const int nmax = 10000;
int mas[nmax];

void Gen(int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; ++i )
        mas[i]=rand() % 1000;
}

float Mid(int l, int r)
{
    int sum=0;
    for (int i = l; i <= r;++i)
        sum+=mas[i];
        if (r-l+1 == 0) return 20;
    return sum / float(r-l+1);
}

void swap_(int &a, int &b)
{
    int t = b;
    b = a;
    a = t;

}

void qsort( int l, int r)
{
    float mid = Mid(l,r);
    int i=l, j=r;
    while (i<=j)
    {
        while (mas[i]<= mid) ++i;
        while (mas[j]>= mid) --j;
        if (i<=j) {
            swap_(mas[i],mas[j]);
            ++i;--j;
        }
    }
    if (i<r) qsort (i,r);
    if (l<j) qsort (l,j);
}
void Tell_Out(int n)
{
    for (int i = 0; i < n; ++i)
    {
        if (i % 10 == 0 ) cout<<endl;
        cout << mas[i]<<" ";

    }
    cout<<endl;
}

int main()
{

    int n = 100;
    Gen(n);
    Tell_Out(n);
    qsort(0,n-1);
    Tell_Out(n);
}
