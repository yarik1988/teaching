#include <iostream>
#include <conio.h>
#include <vector>
#include <set>
#include <algorithm>
#include "aux_fun.h"
using namespace std;

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
				cur=used[cur];
				trash.insert(cur);
				length++;
			}
			while (cur!=i);
			if (length%2==0) count_even++;
		}
 return !(count_even%2);
}

int calc_det_sparse(int index, vector<int> used, sparse_matrix& compactMatrix)
{
    if (index==compactMatrix.n_cols) return is_even(used)*2-1;
    int cur_row=compactMatrix.data[0][index];
    int next_index=index;
    while (next_index<compactMatrix.n_cols&&compactMatrix.data[0][next_index]==cur_row) next_index++;
    if (next_index<compactMatrix.n_cols&&compactMatrix.data[0][next_index]!=cur_row+1) return 0;
    int det=0;
    for (int i=index;i<next_index;i++)
    {
       if (count(used.begin(), used.end(), compactMatrix.data[1][i])==0)
       {
            vector<int> new_used=used;
            new_used.push_back(compactMatrix.data[1][i]);
            det+= calc_det_sparse(next_index, new_used, compactMatrix) * compactMatrix.data[2][i];
       }

    }
return det;
}

double determinant(dense_matrix m) {
    double det = 1.0;
    for (int i = 0; i < m.size; i++) {
        int pivot = i;
        for (int j = i + 1; j < m.size; j++) {
            if (abs(m.data[j][i]) > abs(m.data[pivot][i])) {
                pivot = j;
            }
        }
        if (pivot != i) {
            swap(m.data[i], m.data[pivot]);
            det *= -1;
        }
        if (m.data[i][i] == 0) {
            return 0;
        }
        det *= m.data[i][i];
        for (int j = i + 1; j < m.size; j++) {
            double factor = m.data[j][i] / m.data[i][i];
            for (int k = i + 1; k < m.size; k++) {
                m.data[j][k] -= factor * m.data[i][k];
            }
        }
    }
    return det;
}

int main()
{
    dense_matrix init=read_matrix_from_file("../matrix5x5.txt");
    sparse_matrix compactMatrix=convert_to_sparse(init);
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<compactMatrix.n_cols; j++)
            cout <<" "<< compactMatrix.data[i][j];

        cout <<"\n";
    }
    vector<int> used;
    cout << calc_det_sparse(0, used, compactMatrix)<<endl;
    cout << determinant(init)<<endl;
    return 0;
}
