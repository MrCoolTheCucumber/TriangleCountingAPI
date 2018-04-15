//
// A class that represents edge sampling that utilizes an adjacency list structure.
//

#include <chrono>
#include <random>
#include <iostream>
#include "sampler.h"

tcount::sampler::sampler() {
    this->g = new std::unordered_map<unsigned long, std::unordered_set<unsigned long>>;
}

tcount::sampler::~sampler() {
    delete g;
}

/**
 * Add an edge e = (u, v) to the undirected graph stored in the sampler.
 *
 * @param u The node u of edge e
 * @param v The node v of the edge e
 */
void tcount::sampler::add_edge(unsigned long u, unsigned long v) {
    (*g)[u].insert(v);
    (*g)[v].insert(u);
}

/**
 * Samples the specified number of triangles and returns an approximation of
 * the number of triangles in the undirected graph stored in the sampler.
 *
 * @param number_of_samples The number of samples to perform.
 * @return An approximation of the triangle count of the graph.
 */
unsigned long tcount::sampler::sample_triangles(unsigned long number_of_samples) {

    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);

    double sum = 0;

    for (int i = 0; i < number_of_samples; ++i) {
        //obtain random node u
        std::uniform_int_distribution<unsigned int> dist_u(0, static_cast<unsigned int>(g->size() - 1));
        auto u_itr = g->begin();
        std::advance(u_itr, dist_u(mt));
        unsigned long sample_u = (*u_itr).first;

        //obtain random node v that is a neighbour of u
        std::uniform_int_distribution<unsigned int> dist_v(0, static_cast<unsigned int>((*u_itr).second.size() - 1));
        auto v_itr = (*u_itr).second.begin();
        std::advance(v_itr, dist_v(mt));
        unsigned long sample_v = (*v_itr);

        auto degree_u = (*u_itr).second.size();
        auto degree_v = (*g)[sample_v].size();

        //calculate |N(u) intersect N(v)|
        unsigned long lambda = 0;
        auto candidate_iterator = sample_u;
        auto candidate_comparator = sample_v;

        if (degree_v < degree_u) {
            candidate_iterator = sample_v;
            candidate_comparator = sample_u;
        }

        for (auto node: (*g)[candidate_iterator]) {
            if ((*g)[candidate_comparator].find(node) != (*g)[candidate_comparator].end()) {
                lambda += 1;
            }
        }

        sum += lambda * ((double) g->size() * (double) degree_u * (double) degree_v) /
               (3.0 * ((double) degree_u + (double) degree_v));
    }

    sum /= number_of_samples;

    return (unsigned long) sum;
}
