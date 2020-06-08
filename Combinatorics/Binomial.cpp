#include <iostream>
#include <vector>
#include <conio.h>
using namespace std;
int n,k;

void Binom(vector<int> a,int curr_elem)
{
if (curr_elem<=n&&a.size()<k)
	{
	Binom(a,curr_elem+1);
	a.push_back(curr_elem);
	Binom(a,curr_elem+1);
	}
else if (a.size()==k)
	{
     for (int i=0;i<k;i++)
     cout<<a[i]<<" ";
     cout<<endl;
 	 }


}

int main () {
	cout << "Enter n: ";
	cin >> n;
	cout << "Enter k: ";
	cin >> k;
	vector<int> a;
	Binom(a,1);
	return 0;
}
