#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	const int N=500;
	int arr[N][N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			arr[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++) {
		arr[i][i] = 1;
		arr[0][i] = 1;
		arr[i][0] = 1;
	}
	std::vector<int> pattern;
	int i_old=0;
	int j_old=0;
	for (int i = 1; i < N; i++) {
		bool flag = true;
		for (int j = 1; j <N; j++) {
			if (arr[i][j] == 0 && flag) {
				arr[i][j] = 2;
				if (i < j) {
					std::cout << i + 1 << " " << j + 1 << "\n";
					pattern.push_back(i+1-i_old-1);
					i_old=i+1;
					j_old=j+1;
				}
				
				flag = false;
				for (int k = i+1; k < N; k++) {
					arr[k][j] = 1;
				}
				for (int k = 1; k < N- std::max(i, j); k++) {
					arr[i + k][j + k] = 1;
				}
			}
			else {
				arr[i][j] = 1;
			}
		}
	}

    for (int i=0;i<pattern.size();i++)
        std::cout<<pattern[i];
    return 0;
}

