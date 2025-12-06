#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::ifstream infile("day06-input.txt");
    std::string line;
    
    std::vector<std::string> lines = {};
    std::string operations = "";
    
    while(std::getline(infile, line)) {
        if(line[0] != ' ' && line[0] < '0') { // last line: operations
            operations = line;
        } else { // operands
            lines.push_back(line);
        }
    }

    int lines_amount = lines.size();
    int line_size = lines[0].size();
    int columns_amount = operations.size();
    uint64_t sum = 0;

    int start_index = 0;
    int end_index = -1;
    int v_amount;
    
    std::string v_number;
    std::vector<uint64_t> v_numbers;
    uint64_t v_res;

    for(int c=0; c<line_size; ++c) {
        bool white_space_column = true;
        for(int l=0; l<lines_amount; ++l) {
            white_space_column &= (lines[l][c] == ' ');
        }

        if(white_space_column) {
            start_index = end_index + 1;
            end_index = c;
            v_numbers = {};
            for(int i=start_index; i<end_index; ++i) {
                v_number = "";
                for(int j=0; j<lines_amount; ++j) {
                    if(lines[j][i] != ' ') { v_number += lines[j][i]; }
                }
                v_numbers.push_back(std::stoull(v_number));
            }
            
            v_amount = v_numbers.size();
            if(operations[start_index] == '+') {
                v_res = 0;
                for(int i=0; i<v_amount; ++i) { v_res += v_numbers[i]; }
            } else {
                v_res = 1;
                for(int i=0; i<v_amount; ++i) { v_res *= v_numbers[i]; }
            }

            sum += v_res;
        }
    }

    // last number column has no right-white-space-column
    start_index = end_index+1;
    end_index = line_size;
    v_numbers = {};
    for(int i=start_index; i<end_index; ++i) {
        v_number = "";
        for(int j=0; j<lines_amount; ++j) {
            if(lines[j][i] != ' ') { v_number += lines[j][i]; }
        }
        v_numbers.push_back(std::stoull(v_number));
    }

    v_amount = v_numbers.size();
    if(operations[start_index] == '+') {
        v_res = 0;
        for(int i=0; i<v_amount; ++i) { v_res += v_numbers[i]; }
    } else {
        v_res = 1;
        for(int i=0; i<v_amount; ++i) { v_res *= v_numbers[i]; }
    }

    sum += v_res;

    std::cout << "sum:" << sum << '\n';
    return 0;
}