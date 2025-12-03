#include <iostream>
#include <fstream>

int getLargestJoltage(const std::string& bank) {
    char left_digit = '0';
    char right_digit = '0';
    int bank_size = bank.size(); // bank.size() from the input.txt is never too large for an int

    int left_digit_pos = 0;
    // find max left_digit excluding last position
    for(int i=0; i<bank_size-1; ++i) {
        if(bank[i] > left_digit) {
            left_digit_pos = i;
            left_digit = bank[i];
        }
    }
    // find max right_digit to the right of left_digit
    for(int i=left_digit_pos+1; i<bank_size; ++i) {
        if(bank[i] > right_digit) {
            right_digit = bank[i];
        }
    }

    char s_joltage[3] = { left_digit, right_digit };
    return std::stoi(s_joltage);
}

int main() {
    std::ifstream infile("day03-input.txt");
    std::string line;
    int joltage_counter = 0;
    while(std::getline(infile, line)) {
        joltage_counter += getLargestJoltage(line);
    }
    std::cout << "joltage_counter:" << joltage_counter << '\n';
    return 0;
}