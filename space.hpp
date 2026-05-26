#include "entities.hpp"

class pair {
    public:
    int idx;
    int idy;
    pair(int x, int y) : idx(x), idy(y) {}
};

void space_partition(std::vector<circle> &shapes);
