#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <stack>
#include "gps_post_stream.h"
#include "adjacency_list_graph.h"
#include "sampler.h"
#include "sampler_edge_array.h"
#include <iomanip>

void gps_example(const char* filename, long res_size) {
    tcount::gps_post_stream gps_stream(res_size);

    FILE *file = fopen(filename, "r");

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        //edge_stream.push_back(std::make_tuple(x, y));
        gps_stream.add_edge(x, y);
    }

    std::cout << gps_stream.compute_triangle_count() << std::endl;
}

void forward_example(const char* filename) {
    tcount::adjacency_list_graph g;

    FILE *file = fopen(filename, "r");

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    std::cout << tcount::forward(g) << std::endl;
}

void doulion_example_forward(const char* filename, double p) {
    tcount::adjacency_list_graph g;

    FILE *file = fopen(filename, "r");

    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        if (dist(mt) < p) {
            g[x].push_back(y);
            g[y].push_back(x);
        }
    }

    unsigned long val = tcount::forward(g);
    double e = 1.0 / (p*p*p);
    double t = val * e;

    std::cout << (unsigned long)t << std::endl;
}

long doulion_example_sampling(const char* filename, long samples, double p) {
    tcount::sampler_edge_array sampler;

    FILE *file = fopen(filename, "r");

    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        if (dist(mt) < p) {
            sampler.add_edge(x, y);
        }
    }
    sampler.build_edge_array(true);

    unsigned long val = sampler.sample_triangles(500000);

    double e = 1.0 / (p*p*p);
    double t = val * e;

    std::cout << (unsigned long)t << std::endl;

    return t;
}

void doulion_example_gps(const char* filename, long res_size, double p) {
    tcount::gps_post_stream gps(res_size);

    FILE *file = fopen(filename, "r");

    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        if (dist(mt) < p) {
            gps.add_edge(x, y);
        }
    }

    unsigned long val = static_cast<unsigned long>(gps.compute_triangle_count());
    double e = 1.0 / (p*p*p);
    double t = val * e;

    std::cout << (unsigned long)t << std::endl;
}

void sample_edge_array_exmaple(const char *filename, long samples) {
    tcount::sampler_edge_array sampler;

    FILE *file = fopen(filename, "r");

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        sampler.add_edge(x, y);
    }

    sampler.build_edge_array(true);

    std::cout << sampler.sample_triangles(samples) << std::endl;
}

int main(int argc,char* argv[]) {
    if(argc == 1) {
        std::cout << "No arguments entered." << std::endl;
        return 0;
    }
    /*
     * 1 = Forward
     * 2 = edge sampling
     * 3 = gps
     * 4 = doulion + Forward
     * 5 = doulion + edge
     * 6 = doulion + gps
     */
    else if(argc >= 2) {
        std::string operation(argv[1]);

        if(operation == "1") {
            forward_example(argv[2]);
        }

        else if(operation == "2") {
            long number_of_samples = atol(argv[3]);
            sample_edge_array_exmaple(argv[2], number_of_samples);
        }

        else if(operation == "3") {
            long res_size = atol(argv[3]);
            gps_example(argv[2], res_size);
        }

        else if(operation == "4") {
            double p = atof(argv[3]);
            doulion_example_forward(argv[2], p);
        }

        else if(operation == "5") {
            double p = atof(argv[4]);
            long number_of_samples = atol(argv[3]);
            doulion_example_sampling(argv[2], number_of_samples, p);
        }

        else if(operation == "6") {
            double p = atof(argv[4]);
            long res_size = atol(argv[3]);
            doulion_example_gps(argv[2], res_size, p);
        }
    }

    return 0;
}

