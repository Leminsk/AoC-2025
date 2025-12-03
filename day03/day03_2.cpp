#include <iostream>
#include <fstream>
#include <vector>

uint64_t getLargestJoltage(const std::string& bank) {
    char digits[13] = "000000000000";
    std::vector<int> digits_pos(12, 0);
    int bank_size = bank.size(); // bank.size() from the input.txt is never too large for an int

    // find max digit_00 excluding last 11
    for(int i=0; i<bank_size-11; ++i) {
        if(bank[i] > digits[0]) {
            digits_pos[0] = i;
            digits[0] = bank[i];
        }
    }
    // find all other digits to the right of digit_00
    int ref_pos;
    int curr_digit;
    for(int i=0; i<11; ++i) {
        ref_pos = digits_pos[i];
        curr_digit = i+1;
        for(int p=ref_pos+1; p<bank_size-(10-i); ++p) {
            if(bank[p] > digits[curr_digit]) {
                digits_pos[curr_digit] = p;
                digits[curr_digit] = bank[p];                
            }
        }
    }
    return std::stoull(digits);
}

int main() {
    std::ifstream infile("day03-input.txt");
    std::string line;
    uint64_t joltage_counter = 0;
    while(std::getline(infile, line)) {
        joltage_counter += getLargestJoltage(line);
    }
    std::cout << "joltage_counter:" << joltage_counter << '\n';
    return 0;
}