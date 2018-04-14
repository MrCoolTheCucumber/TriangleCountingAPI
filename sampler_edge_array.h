//
// Created by Ruben on 30/03/2018.
//

#ifndef TRIANGLECOUNTINGAPI_SAMPLER_EDGE_ARRAY_H
#define TRIANGLECOUNTINGAPI_SAMPLER_EDGE_ARRAY_H


#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace tcount {
    class sampler_edge_array {
        std::unordered_map<unsigned long, std::unordered_set<unsigned long>>* g;
        std::vector<unsigned long>* node_array;
        std::vector<unsigned long>* edge_array;

    public:
        sampler_edge_array();
        ~sampler_edge_array();
        void add_edge(unsigned long u, unsigned long v);
        void build_edge_array(bool relabel);
        unsigned long sample_triangles(unsigned long number_of_samples);
    };
}


#endif //TRIANGLECOUNTINGAPI_SAMPLER_EDGE_ARRAY_H
