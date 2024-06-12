#include <mymath.hpp>

namespace mymath {

std::tuple<std::complex<double>, std::complex<double>>
quadratic(double a, double b, double c) {
  std::complex<double> det = (b * b) - (4 * a * c);
  auto s1 = std::sqrt(det);
  auto s2 = -s1;
  auto r1 = (-b + s1) / (2 * a);
  auto r2 = (-b + s2) / (2 * a);
  return {r1, r2};
}

} // namespace mymath