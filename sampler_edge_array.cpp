//
//
//

#include "sampler_edge_array.h"

#include <chrono>
#include <algorithm>
#include <random>
#include <iostream>

tcount::sampler_edge_array::sampler_edge_array() {
    this->g = new std::unordered_map<unsigned long, std::unordered_set<unsigned long>>;
    this->node_array = nullptr;
    this->edge_array = nullptr;
}

tcount::sampler_edge_array::~sampler_edge_array() {
    delete (g, node_array, edge_array);
}

/**
 * Add an edge e = (u, v) to the undirected graph stored in the sampler.
 *
 * @param u The node u of edge e
 * @param v The node v of the edge e
 */
void tcount::sampler_edge_array::add_edge(unsigned long u, unsigned long v) {
    (*g)[u].insert(v);
    (*g)[v].insert(u);
}

/**
 * Builds the edge array based on the edges added to the undirected graph structure.
 *
 * @param relabel If set to true, edges will be re-labeled such that they are consecutive. This
 * should be set to true if nodes added are not consecutively labeled or you are not sure if they are.
 */
void tcount::sampler_edge_array::build_edge_array(bool relabel) {
    //sort map keys, then build off that
    //or just assume all is okay and work off using the map size

    this->node_array = new std::vector<unsigned long>(g->size() + 1);
    this->edge_array = new std::vector<unsigned long>;


    if(relabel) {
        //re-label nodes such that labels are consecutive.
        //g_node_label -> edge_array_node_label
        std::unordered_map<unsigned long, unsigned long> label;
        //edge_array_label -> g_node_label
        std::unordered_map<unsigned long, unsigned long> reverse_label;

        unsigned long current_label = 0;
        for(const auto &pair: (*g)) {
            label[pair.first] = current_label;
            reverse_label[current_label] = pair.first;
            current_label += 1;
        }

        unsigned long current_index = 0;
        for(unsigned long i = 0; i < g->size(); ++i) {
            (*node_array)[i] = current_index;

            for(unsigned long node: (*g)[reverse_label[i]]) {
                (*edge_array).push_back(label[node]);
                current_index += 1;
            }
        }

    }
    else {
        unsigned long current_index = 0;
        for(unsigned long i = 0; i < g->size(); ++i) {
            (*node_array)[i] = current_index;

            for(unsigned long node: (*g)[i]) {
                (*edge_array).push_back(node);
                current_index += 1;
            }
        }
    }




    delete(g);
    this->g = nullptr;
}

/**
 * Samples the specified number of triangles and returns an approximation of
 * the number of triangles in the edge array stored in the sampler.
 *
 * The edge array must be build first before this function is called.
 *
 * @param number_of_samples The number of samples to perform.
 * @return An approximation of the triangle count of the graph.
 */
unsigned long tcount::sampler_edge_array::sample_triangles(unsigned long number_of_samples) {
    std::vector<unsigned long> node_array_ = *node_array;
    std::vector<unsigned long> edge_array_ = *edge_array;

    (*node_array)[node_array->size() - 1] = edge_array->size();
    auto seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);

    std::vector<int64_t> sorted(node_array->size() - 1);

    auto M = static_cast<unsigned long>((edge_array->size()) / 2);
    double sum = 0;

    for (int i = 0; i < number_of_samples; ++i) {
        //std::cout << "0" << std::endl;
        //obtain random node u
        std::uniform_int_distribution<unsigned int> dist_u(0, static_cast<unsigned int>(edge_array->size() - 1));
        auto u_itr = edge_array->begin();
        std::advance(u_itr, dist_u(mt));
        unsigned long sample_u = (*u_itr);

        //std::cout << "1" << std::endl;

        unsigned long degree_u;
        unsigned long degree_v;

        //obtain random node v that is a neighbour of u
        degree_u = (*node_array)[sample_u + 1] - (*node_array)[sample_u];
        std::uniform_int_distribution<unsigned int> dist_v(0, degree_u - 1);
        auto v_itr = edge_array->begin();
        std::advance(v_itr, (*node_array)[sample_u]);
        std::advance(v_itr, dist_v(mt));
        unsigned long sample_v = (*v_itr);
        degree_v = (*node_array)[sample_v + 1] - (*node_array)[sample_v];

        //std::cout << "2" << std::endl;

        //calculate |N(u) intersect N(v)|
        unsigned long lambda = 0;

        //sort each neighbour list
        if(!sorted[sample_u]) {
            auto u_begin_itr = edge_array->begin();
            std::advance(u_begin_itr, (*node_array)[sample_u]);

            auto u_end_itr = edge_array->begin();
            std::advance(u_end_itr, (*node_array)[sample_u] + degree_u);

            std::sort(u_begin_itr, u_end_itr);
            sorted[sample_u] = true;
        }

        //std::cout << "3" << std::endl;

        if(!sorted[sample_v]) {
            auto v_begin_itr = edge_array->begin();
            std::advance(v_begin_itr, (*node_array)[sample_v]);

            auto v_end_itr = edge_array->begin();
            std::advance(v_end_itr, (*node_array)[sample_v] + degree_v);

            std::sort(v_begin_itr, v_end_itr);
            sorted[sample_v] = true;
        }

        //std::cout << "4" << std::endl;

        auto candidate = degree_u < degree_v ? sample_u : sample_v;
        auto other = candidate == sample_u ? sample_v : sample_u;

        for(auto n = (*node_array)[candidate], m = (*node_array)[other]; n < (*node_array)[candidate + 1] && m < (*node_array)[other + 1]; /* */)
        {
            if((*edge_array)[n] == (*edge_array)[m]) {
                lambda++;
                n++;
                m++;
            }
            else if((*edge_array)[n] > (*edge_array)[m]) {
                m++;
            }
            else if((*edge_array)[n] < (*edge_array)[m]) {
                n++;
            }
        }

        //std::cout << "5" << std::endl;
        sum += lambda * (M/3.0);
    }

    sum /= number_of_samples;

    return (unsigned long) sum;
}
