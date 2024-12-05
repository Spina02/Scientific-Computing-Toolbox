#ifndef INTERPOLATION_MODULE_HPP
#define INTERPOLATION_MODULE_HPP

#include <vector>
#include <utility> // For std::pair
#include <stdexcept> // For exceptions
#include <algorithm> // For std::lower_bound

// point class
class point {
public:
    point(double x, double y) : x(x), y(y) {}
    ~point() = default;

    double get_x() const { return x; }
    double get_y() const { return y; }

private:
    double x;
    double y;
};

// abline class
class abline {
public:
    abline(const point &p1, const point &p2) {
        slope = (p2.get_y() - p1.get_y()) / (p2.get_x() - p1.get_x());
        intercept = p1.get_y() - slope * p1.get_x();
    }

    ~abline() = default;

    std::pair<double, double> get_abline() const {
        return std::make_pair(slope, intercept);
    }

    double evaluate(double x) const {
        return slope * x + intercept;
    }

private:
    double slope;
    double intercept;
};

// interval class
class interval {
public:
    interval(double lower_bound, double upper_bound) 
        : lower_bound(lower_bound), upper_bound(upper_bound) {}

    ~interval() = default;

    double get_lower_bound() const { return lower_bound; }
    double get_upper_bound() const { return upper_bound; }
    std::pair<double, double> get_interval() const { return {lower_bound, upper_bound}; }
    double get_difference() const { return upper_bound - lower_bound; }

    bool contains(double x) const {
        return x >= lower_bound && x <= upper_bound;
    }

private:
    double lower_bound;
    double upper_bound;
};

// Abstract base class
class Interpolation {
public:
    Interpolation(const std::vector<point>& data) : data(data) {
        if (data.empty()) {
            throw std::invalid_argument("Data points cannot be empty.");
        }
    }

    virtual ~Interpolation() = default;

    virtual double interpolate(double x) const = 0;
    virtual double operator()(double x) const { return interpolate(x); }

protected:
    const std::vector<point>& getData() const { return data; }

private:
    std::vector<point> data;
};

// Derived class for linear interpolation
class LinearInterpolation : public Interpolation {
public:
    LinearInterpolation(const std::vector<point>& data);

    double interpolate(double x) const override;

private:
    std::vector<std::pair<interval, abline>> interpolated_functions;
};

#endif
