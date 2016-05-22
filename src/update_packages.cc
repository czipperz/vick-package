/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include <iostream>

#include "color.hh"

void update_packages(int num_packages, char** packages)
{
    using namespace boost::filesystem;
    using namespace std;
    const path cur = current_path() / "plugins";
    const auto fun = [](path p) {
        cout << BOLD(p.filename().c_str() << ": ") << flush;
        chdir(p.c_str());

        int r = system("git pull");
        if (r)
            throw r;
    };
    if (num_packages) {
        auto n = num_packages;
        auto p = packages;
        auto shouldthrow = false;
        for (; n; --n, ++p) {
            auto path = cur / *p;
            if (not exists(path)) {
                cerr << RED("Error:")
                     << " Package you specified to update (" << *p
                     << ") does not exist." << endl;
                shouldthrow = true;
            }
        }
        if (shouldthrow) {
            cerr << RED("Error:") << " Some packages don't exist so "
                                     "won't update any specified."
                 << endl;
            exit(EXIT_FAILURE);
        }
        for (; num_packages; --num_packages, ++packages) {
            path p = cur / *packages;
            if (not exists(p)) {
                cerr << RED("Error:")
                     << " Directory you specified to update ("
                     << *packages << ") does not exist." << endl
                     << "It did when we safety checked!" << endl;
                exit(EXIT_FAILURE);
            }
            fun(p);
        }
    } else {
        for_each(directory_iterator(cur), directory_iterator(), fun);
    }
}
