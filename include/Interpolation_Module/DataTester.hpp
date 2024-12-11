#ifndef INTERPOLATIONTESTER_HPP
#define INTERPOLATIONTESTER_HPP

#include <set>
#include "utilities_interpolation.hpp"

namespace ScientificToolbox::Interpolation{
    class InterpolationTester{
    public:
        InterpolationTester();
        ~InterpolationTester() = default;

        void run_tests();
    
    private:
        bool import_data();
        bool check_data();
        
        std::set<point<double>> points_1;
        std::set<point<double>> points_2;
        std::set<point<double>> points_3;
        std::set<point<double>> points_4;

        std::vector<double> x_values_1;
        std::vector<double> x_values_2;
        std::vector<double> x_values_3;
        std::vector<double> x_values_4;

    };
}


#endif