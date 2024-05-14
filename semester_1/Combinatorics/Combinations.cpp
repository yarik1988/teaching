#include <iostream>
#include <clocale>
#include <string>
#include <windows.h>
using namespace std;
int N,K;
int* comb_set;
void in(int n, int ost)
{
	if (ost > 0 && n <= N)
        {
        comb_set[K-ost] = n;
        in(n + 1, ost - 1);
        in(n + 1, ost);
        }
	if (ost == 0){
		for (int i = 0; i < K; i++)
			cout << comb_set[i] << ' ';
		cout << "\n";
	}
}

int main()
{
    cout<<"Enter number of elements:";
    cin>>N;
    cout<<"enter element count in combination";
    cin>>K;
    comb_set=new int[K];
	in(1,K);
	system("Pause");
}
