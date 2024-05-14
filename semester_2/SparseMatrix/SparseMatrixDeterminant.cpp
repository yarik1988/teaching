#include <iostream>
#include <conio.h>
#include <vector>
#include <set>
#include <algorithm>
#define NNONZERO 7
using namespace std;

int compactMatrix[3][NNONZERO] =
    {
        {1 , 2 , 3 , 3 ,4, 4,  5},
        {1 , 2 , 3 , 4 ,3, 4,  5},
        {2 , 2 , 2 , 2 ,-3, 2, 2},
    };

bool is_even(vector<int> used)
{
	set<int> trash;
	int count_even=0;
	for (int i=0;i<used.size();i++)
		if (trash.count(i)==0)
		{
			trash.insert(i);
			int cur=i;
			int length=0;
			do
			{
				cur=used[cur]-1;
				trash.insert(cur);
				length++;
			}
			while (cur!=i);
			if (length%2==0) count_even++;
		}
 return !(count_even%2);
}

int gen_permut(int index, vector<int> used)
{
    if (index==NNONZERO) return is_even(used)*2-1;
    int cur_row=compactMatrix[0][index];
    int next_index=index;
    while (next_index<NNONZERO&&compactMatrix[0][next_index]==cur_row) next_index++;
    if (next_index<NNONZERO&&compactMatrix[0][next_index]!=cur_row+1) return 0;
    int det=0;
    for (int i=index;i<next_index;i++)
    {
       if (count(used.begin(), used.end(), compactMatrix[1][i])==0)
       {
            vector<int> new_used=used;
            new_used.push_back(compactMatrix[1][i]);
            det+=gen_permut(next_index,new_used)*compactMatrix[2][i];
       }

    }
return det;
}

int main()
{

    for (int i=0; i<3; i++)
    {
        for (int j=0; j<NNONZERO; j++)
            cout <<" "<< compactMatrix[i][j];

        cout <<"\n";
    }
    vector<int> used;
    cout<<gen_permut(0,used);
	_getch();

    return 0;
}
