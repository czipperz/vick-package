#include <boost/filesystem.hpp>
#include <iostream>

void update_packages(int num_packages, char** packages)
{
    using namespace boost::filesystem;
    using namespace std;
    const path cur = current_path() / "plugins";
    auto fun = [](path p) {
        cout << p.filename().c_str() << ": " << flush;
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
                cout << "Package you specified to update (" << *p
                     << ") does not exist." << endl;
                shouldthrow = true;
            }
        }
        if (shouldthrow) {
            cout << "Some packages don't exist so won't update any specified." << endl;
            throw 8;
        }
        for (; num_packages; --num_packages, ++packages) {
            path p = cur / *packages;
            if (not exists(p)) {
                cout << "Directory you specified to update (" << *packages
                     << ") does not exist." << endl
                     << "It did when we safety checked!" << endl;
                throw 9;
            }
            fun(p);
        }
    } else {
        for_each(directory_iterator(cur), directory_iterator(), fun);
    }
}
