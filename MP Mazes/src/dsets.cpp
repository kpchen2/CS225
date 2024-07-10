/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        vec.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if (vec.at(elem) < 0) {
        return elem;
    }
    int rootIdx = find(vec.at(elem));
    vec.at(elem) = rootIdx;
    return rootIdx;
}

void DisjointSets::setunion(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);

    if (rootA != rootB) {
        if (vec.at(rootA) <= vec.at(rootB)) {
            int temp = vec.at(rootB);
            vec.at(rootB) = rootA;
            vec.at(rootA) += temp;
        } else {
            int temp = vec.at(rootA);
            vec.at(rootA) = rootB;
            vec.at(rootB) += temp;
        }
    }
}

int DisjointSets::size(int elem) {
    std::queue<int> queue;
    int index = find(elem);
    return -1*vec[index];
}

int DisjointSets::num() {
    int ans = 0;
    for (unsigned i = 0; i < vec.size(); i++) {
        if (vec[i] < 0) {
            ans++;
        }
    }
    return ans;
}

std::vector<int> DisjointSets::getVec() {
    return vec;
}