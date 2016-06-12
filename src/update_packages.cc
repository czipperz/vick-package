/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include <cstdio>

#include "color.hh"

using namespace boost::filesystem;
using namespace std;

static bool HAS_ERROR = false;

static void update_directory(path p) {
    printf("%s%s:%s ", color::bold, p.filename().c_str(),
           color::clear);
    fflush(stdout);
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
                fprintf(stderr, "%sError:%s Package you specified to "
                                "update (%s) does not exist.\n",
                        color::red, color::clear, p);
                shouldthrow = true;
            }
        });
        if (shouldthrow) {
            fprintf(stderr, "%sError:%s Some packages don't exist so "
                            "won't update any specified.\n",
                    color::red, color::clear);
            exit(EXIT_FAILURE);
        }
        std::for_each(packages, packages + num, [&cur](char* p) {
            path path = cur / p;
            if (not exists(path)) {
                fprintf(stderr, "%sError:%s Directory you specified "
                                "to update (%s) does not exist.\nIt "
                                "did when we safety checked!\n",
                        color::red, color::clear, p);
                exit(EXIT_FAILURE);
            }
            update_directory(std::move(path));
        });
    }
    if (HAS_ERROR) {
        exit(EXIT_FAILURE);
    }
}
