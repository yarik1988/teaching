#include <iostream>
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>
#define NN 6
#include <clocale>
#include <time.h>
void out(float **a,int n)
{
printf("\n");
for(int i=0;i<n;i++)
{
for(int j=0;j<n;j++) printf("%6.2f\t",a[i][j]);
printf("\n");
}
}

float det(float **a,int n)
{
float kst;
int p=0;
for (int i=0; i<n-1; i++)
{
int t=1;

while(a[i][i]==0&&i+t<n)
{
for(int j=0; j<n; j++)
{
kst=a[i][j];
a[i][j]=a[i+t][j];
a[i+t][j]=kst;
}
p++;
t++;
}
if (i+t==n) return 0;

for (int k=i+1; k<n; k++)
{
kst=a[k][i]/a[i][i];
for(int j=0; j<n; j++)
a[k][j]-=a[i][j]*kst;
}
}

kst=pow(-1,p);
for(int i=0; i<n; i++)
kst*=a[i][i];

return kst;
}

int main()
{
setlocale( LC_ALL,"Russian" );
float kst;
int i,j,n,p,k=0,t;
char yn;


printf("Enter N: N=",NN);
std::cin>>n;
float **a = new float *[n];
for (int i = 0; i < n; i++)   a[i] = new float [n];

k=0;
while(k==0)
{
printf("Generate randomly? y/n: ");
yn=getche();
if ((yn=='y') || (yn=='n')) k=1;
else k=0;
}
switch(yn)
{
case 'y':srand(time(NULL)); for(i=0;i<n;i++) for(j=0;j<n;j++) a[i][j]=rand()%10;break;
default:
{

for(i=0;i<n;i++)
for(j=0;j<n;j++)
{
    printf("\nEnter element %d %d:  ",i,j);
    scanf("%f",&a[i][j]);
}
}printf("\n");
break;
}

out(a,n);
std::cout<<"Determinant value:"<<det(a,n);
}
