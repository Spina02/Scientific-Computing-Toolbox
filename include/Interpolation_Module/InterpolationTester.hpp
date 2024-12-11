#ifndef INTERPOLATIONTESTER_HPP
#define INTERPOLATIONTESTER_HPP

#include <set>
#include "utilities_interpolation.hpp"

namespace ScientificToolbox::Interpolation{
    class InterpolationTester{
    public:
        InterpolationTester();
        ~InterpolationTester() = default;

        bool run_tests();
    
    private:
        bool import_data();
        bool check_data();
        bool test_linear_interpolation();
        bool test_lagrange_interpolation();
        bool test_newton_interpolation();
        bool test_cubic_spline_interpolation();

        std::set<point<double>> points_1;
        std::set<point<double>> points_2;
        std::set<point<double>> points_3;
        std::set<point<double>> points_4;

        std::vector<double> x_values_1;
        std::vector<double> x_values_2;
        std::vector<double> x_values_3;
        std::vector<double> x_values_4;

        const double epsilon = 1e-6;
        const double x_query = 10.54;

        const std::vector<double> results_1{-11.0137, -17.0559, -17.0559, -12.4965};
        const std::vector<double> results_2{81.9502, 776.798, 776.798, 82.6408};
        const std::vector<double> results_3{-19.2184, 249702, 249702, -229.151};
        const std::vector<double> results_4{-17.9081, 30559.2, 30559.2, -457.361};

    };
}


#endif