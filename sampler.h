//
//
//

#ifndef TRIANGLECOUNTINGAPI_SAMPLER_H
#define TRIANGLECOUNTINGAPI_SAMPLER_H


#include <unordered_map>
#include <unordered_set>

namespace tcount {
    class sampler {
        std::unordered_map<unsigned long, std::unordered_set<unsigned long>>* g;

    public:
        sampler();
        ~sampler();
        void add_edge(unsigned long u, unsigned long v);
        unsigned long sample_triangles(unsigned long number_of_samples);
    };
}



#endif //TRIANGLECOUNTINGAPI_SAMPLER_H
