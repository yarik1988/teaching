#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <conio.h>
using namespace std;
void Gen(int *mas, int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; ++i )
        mas[i]=rand() % 1000;
}

void swap_(int &a, int &b)
{
    int t = b;
    b = a;
    a = t;

}

void Tell_Out(int*mas, int n)
{
    for (int i = 0; i < n; ++i)
    {
        if (i % 10 == 0 ) cout<<endl;
        cout << mas[i]<<" ";

    }
    cout<<endl;
}

int partition(int arr[], int start, int end)
{

    int pivot = arr[start];

    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }

    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    // Sorting left and right parts of the pivot element
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i] <= pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}

void quickSort(int arr[], int start, int end)
{

    // base case
    if (start >= end)
        return;

    // partitioning the array
    int p = partition(arr, start, end);

    // Sorting the left part
    quickSort(arr, start, p - 1);

    // Sorting the right part
    quickSort(arr, p + 1, end);
}


int main()
{
    int n;
    cin>>n;
    int *mas=new int[n];
    Gen(mas, n);
    Tell_Out(mas, n);
    quickSort(mas,0,n-1);
    Tell_Out(mas, n);
    delete[] mas;
}
