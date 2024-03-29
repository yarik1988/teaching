#include <conio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
typename T::size_type levenshtein_distance(const T & src, const T & dst) {
  const typename T::size_type m = src.size();
  const typename T::size_type n = dst.size();
  if (m == 0) {
    return n;
  }
  if (n == 0) {
    return m;
  }

  std::vector< std::vector<typename T::size_type> > matrix(m + 1);

  for (typename T::size_type i = 0; i <= m; ++i) {
    matrix[i].resize(n + 1);
    matrix[i][0] = i;
  }
  for (typename T::size_type i = 0; i <= n; ++i) {
    matrix[0][i] = i;
  }

  typename T::size_type above_cell, left_cell, diagonal_cell, cost;

  for (typename T::size_type i = 1; i <= m; ++i) {
    for(typename T::size_type j = 1; j <= n; ++j) {
      cost = src[i - 1] == dst[j - 1] ? 0 : 1;
      above_cell = matrix[i - 1][j];
      left_cell = matrix[i][j - 1];
      diagonal_cell = matrix[i - 1][j - 1];
      matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
    }
  }

  return matrix[m][n];
}

int main()
{
	cout<<"Enter two strings!";
    std::string s1;
    std::string s2;
    cin>>s1;
    cin>>s2;
    cout << levenshtein_distance(s1, s2);
    getch();
    return 0;
}
