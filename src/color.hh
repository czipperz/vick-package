/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_COLORS_H
#define HEADER_GUARD_COLORS_H

#include <ostream>

namespace color {

using color_t = const char*;

constexpr color_t red = "\033[0;31m";
constexpr color_t bold = "\033[0;01m";
constexpr color_t clear = "\033[0m";
}

#endif
