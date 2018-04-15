//
// A class for representing an adjacency list as well as providing triangle ccounting algorithms
// that can be performed on the adjacency list graphs.
//

#include <algorithm>
#include <unordered_set>
#include <iostream>
#include "adjacency_list_graph.h"


tcount::adjacency_list_graph::adjacency_list_graph() {
    this->g = new std::unordered_map<unsigned long, std::vector<unsigned long>>;
}

tcount::adjacency_list_graph::~adjacency_list_graph() {
    delete g;
}

std::vector<unsigned long> & tcount::adjacency_list_graph::operator[](unsigned long i) {
    return (*g)[i];
}

std::unordered_map<unsigned long, std::vector<unsigned long>>::iterator tcount::adjacency_list_graph::begin() {
    return this->g->begin();
}

std::unordered_map<unsigned long, std::vector<unsigned long>>::iterator tcount::adjacency_list_graph::end() {
    return this->g->end();
}

unsigned long long int tcount::adjacency_list_graph::number_of_nodes() {
    return this->g->size();
}

unsigned long tcount::node_iterator(tcount::adjacency_list_graph &g) {
    for(auto pair: g) {
        auto vec = pair.second;
        std::sort(vec.begin(), vec.end());
    }

    unsigned long t = 0;

    for(auto pair: g) {
        unsigned long node = pair.first;

        for(auto n1: g[node]) {
            for(auto n2: g[node]){
                if(std::binary_search(g[n1].begin(), g[n1].end(), n2)) {
                    t += 1;
                }
            }
        }
    }

    t = t / 6;
    return t;
}

bool node_order_sort(std::pair<unsigned int, unsigned int> vec1, std::pair<unsigned int, unsigned int> vec2) {
    return vec1.second < vec2.second;
}

unsigned long tcount::forward(tcount::adjacency_list_graph &g) {
    // ( node_id, d(node_id) )
    std::vector<std::pair<unsigned int, unsigned int>> sorted_node_order(g.number_of_nodes());
    // node_id -> eta (index of element in sorted_node_order)
    std::unordered_map<unsigned int, unsigned int> eta;

    unsigned long i = 0;
    for(auto pair: g) {
        sorted_node_order[i].first = pair.first;
        sorted_node_order[i].second = static_cast<unsigned int>(pair.second.size());
        i += 1;
    }

    std::sort(sorted_node_order.begin(), sorted_node_order.end(), node_order_sort);

    unsigned int eta_label = 0;
    for (auto node: sorted_node_order) {
        eta[node.first] = eta_label++;
    }

    std::unordered_map<unsigned int, std::unordered_set<unsigned int>> a(g.number_of_nodes());
    unsigned int T = 0;

    for (auto ordered_tuple: sorted_node_order) {
        unsigned int s = ordered_tuple.first;
        for (auto t: g[s]) {
            if (eta[s] < eta[t]) {
                // s is lower in degree
                for (auto v : a[s]) {
                    if (a[t].find(v) != a[t].end()) {
                        T += 1;
                    }
                }
                a[t].insert(s);
            }
        }
    }

    return T;
}
