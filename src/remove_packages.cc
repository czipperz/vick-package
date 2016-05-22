/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "remove_packages.hh"
#include <boost/filesystem.hpp>
#include <iostream>

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
            cout << "Package you specified to remove (" << *p
                 << ") does not exist." << endl;
            shouldthrow = true;
        }
    }
    if (shouldthrow) {
        cout << "Some packages don't exist so won't remove any specified." << endl;
        exit(EXIT_FAILURE);
    }
    for (; num_packages; --num_packages, ++packages) {
        path p = cur / *packages;
        if (not exists(p)) {
            cout << "Directory you specified to remove (" << *packages
                 << ") does not exist." << endl
                 << "It did when we safety checked!" << endl;
            exit(EXIT_FAILURE);
        }
        /* ensure not unistd.h's remove */
        boost::filesystem::remove_all(p);
    }
}
