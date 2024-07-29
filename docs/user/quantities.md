# Working with Quantities  {#userguide_quantities}

## What's in a Quantity?

The mu::quantity class template represents a measured value. It is templated on
two type parameters.

1. `Rep` is the type of value held in the quantity. We call this the quantity
   **representation**, which can be any fundamental C++ type, so long as that
   type represents an integral or floating point value. User-defined types can
   also serve as representation types if they satisfy the mu::rep concept.

2. `Units` is a type that signifies the **units** of the quantity. Mu provides a
   collection of pre-defined unit types. As we will see, units can be composed
   together to form new units. Users can define their own types to represent
   their own unit system, so long as the types satisfy the mu::units concept.

mu::quantity is simply a wrapper around a `Rep` instance, and is strongly-typed by its `Units`. The units are never instantiated by the quantity.

For example:

```cpp
#include <cstdint>
#include <mu/mu.hpp>

mu::quantity<int, mu::meter> i;          // An int, measured in meters.
mu::quantity<float, mu::hour> f;         // A float, measured in hours.
mu::quantity<double, mu::kilogram> d;    // A double, measured in kilograms.
mu::quantity<long double, mu::joule> ld; // A long double, measured in joules.
mu::quantity<std::int8_t, mu::watt> i8;  // An 8-bit integer, measured in watts.

// Each quantity only contains an instance of its representation.
static_assert(sizeof(i) == sizeof(int));
static_assert(sizeof(f) == sizeof(float));
static_assert(sizeof(d) == sizeof(double));
static_assert(sizeof(ld) == sizeof(long double));
static_assert(sizeof(i8) == sizeof(std::int8_t));
```

## Constructing Quantities

### Default Constructor
```cpp
mu::quantity<int, mu::kilometer> dist;
```
The _default constructor_ performs default construction of its representation.

In the above example `dist` has an indeterminate value, as is the
case for a default-constructed `int`.

### Initial Value Constructor
```cpp
mu::quantity<double, mu::joule> energy{12.34};
```
A quantity can be constructed with an initial value.

This constructor is marked _explicit_ to prevent implicit construction from raw values.

### Copy and Move Constructors
```cpp
mu::quantity<int, mu::kilometer> dist1{5};
mu::quantity<int, mu::kilometer> dist2 = dist1;
mu::quantity<int, mu::kilometer> dist3 = std::move(dist1);
```
Quantities implement copy and move constructors which source their value from
another quantity.

When the source and destination quantities have **equivalent** units, then the
copy and move constructors perform copy or move construction of the raw value.
The move-constructor may seem redundant for built-in representations like `int`
or `double`, but can greatly improve performance if the representation is an
expensive user-defined representation, such as a MxN matrix.

When the source and destination quanities units are not equivalent, but are
**convertible**, then the raw value of the source value is converted to a raw
value measured in the units of the destination.

## Converting Quantities

Mu allows construction and assignment between quantities of different units and
automatically scales the raw value by a scale factor computed at compile time.
However, for the source quantity to be assignable to the destination quantity,
the two must satisfy the mu::quantity_losslessly_convertible_to concept. This
concept requires the following properties:

1. **The units must have the same dimensions.** For example, all units of speed
   have the same dimensions, even if they are measured in different units:
   `meters/second` has the same dimensions as `miles/hour`. This is formalized
   by the mu::units_convertible_to concept.

2. **A conversion cannot result in loss of precision.** This property considers
   the scale value together with the representations of the source and
   destination quantities. If no scaling is required (i.e. the source and
   destination units are **equivalent**), then the source quantity's `Rep` must
   be perfectly representable by the destination quantity's `Rep`. If scaling
   _is_ required, then the _scaled value_ (which may be integral or floating
   point) must be perfectly representable by the destination quantity's `Rep`.

### Valid Examples

The following examples are well-formed conversions between quantities.

```cpp
// Example 1
mu::quantity<int, mu::kilometer> a = 5;  // 5 km
mu::quantity<int, mu::meter> b = a;      // 5'000 m
```

Meters and kilometers share the same dimensions (length). To convert kilometers
into meters, we need to multiply the source value (in kilometers) by `1000`. The
converted value is an `int`, which is perfectly representable by `b`'s
representation (also an `int`).

```cpp
// Example 2
mu::quantity<int, mu::meter> a = 23;        // 23 m
mu::quantity<double, mu::kilometer> b = a;  // 0.023 km
```

Meters and kilometers share the same dimensions (length). To convert meters to
kilometers, we need to multiply the source value by `0.001`. The converted value
is a `double`, which is perfectly representable by `b`'s representation (also a `double`).

```cpp
// Example 3
mu::quantity<int, mu::meter> a = 23;       // 23 m
mu::quantity<float, mu::kilometer> b = a;  // 0.023 km
```

Meters and kilometers share the same dimensions (length). To convert meters to
kilometers, we need to multiply the source value by `0.001`. The converted value
is a `float`, which is perfectly representable by `b`'s representation (also a `float`).

\note An astute reader may spot an apparent contradiction between example 2 and
example 3. In example 2 the converted value is a `double`, but in example 3 the
converted value is a `float`. In fact, both are true! Whenever a value must be
scaled by a floating point value, the scale is initially computed as a
`long double` to minimize precision loss. Then, if the destination value is also
a floating point type, the scale is **relaxed** to match the destination type.
This allows floating-point conversions to be as precise as possible without
forcing the user to store the results of such conversions as `long double`.

```cpp
// Example 4
mu::quantity<std::int8_t, mu::meter> a = 42;  // 42 m
mu::quanitty<std::int16_t, mu::meter> b = a;  // 42 m
```

Meters and kilometers share the same dimensions (length). The units are
equivalent, so no scaling is required. The representation of `a` (`int8_t`) is
perfectly representable by the representation of `b` (`int16_t`).

```cpp
// Example 5
mu::quantity<int, mu::meter> a = 123;   // 123 m
mu::quantity<double, mu::meter> b = a;  // 123.0 m
```

Meters and kilometers share the same dimensions (length). The units are
equivalent, so no scaling is required. According to the C++ standard, any
conversion from an integral type to a floating point type is considered a
"narrowing conversion", but Mu goes further and checks if the significand
(a.k.a. mantissa) of the floating point type can completely hold the bits of the
integral type. If so, then it performs a `static_cast` knowing there is no risk
of truncation. On _most_ machines, a 32-bit `int` can completely fit in the
53-bit significand of a `double`, so this conversion is valid.