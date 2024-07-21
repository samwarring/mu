# Mu

**Mu** is C++ header-only library for working with physical units.

This library lets you define **quantity** objects that are strongly typed by
their phsical units. Quantity objects can be added, subtracted, negated,
multiplied, or divided while adhering to the rules of dimensional analysis.

Quantity objects contain a **value** that stores the magnitude of the quantity.
The value can be any fundamental C++ type, and the library provides
customization points allowing clients to integrate their own types.

Quantity types are templated on a **units** type. The template parameters
determine the dimensions and scale of the quantity. It is a _compile-time error_
to assign quantities of different dimensions, and if the dimensions are the
same, the library determines the correct _scale factor_ at compile-time. Users
can define their own units, which can be fundamental or composed from existing
units.

For the complete explanation of features, visit the [User Guide](#userguide).

Example:
```cpp
#include <mu/mu.hpp>
#include <sstream>

int main() {
    // Calculate speed in km/h.
    mu::quantity<int, mu::kilometer> distance{100};
    mu::quantity<int, mu::hour> time{5};
    auto speed_kmph = distance / time;

    // Converts km/h to m/s.
    mu::quantity<double, mu::meter_per_second> speed_mps = speed_kmph;

    // Print both quantities.
    std::ostringstream sout;
    sout << mu::stream::symbols
         << mu::stream::superscript_exponents
         << mu::stream::mult_sep(" • ")
         << speed_kmph << " = "
         << speed_mps;

    // Check expected string.
    std::string actual = sout.str();
    std::string expected = "20 km • hr⁻¹ = 5.55556 m • s⁻¹";
    return actual != expected;
}
```

## Prior Art

This library is a hobby project (and I learned a lot while making it)! I
currently do not recommend using this library in production, because there
already exist other mature libraries for the same purpose. You might consider
using:

  1. [mpusz/mp-units](https://github.com/mpusz/mp-units)
  2. [aurora-opensource/Au](https://github.com/aurora-opensource/au)