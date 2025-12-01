#include <iostream>
#include <fstream>

int calculateRotation(const int& current_position, const char& direction, const std::string& amount, int& passed_0) {
    int new_position;
    int zero_counter = 0;
    int int_amount = std::stoi(amount);
    int d = int_amount%100; // multiples of 100 are a full rotation
    zero_counter += int_amount/100; // have to count those full rotations
    
    if(direction == 'R') {
        new_position = current_position + d;
        if(new_position >= 100) { 
            ++zero_counter;
            new_position -= 100; 
        }
    } else {
        new_position = current_position - d;
        if(new_position <= 0) {
            if(current_position != 0) { ++zero_counter; }            
            if(new_position < 0) { new_position += 100; }
        }
    }
    passed_0 += zero_counter;

    return new_position;
}

int main() {
    std::ifstream infile("day01-input.txt");
    std::string line;
    std::string num_str;
    int current_pos = 50;
    int zero_counter = 0;
    while(std::getline(infile, line)) {
        num_str = line.substr(1, line.size()-1);
        current_pos = calculateRotation(current_pos, line[0], num_str, zero_counter);
    }
    std::cout << "zero_counter:" << zero_counter << '\n';
    return 0;
}