#ifndef INCLUDED_MYMATH_HPP
#define INCLUDED_MYMATH_HPP

#include <complex>
#include <tuple>

namespace mymath {

std::tuple<std::complex<double>, std::complex<double>>
quadratic(double a, double b, double c);

}

#endif