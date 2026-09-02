#include <iostream>
#include <conio.h>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <tgmath.h>
using namespace std;

const int MATRIX_SIZE = 5;
const int ZERO_PERCENT = 70;

using namespace std;

struct dense_matrix
{
	double** data;
	int size;
};

struct sparse_matrix
{
	int* data[3];
	int n_cols;
};

sparse_matrix convert_to_sparse(dense_matrix in)
{
	int num_nonzero = 0;
	for (int i=0;i<in.size;i++)
		for (int j=0;j<in.size;j++)
			if (in.data[i][j]!=0) num_nonzero++;
	sparse_matrix res{};
	res.n_cols=num_nonzero;
	for (auto & i : res.data)
		i=new int[num_nonzero];

	int counter=0;
	for (int i=0;i<in.size;i++)
		for (int j=0;j<in.size;j++)
			if (in.data[i][j]!=0) {
				res.data[0][counter]=i;
				res.data[1][counter]=j;
				res.data[2][counter]=round(in.data[i][j]);
				counter++;
			}
	return res;
}

int random_nonzero()
{
	int value;
	do
	{
		value=rand()%19-9;
	}
	while (value==0);
	return value;
}

dense_matrix generate_sparse_matrix(int size, int zero_percent)
{
	dense_matrix res{};
	res.size=size;
	res.data=new double*[size];
	for (int i=0;i<size;i++)
		res.data[i]=new double[size];

	for (int i=0;i<size;i++)
	{
		int forced=rand()%size;
		for (int j=0;j<size;j++)
		{
			if (j!=forced && rand()%100<zero_percent)
				res.data[i][j]=0;
			else
				res.data[i][j]=random_nonzero();
		}
	}

	// make sure no column is completely zero
	for (int j=0;j<size;j++)
	{
		bool has_nonzero=false;
		for (int i=0;i<size;i++)
			if (res.data[i][j]!=0) has_nonzero=true;
		if (!has_nonzero)
			res.data[rand()%size][j]=random_nonzero();
	}
	return res;
}

void print_matrix(dense_matrix m)
{
	for (int i=0;i<m.size;i++)
	{
		for (int j=0;j<m.size;j++)
			cout <<" "<< m.data[i][j];
		cout <<"\n";
	}
}

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
    srand((unsigned)(time(nullptr)^chrono::steady_clock::now().time_since_epoch().count()));
    dense_matrix init=generate_sparse_matrix(MATRIX_SIZE, ZERO_PERCENT);
    cout << "Generated matrix:\n";
    print_matrix(init);
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
