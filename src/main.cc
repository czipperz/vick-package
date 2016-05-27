/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include <iostream>

#include "color.hh"
#include "create_package.hh"
#include "install_packages.hh"
#include "parse_packages.hh"
#include "print_helper.hh"
#include "remove_packages.hh"
#include "search_packages.hh"
#include "update_packages.hh"

using namespace std;
using namespace boost::filesystem;

static int
parse_package_list(int& argc, char**& argv, path& package_list) {
    if (argc >= 1 and (strcmp(argv[0], "--package-list") == 0 or
                       strcmp(argv[0], "-p") == 0)) {
        if (argc == 1) {
            cerr << RED("Error:") << " Need a package list file "
                                     "when given argument `"
                 << argv[1] << "`." << endl
                 << BOLD("Hint:")
                 << " give a package list as an argument." << endl;
            return 1;
        }
        package_list = argv[2];
        argc -= 2;
        argv += 2;
    } else {
        package_list = path(argv[-2]).parent_path() / "packagelist";
    }
    return 0;
}

int main(int argc, char** argv) {
    // make args not worry about program name
    argc--;
    argv++;
    create_directory("plugins");

    if (argc == 0) {
        cerr << RED("Error:") << " Need a command argument." << endl;
        print_usage(argv[-1]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[0], "--help") == 0 or
        strcmp(argv[0], "help") == 0) {
        print_usage(argv[-1]);
        return EXIT_SUCCESS;
    } else if (strcmp(argv[0], "update") == 0 or
               strcmp(argv[0], "u") == 0) {
        update_packages(argc - 1, argv + 1);
    } else if (strcmp(argv[0], "install") == 0 or
               strcmp(argv[0], "i") == 0) {
        if (argc == 1) {
        install_error:
            cerr << RED("Error:") << " Need some packages to install "
                                     "when installing packages."
                 << endl
                 << BOLD("Hint:") << " give some packages to install."
                 << endl;
            return EXIT_FAILURE;
        }
        path package_list;
        argc--;
        argv++;
        if (parse_package_list(argc, argv, package_list)) {
            goto install_error;
        }
        install_packages(package_list, argc, argv);
    } else if (strcmp(argv[0], "remove") == 0 or
               strcmp(argv[0], "r") == 0) {
        if (argc < 2) {
            cerr << RED("Error:") << " Need some packages to remove "
                                     "when removing packages."
                 << endl
                 << BOLD("Hint:") << " give some packages to remove."
                 << endl;
            return EXIT_FAILURE;
        }
        remove_packages(argc - 1, argv + 1);
    } else if (strcmp(argv[0], "search") == 0 or
               strcmp(argv[0], "s") == 0) {
        if (argc > 3) {
            cerr << RED("Error:")
                 << " Need at most three arguments when searching."
                 << endl
                 << BOLD("Hint:")
                 << " give a search query (or leave it blank)"
                 << endl;
            return EXIT_FAILURE;
        }
        path package_list;
        argc--;
        argv++;
        if (parse_package_list(argc, argv, package_list)) {
            exit(EXIT_FAILURE);
        }
        search_packages(package_list, argc - 1, argv + 1);
    } else if (strcmp(argv[0], "create") == 0 or
               strcmp(argv[0], "init") == 0 or
               strcmp(argv[0], "new") == 0) {
        if (argc == 1) {
            create_package(NULL);
        } else {
            for (auto argi = 1; argi != argc; ++argi) {
                create_package(argv[argi]);
            }
        }
    } else {
        cerr << RED("Error:") << " Invalid command \"" << argv[1]
             << "\"." << endl
             << BOLD("Hint:")
             << " Use update, install, or remove instead." << endl;
        return EXIT_FAILURE;
    }
}
