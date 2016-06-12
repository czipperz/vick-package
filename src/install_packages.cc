/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <map>

#include "color.hh"
#include "install_packages.hh"
#include "parse_packages.hh"

using namespace std;
static void calculate_dependency(
    const string& p, vector<pair<string, bool> >& pac,
    const map<string, pair<string, vector<string> > >& pacstash) {
    if (p.empty())
        return;
    auto itr = pacstash.find(p);
    if (itr == pacstash.end()) {
        std::fprintf(stderr,
                     "%sError:%s Package not regonized: %s.\n",
                     color::red, color::clear, p.c_str());
        exit(EXIT_FAILURE);
    }
    for (auto& a : itr->second.second) {
        if (find_if(begin(pac), end(pac),
                    [&a](pair<string, bool>& p) {
                        return a == p.first;
                    }) == end(pac)) {
            pac.emplace_back(a, false);
            calculate_dependency(a, pac, pacstash);
        }
    }
}
static void calculate_dependencies(
    vector<pair<string, bool> >& pac,
    const map<string, pair<string, vector<string> > >& pacstash) {
    for (auto& p : pac) {
        calculate_dependency(p.first, pac, pacstash);
    }
}

void install_packages(const boost::filesystem::path& package_list,
                      int num_packages, const char* const* packages) {
    using namespace boost::filesystem;

    // ensure valid path to packages
    {
        path p;
        try {
            p = packages[-2];
        } catch (filesystem_error&) {
            goto filesystemerr;
        }
        if (not is_regular_file(p)) {
        filesystemerr:
            std::fprintf(stderr, "%sError:%s Package list (%s) is "
                                 "not a valid file.\n",
                         color::red, color::clear,
                         package_list.c_str());
            exit(EXIT_FAILURE);
        }
    }

    map<string, pair<string, vector<string> > > pacstash =
        parse_packages(package_list.c_str());

    const path cur = current_path() / "plugins";
    auto n = num_packages;
    auto p = packages;
    auto should_throw = false;
    for (; n; --n, ++p) {
        path comb = cur / *p;
        if (exists(comb)) {
            std::fprintf(stderr, "%sWarning:%s Package you specified "
                                 "to install (%s) is already "
                                 "installed.\n  Will remove and "
                                 "reinstall!\n",
                         color::bold, color::clear, *p);
        }
        if (pacstash.find(*p) == pacstash.end()) {
            std::fprintf(stderr, "%sError:%s Package you specified "
                                 "to install (%s) is not a key in "
                                 "the package list you gave (%s).\n",
                         color::red, color::clear, *p,
                         package_list.c_str());
            should_throw = true;
        }
    }

    if (should_throw) {
        std::fprintf(stderr, "%sError:%s Some packages aren't "
                             "registered so won't install any "
                             "specified.\n",
                     color::red, color::clear);
        exit(EXIT_FAILURE);
    }

    vector<pair<string, bool> > pac;
    pac.reserve(num_packages);
    for (auto i = packages; i != packages + num_packages; ++i) {
        // packages explicitly asked for are deleted and recloned
        pac.emplace_back(*i, true);
    }
    // packages pulled in as dependencies are just updated
    calculate_dependencies(pac, pacstash);
    // alphabetize so output is pretty
    sort(begin(pac), end(pac));

    printf("%sPackages to install/update (%zu):%s", color::bold,
           pac.size(), color::clear);
    for (auto& s : pac) {
        printf("  %s", s.first.c_str());
    }
    printf("\n%sContinue with installation? [Y,n]%s ", color::bold,
           color::clear);

    char c = getchar();
    if (not(c == 'Y' or c == 'y' or c == '\n')) {
        exit(EXIT_FAILURE);
    }
    putchar('\n');
    for (auto& pack : pac) {
        chdir(cur.c_str());
        auto itr = pacstash.find(pack.first);
        auto p = cur / itr->first;
        if (exists(p)) {
            if (pack.second) {
                remove_all(p);
            } else {
                chdir(p.c_str());
                printf("%s%s:%s\n", color::bold, itr->first.c_str(),
                       color::clear);
                fflush(stdout);
                system("git pull");
                putchar('\n');
                continue;
            }
        }
        printf("%s%s:%s\n", color::bold, itr->first.c_str(),
               color::clear);
        fflush(stdout);
        auto str = string("git clone '") + itr->second.first + "' '" +
                   itr->first + "'";
        int i = system(str.c_str());
        if (i)
            exit(EXIT_FAILURE);
        putchar('\n');
    }
}
