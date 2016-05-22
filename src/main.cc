/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <iostream>
#include <boost/filesystem.hpp>

#include "color.hh"
#include "create_package.hh"
#include "parse_packages.hh"
#include "update_packages.hh"
#include "remove_packages.hh"
#include "install_packages.hh"
#include "search_packages.hh"
#include "print_helper.hh"

int main(int argc, char** argv) {
    using namespace std;
    using namespace boost::filesystem;

    /* make args not worry about program name */
    argc--;
    argv++;
    create_directory("plugins");

    if (argc < 2) {
        cerr << RED("Error:") << " Need at least two arguments."
             << endl;
        print_usage(argv[-1]);
        return 5;
    }
    if (strcmp(argv[1], "update") == 0 or strcmp(argv[1], "u") == 0) {
        try {
            update_packages(argc - 2, argv + 2);
        } catch (int i) {
            return i;
        }
    } else if (strcmp(argv[1], "install") == 0 or
               strcmp(argv[1], "i") == 0) {
        if (argc < 3) {
            cerr << RED("Error:") << " Need at least three arguments "
                                     "when not updating."
                 << endl
                 << BOLD("Hint:") << " give some packages to install"
                 << endl;
            return 7;
        }
        try {
            install_packages(argc - 2, argv + 2);
        } catch (int i) {
            return i;
        }
    } else if (strcmp(argv[1], "remove") == 0 or
               strcmp(argv[1], "r") == 0) {
        if (argc < 3) {
            cerr << RED("Error:") << " Need at least three arguments "
                                     "when not updating."
                 << endl
                 << BOLD("Hint:") << " give some packages to remove"
                 << endl;
            return 14;
        }
        try {
            remove_packages(argc - 2, argv + 2);
        } catch (int i) {
            return i;
        }
    } else if (strcmp(argv[1], "search") == 0 or
               strcmp(argv[1], "s") == 0) {
        if (argc > 3) {
            cerr << RED("Error:")
                 << " Need at most three arguments when searching."
                 << endl
                 << BOLD("Hint:")
                 << " give a search query (or leave it blank)"
                 << endl;
            return 15;
        }
        try {
            search_packages(argc - 2, argv + 2);
        } catch (int i) {
            return i;
        }
    } else if (strcmp(argv[1], "new") == 0 or
               strcmp(argv[1], "create") == 0) {
        if (argc > 3) {
            cerr << RED("Error:") << " Need at most three arguments "
                                     "when creating package."
                 << endl;
            return 16;
        }
        try {
            create_package(argc - 2, argv + 2);
        } catch (int i) {
            return i;
        }
    } else {
        cerr << RED("Error:") << " Invalid command \"" << argv[1]
             << "\"." << endl
             << BOLD("Hint:")
             << " Use update, install, or remove instead." << endl;
        return 6;
    }
}
