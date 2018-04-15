//
// A class for reperesenting an adjacency matrix.
//

#include "adjacency_matrix_graph.h"


tcount::adjacency_matrix_graph::adjacency_matrix_graph(unsigned long nodes) {
    this->size = nodes;
    this->g = new std::vector<std::vector<unsigned long>>;
    for (int i = 0; i < nodes; ++i) {
        std::vector<unsigned long> inner;
        inner.resize(nodes);
        g->push_back(inner);
    }
}

tcount::adjacency_matrix_graph::~adjacency_matrix_graph() {
    delete g;
}

void tcount::adjacency_matrix_graph::add_edge(unsigned long u, unsigned long v) {
    (*g)[u][v] = 1;
}

void tcount::adjacency_matrix_graph::remove_edge(unsigned long u, unsigned long v) {
    (*g)[u][v] = 0;
}

std::vector<unsigned long> tcount::adjacency_matrix_graph::operator[](unsigned long i) {
    return (*g)[i];
}

