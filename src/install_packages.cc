#include "install_packages.hh"
#include "parse_packages.hh"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <iostream>
#include <map>

using namespace std;
static void calculate_dependency(
    const string& p,
    vector<pair<string, bool> >& pac,
    const map<string, pair<string, vector<string> > >& pacstash)
{
    auto itr = pacstash.find(p);
    if (itr == pacstash.end()) {
        cout << "Package not recognized: " << p << endl;
        throw 11;
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

    path package_list;
    try {
        package_list = packages[-2];
    } catch (filesystem_error&) {
        goto filesystemerr;
    }
    if (not is_regular_file(package_list)) {
    filesystemerr:
        cout << "First argument (" << packages[-2] << ") is not a valid file" << endl;
        throw 4;
    }

    map<string, pair<string, vector<string> > > pacstash =
        parse_packages(package_list.c_str());

    const path cur = current_path() / "plugins";
    auto n = num_packages;
    auto p = packages;
    auto shouldthrow = false;
    for (; n; --n, ++p) {
        path comb = cur / *p;
        if (exists(comb)) {
            cout << "Warning: Package you specified to install (" << *p
                 << ") is already installed." << endl
                 << "  Will remove and reinstall!" << endl;
        }
        if (pacstash.find(*p) == pacstash.end()) {
            cout << "Package you specified to install (" << *p
                 << ") is not a key in the package list you gave (" << packages[-2]
                 << ")." << endl;
            shouldthrow = true;
        }
    }

    if (shouldthrow) {
        cout << "Some packages aren't registered so won't install any specified" << endl;
        throw 10;
    }

    vector<pair<string, bool> > pac;
    pac.reserve(num_packages);
    for (auto i = packages; i != packages + num_packages; ++i) {
        pac.emplace_back(*i, true);
    }
    calculate_dependencies(pac, pacstash);
    sort(begin(pac), end(pac));

    cout << "Packages to install (" << pac.size() << "):";
    for (auto& s : pac) {
        cout << "  " << s.first;
    }
    cout << endl << "Continue with installation? [Y,n] ";

    char c = getchar();
    if (not (c == 'Y' or c == 'y' or c == '\n')) {
        throw 3;
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
                cout << itr->first << ':' << endl;
                system("git pull");
                cout << endl;
                continue;
            }
        }
        cout << itr->first << ':' << endl;
        auto str = string("git clone '") + itr->second.first + "' '" +
                   itr->first + "'";
        int i = system(str.c_str());
        if (i)
            throw i;
        cout << endl;
    }
}
