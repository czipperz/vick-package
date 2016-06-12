/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cstdio>

#include "color.hh"
#include "parse_packages.hh"
#include "search_packages.hh"

void search_packages(const boost::filesystem::path& package_list,
                     int argc, const char* const* arguments) {
    using namespace std;
    using namespace boost::filesystem;
    // ensure valid path to packages
    {
        if (not is_regular_file(package_list)) {
            fprintf(stderr, "%sError:%s Package list (%s) is not a "
                            "valid file.\n",
                    color::red, color::clear, package_list.c_str());
            exit(EXIT_FAILURE);
        }
    }

    map<string, pair<string, vector<string> > > pacstash =
        parse_packages(package_list.string());

// define common loop body as only if changes
#define loop(cond)                                                   \
    for (auto it = pacstash.begin(); it != pacstash.end(); ++it) {   \
        if (cond) {                                                  \
            printf("%s%s%s\n", color::bold, it->first.c_str(),       \
                   color::clear);                                    \
        }                                                            \
    }

    if (argc) {
        loop(it->first.find(*arguments) != std::string::npos);
    } else {
        loop(true);
    }
}
