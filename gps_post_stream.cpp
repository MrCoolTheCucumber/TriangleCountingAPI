//
// Created by Ruben on 11/03/2018.
//

#include "gps_post_stream.h"
#include <chrono>
#include <cfloat>
#include <iostream>

tcount::gps_post_stream::gps_post_stream(unsigned long res_size) {
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    mt = new std::mt19937(seed);
    dist = new std::uniform_real_distribution<double>(std::nextafter(0.0, DBL_MAX), std::nextafter(1.0, DBL_MAX));
    g_res = new std::unordered_map<unsigned long, std::unordered_set<unsigned long>>;
    weight_table = new std::unordered_map<
            std::pair<unsigned long, unsigned long>,
            double,
            boost::hash<std::pair<unsigned long, unsigned long> >
    >;
    res = new std::priority_queue<
            std::tuple<unsigned long, unsigned long, double>,
            std::vector<std::tuple<unsigned long, unsigned long, double> >,
            min_edge
    >;
    this->res_size = res_size;
    this->z_star = 0.0;
}

tcount::gps_post_stream::~gps_post_stream() {
    delete (mt, dist, g_res, weight_table, res);
}

void tcount::gps_post_stream::add_edge(unsigned long u, unsigned long v) {
    (*g_res)[u].insert(v);
    (*g_res)[v].insert(u);

    double u_k = (*dist)((*mt));

    double w_k = weight(u, v);
    (*weight_table)[std::make_pair(u, v)] = w_k;
    (*weight_table)[std::make_pair(v, u)] = w_k;

    double r_k = w_k / u_k;
    auto new_k = std::make_tuple(u, v, r_k);

    res->push(new_k);
    if (res->size() > this->res_size) {
        auto k_star = res->top();
        this->z_star = std::max(this->z_star, std::get<2>(k_star));
        res->pop();

        //remove edge from g_res since its out of the min heap
        unsigned long a, b;
        std::tie(a, b, std::ignore) = k_star;

        (*g_res)[a].erase(b);
        (*g_res)[b].erase(a);

        (*weight_table).erase(std::make_pair(a, b));
        (*weight_table).erase(std::make_pair(b, a));
    }
}

unsigned long long tcount::gps_post_stream::compute_triangle_count() {
    unsigned long long N_t = 0;
    std::cout << "z_star: " << this->z_star << std::endl;

    while (!res->empty()) {
        auto edge = res->top();
        unsigned long v1, v2;
        std::tie(v1, v2, std::ignore) = edge;

        double q  = std::min(1.0, (*weight_table)[std::make_pair(v1, v2)] / this->z_star);

        auto canditate = v1;
        auto other = v2;
        if((*g_res)[v2].size() < (*g_res)[v1].size()) {
            canditate = v2;
            other = v1;
        }

        //iterate over neighbours of v1.
        for(auto v3: (*g_res)[canditate]) {
            auto k1 = std::make_pair(canditate, v3);
            double q1 = std::min(1.0, (*weight_table)[k1] / this->z_star);

            if((*g_res)[other].find(v3) != (*g_res)[other].end()) {
                auto k2 = std::make_pair(other, v3);
                double q2 = std::min(1.0, (*weight_table)[k2] / this->z_star);

                double long N_k = 1.0 / (q * q1 * q2);

                N_t += N_k;
            }
        }

        res->pop();
    }

    N_t = N_t / 3;

    return N_t;
}

double tcount::gps_post_stream::weight(unsigned long u, unsigned long v) {
    auto candidate_set = (*g_res)[u];
    auto candidate_other = (*g_res)[v];
    if ((*g_res)[v].size() < (*g_res)[u].size()) {
        candidate_set = (*g_res)[v];
        candidate_other = (*g_res)[u];
    }
    unsigned long completed_triangles = 0;
    for (auto node: candidate_set) {
        if (candidate_other.find(node) != candidate_other.end()) {
            completed_triangles += 1;
        }
    }

    return (9.0 * (double) completed_triangles) + 1.0;
}
