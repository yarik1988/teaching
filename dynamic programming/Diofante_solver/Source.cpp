#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <conio.h>
using namespace std;

void insert_parameters(std::string filename,std::vector<int> &a,int &b)
    {
        ifstream fin;
        fin.open(filename);
        int i;
        while (!fin.eof())
            {
            fin>>i;
            a.push_back(i);
            }
        b=a.back();
        a.pop_back();
        fin.close();
    }

void inc_x(std::vector<int> a,int b,std::vector<int> &x,int pos)
{
int val=0;
for (int i=0;i<pos;i++)
val+=a[i]*x[i];
if (val+a[pos-1]<=b||pos==1) x[pos-1]++;
else
    {
    x[pos-1]=0;
    inc_x(a,b,x,pos-1);
    }

}

bool get_last(std::vector<int> a,int b,std::vector<int> &x)
{
int val=0;
for (int i=0;i<a.size()-1;i++)
val+=a[i]*x[i];
val=b-val;
float last=(float)val/(float)a.back();
if (last==(int)last)
{
  x.back()=last;
  return true;
}
else return false;
}

std::vector<int> solve_problem(std::vector<int> a,int b)
{
std::vector<int> x(a.size(),0);
int pos=a.size()-1;
while ((x[0]*a[0]<=b)&&(get_last(a,b,x)==false))
        inc_x(a,b,x,a.size()-1);

if (x[0]*a[0]>b) x.clear();
return x;
}

int main()
{
	std::vector<int> a;
	int b;
	insert_parameters("../input.txt",a,b);
	std::vector<int> x=solve_problem(a,b);
	if (x.size()>0)
    {
        for (int i=0;i<x.size();i++)
            {
            cout<<a[i]<<"*"<<x[i];
            if (i<x.size()-1) cout<<"+";
            }
        cout<<"="<<b;
    }
    else cout<<"No answer";
}
