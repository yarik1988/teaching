#include <iostream>
#include <fstream>
#include <conio.h>
#include <iomanip>
double** mult_matr(double** a,double **b,int n)
{
double **res = new double *[n];
for (int i = 0; i < n; i++) res[i] = new double [n];
for (int i=0;i<n;i++)
for (int j=0;j<n;j++)
{
res[i][j]=0;
for (int k=0;k<n;k++) res[i][j]+=a[i][k]*b[k][j];
}
return res;
}

double** copy_mat(double**a,int n)
{
double **matrix = new double *[n];
for (int i = 0; i < n; i++)  matrix[i] = new double [n];
for (int i = 0; i < n; i++)  for (int j = 0; j < n; j++)  matrix[i][j]=a[i][j];
return matrix;
}

void out (double**a,int n)
{
for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            std::cout <<std::setw(6)<< a[i][j] << "  ";

        std::cout << std::endl;
    }
}

double det(double **mat,int n)
{
double **a=copy_mat(mat,n);
double kst;
double result=1;
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
result*=-1;
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

for(int i=0; i<n; i++)
result*=a[i][i];

    for (int i = 0; i < n; i++)
        delete [] a[i];

    delete [] a;

return result;
}

double** inversion(double **Mat, int N)
{
    double** A=copy_mat(Mat,N);
    double temp;
    double **E = new double *[N];

    for (int i = 0; i < N; i++)
        E[i] = new double [N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            E[i][j] = 0.0;

            if (i == j)
                E[i][j] = 1.0;
        }

    for (int k = 0; k < N; k++)
    {
        if (A[k][k]==0)
            {
            int l=0;
            while (l<N&&A[l][k]==0) l++;
            for (int s=0;s<N;s++) {A[k][s]+=A[l][s];E[k][s]+=E[l][s];}
            }

        temp = A[k][k];
        for (int j = 0; j < N; j++)
        {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }

        for (int i = k + 1; i < N; i++)
        {
            temp = A[i][k];

            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int k = N - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = A[i][k];

            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int i = 0; i < N; i++)
        delete [] A[i];

    delete [] A;

return E;
}

int main()
{
    int N;
    std::cin>>N;

    double **matrix = new double *[N];

    for (int i = 0; i < N; i++)
        matrix[i] = new double [N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)  std::cin >> matrix[i][j];
    out(matrix,N);
    double deter=det(matrix,N);
    std::cout<<"Determinant="<<deter<<std::endl;
    if (deter!=0)
    {
     double** inv_matr=inversion(matrix, N);
     out(inv_matr,N);
     std::cout<<"Determinant of inv. matr="<<det(inv_matr,N)<<std::endl;
     double** ddd=mult_matr(matrix,inv_matr,N);
     out(ddd,N);
    }

    for (int i = 0; i < N; i++)
        delete [] matrix[i];

    delete [] matrix;

    return 0;
}
