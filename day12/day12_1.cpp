#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::ifstream infile("day12-input.txt");
    std::string line;

    bool reading_index = true;
    bool reading_shape = false;
    bool reading_area = false;
    int curr_index;
    std::vector<std::vector<bool>> curr_shape;

    std::vector< std::vector<std::vector<bool>> > shapes = {};
    std::vector< std::pair<int,int> > regions = {};
    std::vector< std::vector<int> > quantities = {};

    while(std::getline(infile, line)) {
        
        if(reading_index) {
            std::string shape_index = "";
            for(int i=0; i<line.size(); ++i) {
                if(line[i] == ':') { break; }
                if(line[i] == 'x') {
                    reading_area = true;
                    break;
                }
                shape_index += line[i];
            }
            curr_index = std::stoi(shape_index);
            reading_index = false;
            reading_shape = true;
            curr_shape = {};

        } else if(!reading_area && reading_shape) {
            if(line == "") {
                shapes.push_back(curr_shape);
                reading_shape = false;
                reading_index = true;                
            } else {
                std::vector<bool> bool_line = {};
                for(const char& c : line) { bool_line.push_back(c == '#'); }
                curr_shape.push_back(bool_line);
            }
        }

        if(reading_area) {
            int i=0;
            std::string width = "";
            for(; i<line.size(); ++i) {
                if(line[i] == 'x') { ++i; break; }
                width += line[i];
            }
            std::string height = "";
            for(; i<line.size(); ++i) {
                if(line[i] == ':') { i+=2; break; }
                height += line[i];
            }
            regions.push_back( {std::stoi(width), std::stoi(height)} );

            std::string curr_num = "";
            std::vector<int> quantity = {};
            for(; i<line.size(); ++i) {
                if(line[i] == ' ') {
                    quantity.push_back( std::stoi(curr_num) );
                    curr_num = "";
                    continue;
                }
                curr_num += line[i];
            }
            quantity.push_back(std::stoi(curr_num));
            quantities.push_back(quantity);
        }
    }

    std::vector<int> tiles_per_present = {};
    for(auto& s : shapes) {
        int tiles = 0;
        for(auto& l : s) {
            for(const auto& tile : l) {
                if(tile) { ++tiles; }
            }
        }
        tiles_per_present.push_back(tiles);
    }

    int can_fit = 0;

    for(int r=0; r<regions.size(); ++r) {
        if(regions[r].first < 3 || regions[r].second < 3) {
            bool skip = false;
            for(auto& q : quantities[r]) {
                if(q > 0) { skip = true; break; }
            }
            if(skip) { continue; }
        }
        int area = regions[r].first * regions[r].second;
        int area_asked = 0;
        for(int q=0; q<quantities[r].size(); ++q) {
            area_asked += tiles_per_present[q] * quantities[r][q];
        }
        // this is absolutely disgusting and hilarious
        if(area > area_asked) {
            ++can_fit;
        }
    }    


    std::cout << "can_fit: " << can_fit << '\n';

    return 0;
}