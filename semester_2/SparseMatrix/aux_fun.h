//
// Created by yaros on 5/14/2024.
//

#ifndef SPARSE_AUX_FUN_H
#define SPARSE_AUX_FUN_H
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

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

dense_matrix read_matrix_from_file(const string& f_name) {
    dense_matrix res{};
    ifstream matrix_file;
    matrix_file.open(f_name);
    matrix_file >> res.size;
    res.data=new double*[res.size];
    for (int i=0;i<res.size;i++)
        res.data[i]=new double[res.size];

    for (int i = 0; i < res.size; i++) {
        for (int j = 0; j < res.size; j++) {
            matrix_file >> res.data[i][j];
        }
    }
    return res;
}

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

#endif //SPARSE_AUX_FUN_H
