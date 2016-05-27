/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include <iostream>

#include "color.hh"

using namespace boost::filesystem;
using namespace std;

static bool HAS_ERROR = false;

static void update_directory(path p) {
    cout << BOLD(p.filename().c_str() << ": ") << flush;
    auto cmd = "git -C " + p.string() + " pull";
    if (system(cmd.c_str()))
        HAS_ERROR = true;
}

void update_packages(int num, char** packages) {
    const path cur = current_path() / "plugins";
    if (num == 0) {
        for_each(directory_iterator(cur), directory_iterator(),
                 update_directory);
    } else {
        auto shouldthrow = false;
        std::for_each(packages, packages + num, [&cur, &shouldthrow](
                                                    char* p) {
            auto path = cur / p;
            if (not exists(path)) {
                cerr << RED("Error:") << " Package you specified to "
                                         "update ("
                     << *p << ") does not exist." << endl;
                shouldthrow = true;
            }
        });
        if (shouldthrow) {
            cerr << RED("Error:") << " Some packages don't exist so "
                                     "won't update any specified."
                 << endl;
            exit(EXIT_FAILURE);
        }
        std::for_each(packages, packages + num, [&cur](char* p) {
            path path = cur / p;
            if (not exists(path)) {
                cerr << RED("Error:")
                     << " Directory you specified to update (" << p
                     << ") does not exist." << endl
                     << "It did when we safety checked!" << endl;
                exit(EXIT_FAILURE);
            }
            update_directory(std::move(path));
        });
    }
    if (HAS_ERROR) {
        exit(EXIT_FAILURE);
    }
}
