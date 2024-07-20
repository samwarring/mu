#ifndef INCLUDED_MU_UNITS_SI_is_prefixES_HPP
#define INCLUDED_MU_UNITS_SI_is_prefixES_HPP
#include <mu/npow.hpp>
#include <mu/units.hpp>

namespace mu {

struct quecto {
  constexpr static const char *name = "quecto";
  constexpr static const char *symbol = "q";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-30>;
};

struct ronto {
  constexpr static const char *name = "ronto";
  constexpr static const char *symbol = "r";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-27>;
};

struct yocto {
  constexpr static const char *name = "yocto";
  constexpr static const char *symbol = "y";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-24>;
};

struct zepto {
  constexpr static const char *name = "zepto";
  constexpr static const char *symbol = "z";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-21>;
};

struct atto {
  constexpr static const char *name = "atto";
  constexpr static const char *symbol = "a";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-18>;
};

struct femto {
  constexpr static const char *name = "femto";
  constexpr static const char *symbol = "f";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-15>;
};

struct pico {
  constexpr static const char *name = "pico";
  constexpr static const char *symbol = "p";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-12>;
};

struct nano {
  constexpr static const char *name = "nano";
  constexpr static const char *symbol = "n";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-9>;
};

struct micro {
  constexpr static const char *name = "micro";
  constexpr static const char *symbol = "µ";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-6>;
};

struct milli {
  constexpr static const char *name = "milli";
  constexpr static const char *symbol = "m";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-3>;
};

struct centi {
  constexpr static const char *name = "centi";
  constexpr static const char *symbol = "c";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-1>;
};

struct deci {
  constexpr static const char *name = "deci";
  constexpr static const char *symbol = "d";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<-1>;
};

struct deka {
  constexpr static const char *name = "deka";
  constexpr static const char *symbol = "da";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<1>;
};

struct hecto {
  constexpr static const char *name = "hecto";
  constexpr static const char *symbol = "h";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<2>;
};

struct kilo {
  constexpr static const char *name = "kilo";
  constexpr static const char *symbol = "k";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<3>;
};

struct mega {
  constexpr static const char *name = "mega";
  constexpr static const char *symbol = "M";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<6>;
};

struct giga {
  constexpr static const char *name = "giga";
  constexpr static const char *symbol = "G";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<9>;
};

struct tera {
  constexpr static const char *name = "tera";
  constexpr static const char *symbol = "T";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<12>;
};

struct peta {
  constexpr static const char *name = "peta";
  constexpr static const char *symbol = "P";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<15>;
};

struct exa {
  constexpr static const char *name = "exa";
  constexpr static const char *symbol = "E";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<18>;
};

struct zetta {
  constexpr static const char *name = "zeta";
  constexpr static const char *symbol = "Z";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<21>;
};

struct yotta {
  constexpr static const char *name = "yotta";
  constexpr static const char *symbol = "Y";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<24>;
};

struct ronna {
  constexpr static const char *name = "ronna";
  constexpr static const char *symbol = "R";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<27>;
};

struct quetta {
  constexpr static const char *name = "quetta";
  constexpr static const char *symbol = "Q";
  constexpr static bool is_prefix = true;
  using units = mu::pow10<30>;
};

} // namespace mu

#endif