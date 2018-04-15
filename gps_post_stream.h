//
//
//

#ifndef TRIANGLECOUNTINGAPI_GPSPOSTSTREAM_H
#define TRIANGLECOUNTINGAPI_GPSPOSTSTREAM_H

#include <random>
#include <unordered_set>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <queue>

namespace tcount {
    class gps_post_stream {
        std::mt19937* mt;

        std::uniform_real_distribution<double>* dist;

        std::unordered_map<unsigned long, std::unordered_set<unsigned long>>* g_res;

        std::unordered_map<
                std::pair<unsigned long, unsigned long>,
                double,
                boost::hash< std::pair<unsigned long, unsigned long> >
        >* weight_table;

        struct min_edge
        {
            bool operator()(const std::tuple<unsigned long, unsigned long, double>& lhs, const std::tuple<unsigned long, unsigned long, double>& rhs) const
            {
                double o, p;
                std::tie(std::ignore, std::ignore, o) = lhs;
                std::tie(std::ignore, std::ignore, p) = rhs;
                return o > p;
            }
        };

        std::priority_queue<
                std::tuple<unsigned long, unsigned long, double>,
                std::vector< std::tuple<unsigned long, unsigned long, double> >,
                min_edge
        >* res;

        double z_star;
        unsigned long res_size;
    public:
        explicit gps_post_stream(unsigned long res_size);
        ~gps_post_stream();
        void add_edge(unsigned long u, unsigned long v);
        unsigned long long compute_triangle_count();
    private:
        double weight(unsigned long u, unsigned long v);
    };
}

#endif //TRIANGLECOUNTINGAPI_GPSPOSTSTREAM_H
