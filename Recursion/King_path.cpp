#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <conio.h>
#include <time.h>
using namespace std;

struct chesssmatr
{
int** matr;
int n;
int count_borders=0;
void gen(int m_size)
    {
    n=m_size;
    matr=new int*[n];
    for (int i=0;i<n;i++)
        matr[i]=new int[n];

    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        if (rand()%5==0) matr[i][j]=-1;
        else matr[i][j]=0;
    matr[0][0]=0;
    count_borders=0;
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        if (matr[i][j]==-1) count_borders++;
    }

void display()
{
for (int i=0;i<n;i++)
{
     for (int j=0;j<n;j++)
        cout<<setw(5)<<matr[i][j];
        cout<<endl;
}
cout<<endl;
}

};



void king_rush(chesssmatr &mt,int x,int y,int depth,bool& proceed)
{
  if (x<0||y<0||x>=mt.n||y>=mt.n) return;

  if (depth==mt.n*mt.n-mt.count_borders&&x==0&&y==0)
        {
        mt.display();
        proceed=false;
        }
  if (mt.matr[x][y]!=0) return;
  mt.matr[x][y]=depth+1;
  if (proceed) king_rush(mt,x+1,y,depth+1,proceed);
  if (proceed) king_rush(mt,x-1,y,depth+1,proceed);
  if (proceed) king_rush(mt,x,y+1,depth+1,proceed);
  if (proceed) king_rush(mt,x,y-1,depth+1,proceed);
  if (proceed) king_rush(mt,x+1,y-1,depth+1,proceed);
  if (proceed) king_rush(mt,x-1,y+1,depth+1,proceed);
  if (proceed) king_rush(mt,x+1,y+1,depth+1,proceed);
  if (proceed) king_rush(mt,x-1,y-1,depth+1,proceed);
  mt.matr[x][y]=0;
}

int main()
{
    int n=0;
    chesssmatr mt;
    cin>>n;
    srand(time(NULL));
    mt.gen(n);
    mt.display();
    bool proceed=true;
    king_rush(mt,0,0,0,proceed);
    return 0;
}
