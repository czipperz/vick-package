/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "remove_packages.hh"
#include <boost/filesystem.hpp>
#include <cstdio>

void remove_packages(int num_packages, const char* const* packages) {
    using namespace boost::filesystem;
    using namespace std;
    const path cur = current_path() / "plugins";
    auto n = num_packages;
    auto p = packages;
    auto shouldthrow = false;
    for (; n; --n, ++p) {
        auto path = cur / *p;
        if (not exists(path)) {
            printf("Package you specified to remove (%s) does not "
                   "exist.\n",
                   *p);
            shouldthrow = true;
        }
    }
    if (shouldthrow) {
        puts("Some packages don't exist so won't remove any "
             "specified.");
        exit(EXIT_FAILURE);
    }
    for (; num_packages; --num_packages, ++packages) {
        path p = cur / *packages;
        if (not exists(p)) {
            printf("Directory you specified to remove (%s) does not "
                   "exist.\nIt did when we safety checked!\n",
                   *packages);
            exit(EXIT_FAILURE);
        }
        remove_all(p);
    }
}
