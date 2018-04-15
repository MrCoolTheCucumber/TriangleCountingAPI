//
// Created by Ruben on 13/03/2018.
//

#ifndef TRIANGLECOUNTINGAPI_ADJACENCY_LIST_GRAPH_H
#define TRIANGLECOUNTINGAPI_ADJACENCY_LIST_GRAPH_H

#include <vector>
#include <unordered_map>

namespace tcount {
    class adjacency_list_graph {
        std::unordered_map<unsigned long, std::vector<unsigned long>>* g;
    public:
        adjacency_list_graph();
        ~adjacency_list_graph();
        std::vector<unsigned long> & operator [](unsigned long i);
        std::unordered_map<unsigned long, std::vector<unsigned long>>::iterator begin();
        std::unordered_map<unsigned long, std::vector<unsigned long>>::iterator end();
        unsigned long long int number_of_nodes();
    };
}

namespace tcount {
    unsigned long forward(adjacency_list_graph &g);
}

#endif //TRIANGLECOUNTINGAPI_ADJACENCY_LIST_GRAPH_H
