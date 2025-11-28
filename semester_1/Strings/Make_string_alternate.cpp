/*This function calculates the number of operations required to make a string
 * "alternating" (e.g., 010101 or ababab) where no two adjacent characters are
 * the same.*/

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int solve(string &s) {
  int i = 0, count = 0;
  while (true) {
    while (i < s.size() - 1 && s[i] != s[i + 1])
      ++i;
    if (i == s.size() - 1)
      return count;
    int j = i + 2;
    while (j < s.size() - 1 && (s[j] != s[j + 1] || s[i] == s[j]))
      ++j;
    reverse(s.begin() + i + 1, s.begin() + j + 1);
    count++;
  }
}

int main() {
  string s;
  cin >> s;
  cout << solve(s) << "\n";
  return 0;
}