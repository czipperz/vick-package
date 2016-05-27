/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <iostream>

#include "color.hh"
#include "parse_packages.hh"
#include "search_packages.hh"

void search_packages(const boost::filesystem::path& package_list,
                     int argc, const char* const* arguments) {
    using namespace std;
    using namespace boost::filesystem;
    // ensure valid path to packages
    {
        path p;
        try {
            p = arguments[-2];
        } catch (filesystem_error&) {
            goto filesystemerr;
        }
        if (not is_regular_file(p)) {
        filesystemerr:
            cerr << RED("Error:") << " Package list (" << package_list
                 << ") is not a valid file" << endl;
            exit(EXIT_FAILURE);
        }
    }

    map<string, pair<string, vector<string> > > pacstash =
        parse_packages(arguments[-2]);

// define common loop body as only if changes
#define loop(cond)                                                   \
    for (auto it = pacstash.begin(); it != pacstash.end(); ++it) {   \
        if (cond) {                                                  \
            cout << BOLD(it->first) << endl;                         \
        }                                                            \
    }

    if (argc) {
        loop(it->first.find(*arguments) != std::string::npos);
    } else {
        loop(true);
    }
}
