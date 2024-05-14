#include <iostream>
#include <algorithm>
using namespace std;

bool compare(int* a,int *b,int s)
{
while (*a==*b&&a!=NULL) {a++;b++;}
if (*a>=*b) return true;
else return false;
}

int main()
{
    string a,b;
    cout << "Please input a" <<endl;
    cin>>a;
    cout << "Please input b" <<endl;
    cin>>b;

    int Mx=std::max(a.length(),b.length());
    cout<<a.length()<<"\n";
    cout<<b.length()<<"\n";
    int* aArray = new int[Mx];
    int* bArray = new int[Mx];
    int* bRes = new int[Mx];
    for (int i=0;i<Mx;i++) {aArray[i]=0;bArray[i]=0;bRes[i]=0;}
    for (int i=0; i<a.length(); i++) aArray[i+Mx-a.length()]=a[i]-'0';
    for (int i=0; i<b.length(); i++) bArray[i+Mx-b.length()]=b[i]-'0';
    for (int i=0;i<Mx;i++) cout<<aArray[i];
    cout<<"\n";
    for (int i=0;i<Mx;i++) cout<<bArray[i];
    cout<<endl;
    if (compare(aArray,bArray,Mx))
    cout<<"a>b\n"; else cout<<"a<b\n";

    int c;
    bool isSwap=false;
    if (!compare(aArray,bArray,Mx))
    for (int i=0;i<Mx;i++) {c=aArray[i];aArray[i]=bArray[i];bArray[i]=c;isSwap=true;}

    int ost=0;
    for (int i=Mx-1;i>=0;i--)
    {
    if (aArray[i]-ost>=bArray[i]) {bRes[i]=(aArray[i]-ost-bArray[i]);ost=0;}
    else {bRes[i]=10+aArray[i]-ost-bArray[i];ost=1;}
    }

    if (isSwap) bRes[0]=-bRes[0];
    for (int i=0;i<Mx;i++) cout<<bRes[i];
    delete[] aArray;
    delete[] bArray;
    delete[] bRes;

    return 0;
}
