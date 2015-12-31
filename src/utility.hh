#ifndef HEADER_GUARD_SPLIT_H
#define HEADER_GUARD_SPLIT_H

#include <string>

template <class Itr, class Out, class Pred>
void split(Itr begin, Itr end, Out falselist, Out truelist, Pred pred)
{
    if (begin == end)
        return;
    for (; begin != end; ++begin)
        (pred(*begin) ? truelist : falselist).push_back(*begin);
}

bool exists_plugin(const std::string& name);

#endif
