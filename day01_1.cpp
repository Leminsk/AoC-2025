#include <iostream>
#include <fstream>

int calculateRotation(const int& current_position, const char& direction, const std::string& amount) {
    int new_position;
    int d = std::stoi(amount)%100; // multiples of 100 are a full rotation
    if(direction == 'R') {
        new_position = current_position + d;
        if(new_position >= 100) { new_position -= 100; }
    } else {
        new_position = current_position - d;
        if(new_position < 0) { new_position += 100; }
    }
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
        current_pos = calculateRotation(current_pos, line[0], num_str);
        if(current_pos == 0) { ++zero_counter; }
    }
    std::cout << "zero_counter:" << zero_counter << '\n';
    return 0;
}