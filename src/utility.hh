/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_SPLIT_H
#define HEADER_GUARD_SPLIT_H

#include <string>

template <class Itr, class Out, class Pred>
void split(Itr begin, Itr end, Out falselist, Out truelist,
           Pred pred) {
    if (begin == end)
        return;
    for (; begin != end; ++begin)
        (pred(*begin) ? truelist : falselist).push_back(*begin);
}

bool exists_plugin(const std::string& name);

#endif
