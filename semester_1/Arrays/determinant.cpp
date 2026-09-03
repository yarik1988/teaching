// Determinant of a square matrix by Gaussian elimination with partial pivoting.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>

double** allocate_matrix(int n)
{
    double **a = new double *[n];
    for (int i = 0; i < n; i++) a[i] = new double [n];
    return a;
}

void free_matrix(double **a, int n)
{
    for (int i = 0; i < n; i++) delete [] a[i];
    delete [] a;
}

double** copy_matrix(double **a, int n)
{
    double **res = allocate_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) res[i][j] = a[i][j];
    return res;
}

void out(double **a, int n)
{
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) std::cout << std::setw(8) << a[i][j];
        std::cout << std::endl;
    }
    std::cout << std::defaultfloat << std::setprecision(6);   // back to the stream defaults
}

// Eliminates on a copy, so the caller's matrix is left untouched.
double det(double **mat, int n)
{
    double **a = copy_matrix(mat, n);
    double result = 1;
    int sign = 1;

    for (int i = 0; i < n; i++)
    {
        // partial pivoting: the largest element of the column keeps rounding errors small
        int pivot = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(a[k][i]) > fabs(a[pivot][i])) pivot = k;

        if (a[pivot][i] == 0)   // nothing to pivot on => the matrix is singular
        {
            free_matrix(a, n);
            return 0;
        }

        if (pivot != i)
        {
            std::swap(a[i], a[pivot]);   // swapping row pointers is enough
            sign = -sign;
        }

        for (int k = i + 1; k < n; k++)
        {
            double kst = a[k][i] / a[i][i];
            for (int j = i; j < n; j++) a[k][j] -= a[i][j] * kst;
        }

        result *= a[i][i];
    }

    free_matrix(a, n);
    return sign * result;
}

int main()
{
    int n;
    std::cout << "Enter N: N=";
    if (!(std::cin >> n) || n < 1)
    {
        std::cout << "N must be a positive integer" << std::endl;
        return 1;
    }

    double **a = allocate_matrix(n);

    char yn = 0;
    while (yn != 'y' && yn != 'n')
    {
        std::cout << "Generate randomly? y/n: ";
        if (!(std::cin >> yn)) return 1;
    }

    if (yn == 'y')
    {
        srand((unsigned)time(nullptr));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) a[i][j] = rand() % 10;
    }
    else
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                std::cout << "Enter element " << i << " " << j << ": ";
                if (!(std::cin >> a[i][j]))
                {
                    std::cout << "Not a number" << std::endl;
                    free_matrix(a, n);
                    return 1;
                }
            }
    }

    std::cout << std::endl;
    out(a, n);
    std::cout << "Determinant value: " << det(a, n) << std::endl;

    free_matrix(a, n);
    return 0;
}
