/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <boost/filesystem.hpp>
#include <iostream>
#include <map>

#include "color.hh"
#include "install_packages.hh"
#include "parse_packages.hh"

using namespace std;
static void calculate_dependency(
    const string& p,
    vector<pair<string, bool> >& pac,
    const map<string, pair<string, vector<string> > >& pacstash)
{
    if (p.empty()) return;
    auto itr = pacstash.find(p);
    if (itr == pacstash.end()) {
        cerr << RED("Error:")
             << " Package not recognized: " << p << endl;
        exit(EXIT_FAILURE);
    }
    for (auto& a : itr->second.second) {
        if (find_if(begin(pac), end(pac), [&a](pair<string, bool>& p) {
                return a == p.first;
            }) == end(pac)) {
            pac.emplace_back(a, false);
            calculate_dependency(a, pac, pacstash);
        }
    }
}
static void calculate_dependencies(
    vector<pair<string, bool> >& pac,
    const map<string, pair<string, vector<string> > >& pacstash)
{
    for (auto& p : pac) {
        calculate_dependency(p.first, pac, pacstash);
    }
}

void install_packages(int num_packages, const char* const* packages) {
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
            cerr << RED("Error:") << " First argument ("
                 << packages[-2] << ") is not a valid file" << endl;
            exit(EXIT_FAILURE);
        }
    }

    map<string, pair<string, vector<string> > > pacstash =
        parse_packages(packages[-2]);

    const path cur = current_path() / "plugins";
    auto n = num_packages;
    auto p = packages;
    auto should_throw = false;
    for (; n; --n, ++p) {
        path comb = cur / *p;
        if (exists(comb)) {
            cerr << BOLD("Warning:")
                 << " Package you specified to install (" << *p
                 << ") is already installed." << endl
                 << "  Will remove and reinstall!" << endl;
        }
        if (pacstash.find(*p) == pacstash.end()) {
            cerr << RED("Error:")
                 << " Package you specified to install (" << *p
                 << ") is not a key in the package list you gave ("
                 << packages[-2] << ")." << endl;
            should_throw = true;
        }
    }

    if (should_throw) {
        cerr << RED("Error:") << " Some packages aren't registered "
                                 "so won't install any specified"
             << endl;
        exit(EXIT_FAILURE);
    }

    vector<pair<string, bool> > pac;
    pac.reserve(num_packages);
    for (auto i = packages; i != packages + num_packages; ++i) {
        pac.emplace_back(*i, true);
    }
    calculate_dependencies(pac, pacstash);
    sort(begin(pac), end(pac));

    cout << BOLD("Packages to install (" << pac.size() << "):");
    for (auto& s : pac) {
        cout << "  " << s.first;
    }
    cout << endl << BOLD("Continue with installation? [Y,n] ");

    char c = getchar();
    if (not (c == 'Y' or c == 'y' or c == '\n')) {
        exit(EXIT_FAILURE);
    }
    cout << endl;
    for (auto& pack : pac) {
        chdir(cur.c_str());
        auto itr = pacstash.find(pack.first);
        auto p = cur / itr->first;
        if (exists(p)) {
            if (pack.second) {
                remove_all(p);
            } else {
                chdir(p.c_str());
                cout << BOLD(itr->first) << ':' << endl;
                system("git pull");
                cout << endl;
                continue;
            }
        }
        cout << BOLD(itr->first) << ':' << endl;
        auto str = string("git clone '") + itr->second.first + "' '" +
                   itr->first + "'";
        int i = system(str.c_str());
        if (i)
            exit(EXIT_FAILURE);
        cout << endl;
    }
}
