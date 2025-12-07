#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void pushUnique(std::vector<int>& v, int n) {
    std::vector<int>::iterator it;
    it = std::find(v.begin(), v.end(), n);
    if(it == v.end()) {
        v.push_back(n);
    }
}

int main() {
    std::ifstream infile("day07-input.txt");
    std::string line;

    int source_index;
    std::vector<std::string> lines = {};
    
    while(std::getline(infile, line)) {
        bool skip_line = false;
        for(int i=0; i<line.size(); ++i) {
            if(line[i] == 'S') { 
                source_index = i;
                skip_line = true;
                break;
            }
        }
        if(!skip_line) {
            lines.push_back(line);
        }
    }

    std::vector<int> beam_indices = { source_index };
    int last_line = lines.size()-1;
    int line_size = lines[0].size();
    int split_counter = 0;

    for(int l=0; l<=last_line; ++l) {
        std::vector<int> next_beams = {};
        for(const int& b : beam_indices) {
            if(lines[l][b] == '^') {
                ++split_counter;
                lines[l][b-1] = '|';
                lines[l][b+1] = '|';
                pushUnique(next_beams, b-1);
                pushUnique(next_beams, b+1);
            } else {
                lines[l][b] = '|';
                pushUnique(next_beams, b);
            }
        }
        beam_indices = next_beams;
    }

    std::cout << "splits:" << split_counter << '\n';
    

    return 0;
}