#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <set>

class Tile {
public:
    int64_t id, x, y;

    Tile(int64_t id, int64_t x, int64_t y) {
        this->id = id;
        this->x = x;
        this->y = y;
    }

    friend bool operator==(const Tile& a, const Tile& b) {
        return a.id == b.id;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Tile& b) {
        stream << '[' << b.id << "]_{" << b.x << ',' << b.y << '}';
        return stream;
    }
};

struct SetHelper {
    Tile* tile;
    int64_t distance;
};

bool cmp(const SetHelper& a, const SetHelper& b) {
    return a.distance < b.distance;
}

int64_t distance2(const Tile& a, const Tile& b) {
    int64_t x = b.x - a.x;
    int64_t y = b.y - a.y;
    return (x*x) + (y*y);
}

int64_t rectArea(const Tile& a, const Tile& b) {
    int64_t x = (a.x > b.x) ? a.x - b.x : b.x - a.x;
    int64_t y = (a.y > b.y) ? a.y - b.y : b.y - a.y;
    return (x+1) * (y+1);
}

int main() {
    std::ifstream infile("day09-input.txt");
    std::string line;

    std::regex re("\\d+");
    std::smatch match;
    std::string::const_iterator begin;
    std::string::const_iterator end;
    
    std::vector<Tile*> tiles = {};

    int64_t max_width = 0;
    int id_counter = 0;
    while(std::getline(infile, line)) {
        std::vector<int> new_line = {};
        begin = line.cbegin();
        end = line.cend();
        while(std::regex_search(begin, end, match, re)) {
            new_line.push_back(std::stoi(match[0]));
            begin = match.suffix().first;
        }
        Tile* tile = new Tile(id_counter, new_line[0], new_line[1]);
        tiles.push_back(tile);
        if(new_line[0] > max_width) { max_width = new_line[0]; }
        ++id_counter;
    }

    // order all tiles from their distance to {0,0}
    Tile bottom_left(-1, 0, 0);
    std::set<SetHelper, decltype(&cmp)> dist_bottom_left(&cmp);
    // order all tiles from their distance to {max_width,0}
    Tile bottom_right(-2, max_width, 0);
    std::set<SetHelper, decltype(&cmp)> dist_bottom_right(&cmp);
    
    for(Tile*& t : tiles) {
        int64_t d_left = distance2(*t, bottom_left);
        int64_t d_right = distance2(*t, bottom_right);
        dist_bottom_left.insert( {t, d_left} );
        dist_bottom_right.insert( {t, d_right} );
    }
    
    int64_t largest_area_bottom_left = rectArea(*(dist_bottom_left.begin()->tile), *(dist_bottom_left.rbegin()->tile));
    int64_t largest_area_bottom_right = rectArea(*(dist_bottom_right.begin()->tile), *(dist_bottom_right.rbegin()->tile));
    int64_t largest_area = (largest_area_bottom_left > largest_area_bottom_right) ? largest_area_bottom_left : largest_area_bottom_right;

    std::cout << "largest area: " << largest_area << '\n';

    return 0;
}