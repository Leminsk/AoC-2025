#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <set>
#include <unordered_map>

class JunctionBox {
public:
    int64_t id, x, y, z;

    JunctionBox(int64_t id, int64_t x, int64_t y, int64_t z) {
        this->id = id;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    friend bool operator==(const JunctionBox& a, const JunctionBox& b) {
        return (a.id == b.id);
    }
    friend std::ostream& operator<<(std::ostream& stream, const JunctionBox& b) {
        stream << '[' << b.id << "]_{" << b.x << ',' << b.y << ',' << b.z << '}';
        return stream;
    }
};

struct PairToHash {
    std::size_t operator()(const std::pair<JunctionBox*,JunctionBox*>& p) const {
        return std::hash<int>()(p.first->id) ^ (std::hash<int>()(p.second->id) << 1);
    }
};

struct SetHelper {
    std::pair<JunctionBox*,JunctionBox*> jb_pair;
    int64_t distance;
};

bool cmp(const SetHelper& a, const SetHelper& b) {
    return a.distance < b.distance;
}


int64_t distance2(const JunctionBox& a, const JunctionBox& b) {
    int64_t x = b.x - a.x;
    int64_t y = b.y - a.y;
    int64_t z = b.z - a.z;
    return (x*x) + (y*y) + (z*z);
}

int main() {
    std::ifstream infile("day08-input.txt");
    std::string line;

    std::regex re("\\d+");
    std::smatch match;
    std::string::const_iterator begin;
    std::string::const_iterator end;
    
    std::vector< std::vector<JunctionBox*> > circuits = {};

    int id = 0;
    while(std::getline(infile, line)) {
        std::vector<int> new_line = {};
        begin = line.cbegin();
        end = line.cend();
        while(std::regex_search(begin, end, match, re)) {
            new_line.push_back(std::stoi(match[0]));
            begin = match.suffix().first;
        }
        JunctionBox* junction_box = new JunctionBox(id, new_line[0], new_line[1], new_line[2]);
        circuits.push_back({ junction_box });
        ++id;
    }

    // pre-calculate all those darn distances and order them
    std::unordered_map<std::pair<JunctionBox*,JunctionBox*>, bool, PairToHash> checked_pairs;
    std::set<SetHelper, decltype(&cmp)> all_distances(&cmp);
    int distances_amount = 0;
    for(int i=0; i<id-1; ++i) {
        for(int j=i+1; j<id; ++j) {
            JunctionBox* jbi = circuits[i][0];
            JunctionBox* jbj = circuits[j][0];
            int64_t d = distance2(*jbi, *jbj);
            all_distances.insert( {std::make_pair(jbi, jbj), d} );
            ++distances_amount;
        }
    }

    // empty the circuits
    for(int i=0; i<id; ++i) { circuits[i] = {}; }

    // build circuits from the all_distances set using it as an ordered list of edges

    int64_t x1, x2;

    // for every edge
    for(auto it : all_distances) {        
        JunctionBox* jb1 = it.jb_pair.first;
        JunctionBox* jb2 = it.jb_pair.second;

        int circuit_index = -1;
        int jb1_circuit_index = -1;
        int jb2_circuit_index = -1;

        bool skip_and_decrement = false;
        bool at_least_one = false;

        // check if one of its vertices is in a circuit
        for(int i=0; i<id; ++i) {
            bool got1 = false;
            bool got2 = false;
            for(int j=0; j<circuits[i].size(); ++j) {
                if(circuits[i][j] == jb1) { got1 = true; jb1_circuit_index = i; }
                if(circuits[i][j] == jb2) { got2 = true; jb2_circuit_index = i; }
            }
            if(got1 && got2) { skip_and_decrement = true; break; }
        }

        if(skip_and_decrement) { continue; }

        // if they are on different circuits, join based on the largest one
        if(jb1_circuit_index != -1 && jb2_circuit_index != -1) {
            if(circuits[jb1_circuit_index].size() > circuits[jb2_circuit_index].size()) {
                for(auto& jb : circuits[jb2_circuit_index]) { circuits[jb1_circuit_index].push_back(jb); }
                circuits[jb2_circuit_index] = {};
            } else {
                for(auto& jb : circuits[jb1_circuit_index]) { circuits[jb2_circuit_index].push_back(jb); }
                circuits[jb1_circuit_index] = {};
            }
        }
        // otherwise join on the one which is in a circuit
        else if(jb1_circuit_index == -1 && jb2_circuit_index != -1) { 
            circuits[jb2_circuit_index].push_back(jb1);
        }
        else if(jb2_circuit_index == -1 && jb1_circuit_index != -1) { 
            circuits[jb1_circuit_index].push_back(jb2);
        }
        // else, add both to the next empty circuit
        else {
            for(int i=0; i<id; ++i) {
                if(circuits[i].size() == 0) {
                    circuits[i].push_back(jb1);
                    circuits[i].push_back(jb2);
                    break;
                }
            }
        }

        bool all_connected = false;
        // check if are all connected
        for(int i=0; i<id; ++i) {
            if(circuits[i].size() == id) { all_connected = true; break; }            
        }
        if(all_connected) { 
            x1 = jb1->x;
            x2 = jb2->x;
            break; 
        }
    }
    
    std::cout << "X1 multiplies X2: " << x1 * x2 << '\n';

    return 0;
}