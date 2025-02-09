#include <iostream>
#include <vector>
using namespace std;

vector<int> compute_pi_digits(int digits) {
    int len = (10 * digits) / 3 + 1;
    vector<int> a(len, 2);
    vector<int> result;
    int predigit = 0;
    int nines = 0;

    for (int i = 0; i < digits; ++i) {
        int carry = 0;

        // Multiply each digit by 10
        for (int j = 0; j < len; ++j)
            a[j] *= 10;

        // Calculate next digit from right to left
        for (int j = len-1; j >= 1; --j) {
            int denom = 2*j + 1;
            int temp = a[j] + carry;
            a[j] = temp % denom;
            carry = (temp / denom) * j;
        }

        // Process the first element
        int temp = a[0] + carry;
        a[0] = temp % 10;
        int next_digit = temp / 10;

        // Handle digit 10 cases
        if (next_digit == 9) {
            nines++;
        } else if (next_digit == 10) {
            result.push_back(predigit + 1);
            for (int k = 0; k < nines; ++k)
                result.push_back(0);
            predigit = nines = 0;
        } else {
            if (predigit != 0 || !result.empty()) {
                result.push_back(predigit);
                for (int k = 0; k < nines; ++k)
                    result.push_back(9);
            }
            predigit = next_digit;
            nines = 0;
        }

        if (result.size() >= digits) break;
    }

    // Add remaining digits
    if (result.size() < digits) {
        result.push_back(predigit);
        for (int k = 0; k < nines && result.size() < digits; ++k)
            result.push_back(9);
    }

    result.resize(digits);
    return result;
}

int main() {
    int num_digits;
    cout << "Enter number of digits to calculate: ";
    cin >> num_digits;

    if (num_digits < 1) {
        cerr << "Invalid input. Please enter a positive integer." << endl;
        return 1;
    }

    // Compute digits (including the leading 3)
    vector<int> pi_digits = compute_pi_digits(num_digits + 1);

    cout << "3.";
    for (int i = 1; i < pi_digits.size(); ++i) {
        cout << pi_digits[i];
    }
    cout << endl;

    return 0;
}