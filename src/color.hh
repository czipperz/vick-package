#ifndef HEADER_GUARD_COLORS_H
#define HEADER_GUARD_COLORS_H

#include <ostream>

namespace color {

using color_t = const char*;

constexpr color_t red   = "\033[0;31m";
constexpr color_t bold  = "\033[0;01m";
constexpr color_t clear = "\033[0m";

#define RED(t) ::color::red << t << ::color::clear
#define BOLD(t) ::color::bold << t << ::color::clear

}

#endif
