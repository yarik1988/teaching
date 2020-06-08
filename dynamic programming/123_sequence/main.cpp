#include <iostream>

using namespace std;
bool prov(int pos,char *arr)
 {
   for (int i=1;i<=1+pos/2;i++)
   {
    bool equ=true;
    for (int j=pos;j>pos-i;j--)
    if (arr[j]!=arr[j-i]) equ=false;
    if (equ==true) return false;
   }
 return true;
 }

 char* get123seq(int len)
 {
    char *res=new char[len];
    for (int i=0;i<len;i++) res[i]=0;
    int pos=0;
    while (pos<len)
    {
        do
        res[pos]++;
        while ((prov(pos,res)==false)&&res[pos]<=3);
        if (res[pos]==4) {res[pos]=0;pos--;} else pos++;
    }
    return res;
 }

int main()
{
    int len;
    cin>>len;
    char* res=get123seq(len);
    for (int i=0;i<len;i++)
        cout<<(int)res[i]<<" ";

    return 0;
}
