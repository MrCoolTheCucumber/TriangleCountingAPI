//
// Created by Ruben on 13/03/2018.
//

#ifndef TRIANGLECOUNTINGAPI_ADJACENCY_MATRIX_GRAPH_H
#define TRIANGLECOUNTINGAPI_ADJACENCY_MATRIX_GRAPH_H

#include <vector>

namespace tcount {
    class adjacency_matrix_graph {
        std::vector<std::vector<unsigned long>>* g;
        unsigned long size;

    public:
        explicit adjacency_matrix_graph(unsigned long nodes);
        ~adjacency_matrix_graph();
        void add_edge(unsigned long u, unsigned long v);
        void remove_edge(unsigned long u, unsigned long v);
        std::vector<unsigned long> operator [](unsigned long i);
    };
}

namespace tcount {
    //unsigned long count_triangles(adjacency_matrix_graph g);
}

#endif //TRIANGLECOUNTINGAPI_ADJACENCY_MATRIX_GRAPH_H
