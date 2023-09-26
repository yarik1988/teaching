//There is a queen in the lower left corner of the M×N board. Two players take turns moving the queen, moving it any number of squares vertically up, horizontally to the right, or diagonally to the right and up. The winner is the one who places the queen in the upper right corner of the board. Determine which player has the winning strategy.
#include <iostream>
#include <algorithm>
#include <vector>

std::vector<std::pair<int,int>> get_pos_from_pattern(std::vector<int> pattern)
{
    std::vector<std::pair<int,int>> res;
    int i=0;
    int j=0;
    for (auto it=pattern.begin();it!=pattern.end();it++)
    {
        if (*it==0)
        {
            i+=1;
            j+=2;
        } else
        {
            i+=2;
            j+=3;
        }
        res.emplace_back(i,j);
    }
    return res;
}

std::vector<int> gen_pattern(int n) {
    std::vector<int> cur_pattern;
    cur_pattern.push_back(1);
    std::vector<int> next_pattern;
    while (true) {
        next_pattern.clear();
        for (auto it = cur_pattern.begin(); it != cur_pattern.end(); it++) {
            if (*it == 0)
                next_pattern.insert(next_pattern.end(), {1, 0});
            else
                next_pattern.insert(next_pattern.end(), {1, 1, 0});
        }
        cur_pattern=next_pattern;
        auto vv=get_pos_from_pattern(next_pattern);
        if (vv.back().second>n) break;
    }

return cur_pattern;
}


int main()
{
	const int N=100;
    //brute_force approach, using dynamic programming. We create matrix with expected winner
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
	for (int i = 1; i < N; i++) {
		bool flag = true;
		for (int j = 1; j <N; j++) {
			if (arr[i][j] == 0 && flag) {
				arr[i][j] = 2;
				if (i < j) {
					std::cout << i + 1 << " " << j + 1 << "\n";
					pattern.push_back(i-i_old);
					i_old=i+1;
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
    /*
    for (int i=0;i<N;i++) {
        for (int j = 0; j < N; j++)
            std::cout<<arr[i][j]<<" ";
           std::cout<<std::endl;
    }
     */
    //pattern is binary sequence which can be used to reconstruct position of 2's
    auto gogi=get_pos_from_pattern(pattern);
    for (int i=0;i<pattern.size();i++)
        std::cout<<pattern[i];
    std::cout<<std::endl;
    for (auto it:gogi)
        std::cout<<it.first<<" "<<it.second<<std::endl;


    //But!!! pattern can be built in more elegant way
    auto pattern1=gen_pattern(N);
    for (int i=0;i<pattern1.size();i++)
        std::cout<<pattern1[i];
    return 0;
}

