#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <stack>
#include "gps_post_stream.h"
#include "adjacency_list_graph.h"
#include "sampler.h"
#include "sampler_edge_array.h"
#include <boost/timer.hpp>
#include <iomanip>

void gps_example() {
    tcount::gps_post_stream gps_stream(250000);

    FILE *file = fopen(R"(C:\graphs\real\com-orkut.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

    //unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    //std::vector<std::tuple<unsigned long, unsigned long>> edge_stream;

    boost::timer t1;

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        //edge_stream.push_back(std::make_tuple(x, y));
        gps_stream.add_edge(x, y);
    }

    /*std::shuffle(edge_stream.begin(), edge_stream.end(), std::mt19937(seed));

    for(auto edge: edge_stream) {
        gps_stream.add_edge(std::get<0>(edge), std::get<1>(edge));
    }*/


    std::cout << gps_stream.compute_triangle_count() << std::endl;
    std::cout << t1.elapsed() << std::endl;
}

void forward_example() {
    tcount::adjacency_list_graph g;

    FILE *file = fopen(R"(C:\graphs\real\com-orkut.ungraph.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        //edge_stream.push_back(std::make_tuple(x, y));
        g[x].push_back(y);
        g[y].push_back(x);
    }

    std::cout << "imported graph." << std::endl;

    boost::timer t;
    std::cout << tcount::forward(g) << " triangles." << std::endl;
    std::cout << t.elapsed() << " seconds." << std::endl;
}

void doulion_example_forward() {
    tcount::adjacency_list_graph g;
    double p = 0.1;

    FILE *file = fopen(R"(C:\graphs\real\com-orkut.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

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

    boost::timer timer;
    unsigned long val = tcount::forward(g);
    auto time = timer.elapsed();
    double e = 1.0 / (p*p*p);
    double t = val * e;

    std::cout << (unsigned long)t << std::endl;
    std::cout << "time: " << time <<std::endl;
}

long doulion_example_sampling() {
    tcount::sampler_edge_array sampler;
    double p = 0.1;

    FILE *file = fopen(R"(C:\graphs\real\com-orkut.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

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
    boost::timer timer;
    unsigned long val = sampler.sample_triangles(500000);
    //std::cout << val <<std::endl;
    auto time = timer.elapsed();
    double e = 1.0 / (p*p*p);
    double t = val * e;

    std::cout << (unsigned long)t << std::endl;
    std::cout << "time: " << time <<std::endl;

    return t;
}

void doulion_example_gps(unsigned long res_size) {
    tcount::gps_post_stream gps(res_size);
    double p = 0.1;

    FILE *file = fopen(R"(C:\graphs\real\com-orkut.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        if (dist(mt) < p) {
            gps.add_edge(x, y);
        }
    }

    boost::timer timer;
    unsigned long val = static_cast<unsigned long>(gps.compute_triangle_count());
    //std::cout << val <<std::endl;
    auto time = timer.elapsed();
    double e = 1.0 / (p*p*p);
    double t = val * e;

    std::cout << (unsigned long)t << std::endl;
    std::cout << "time: " << time <<std::endl;
}

void sample_edge_array_exmaple() {
    tcount::sampler_edge_array sampler;

    FILE *file = fopen(R"(C:\graphs\real\soc-pokec.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

    //unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    //std::vector<std::tuple<unsigned long, unsigned long>> edge_stream;

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        //edge_stream.push_back(std::make_tuple(x, y));
        sampler.add_edge(x, y);
    }

    sampler.build_edge_array(false);
    std::cout << "Graph Imported." << std::endl;

    boost::timer t1;
    std::cout << sampler.sample_triangles(500) << std::endl;
    std::cout << "1 " << t1.elapsed() << std::endl;

    boost::timer t2;
    std::cout << sampler.sample_triangles(1000) << std::endl;
    std::cout << "2 " << t2.elapsed() << std::endl;

    boost::timer t3;
    std::cout << sampler.sample_triangles(10000) << std::endl;
    std::cout << "3 " << t3.elapsed() << std::endl;

    boost::timer t4;
    std::cout << sampler.sample_triangles(50000) << std::endl;
    std::cout << "4 " << t4.elapsed() << std::endl;

    boost::timer t5;
    std::cout << sampler.sample_triangles(100000) << std::endl;
    std::cout << "5 " << t5.elapsed() << std::endl;

    boost::timer t6;
    std::cout << sampler.sample_triangles(200000) << std::endl;
    std::cout << "6 " << t6.elapsed() << std::endl;

    boost::timer t7;
    std::cout << sampler.sample_triangles(500000) << std::endl;
    std::cout << "7 " << t7.elapsed() << std::endl;

    boost::timer t8;
    std::cout << sampler.sample_triangles(1000000) << std::endl;
    std::cout << "8 " << t8.elapsed() << std::endl;
}

void sample_example() {
    tcount::sampler sampler;

    FILE *file = fopen(R"(C:\graphs\real\com-orkut.txt)", "r");
    unsigned long n, m;
    fscanf(file, "%lu %lu\n", &n, &m);

    //unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    //std::vector<std::tuple<unsigned long, unsigned long>> edge_stream;

    unsigned long x, y;
    while (fscanf(file, "%lu %lu\n", &x, &y) != EOF) {
        //edge_stream.push_back(std::make_tuple(x, y));
        sampler.add_edge(x, y);
    }

    std::cout << "Graph Imported." << std::endl;

    /*std::shuffle(edge_stream.begin(), edge_stream.end(), std::mt19937(seed));

    for(auto edge: edge_stream) {
        gps_stream.add_edge(std::get<0>(edge), std::get<1>(edge));
    }*/

    boost::timer t1;
    std::cout << sampler.sample_triangles(500) << std::endl;
    std::cout << "1 " << t1.elapsed() << std::endl;

    /*boost::timer t2;
    std::cout << sampler.sample_triangles(1000) << std::endl;
    std::cout << "2 " << t2.elapsed() << std::endl;

    boost::timer t3;
    std::cout << sampler.sample_triangles(10000) << std::endl;
    std::cout << "3 " << t3.elapsed() << std::endl;

    boost::timer t4;
    std::cout << sampler.sample_triangles(50000) << std::endl;
    std::cout << "4 " << t4.elapsed() << std::endl;

    boost::timer t5;
    std::cout << sampler.sample_triangles(100000) << std::endl;
    std::cout << "5 " << t5.elapsed() << std::endl;

    boost::timer t6;
    std::cout << sampler.sample_triangles(200000) << std::endl;
    std::cout << "6 " << t6.elapsed() << std::endl;

    boost::timer t7;
    std::cout << sampler.sample_triangles(500000) << std::endl;
    std::cout << "7 " << t7.elapsed() << std::endl;

    boost::timer t8;
    std::cout << sampler.sample_triangles(1000000) << std::endl;
    std::cout << "8 " << t8.elapsed() << std::endl;*/
}

int main() {
    doulion_example_gps(50000);
    return 0;
}

