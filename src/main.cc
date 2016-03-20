#include <iostream>
#include <boost/filesystem.hpp>

#include "color.hh"
#include "parse_packages.hh"
#include "update_packages.hh"
#include "remove_packages.hh"
#include "install_packages.hh"
#include "print_helper.hh"

int main(int argc, char** argv)
{
    using namespace std;
    using namespace boost::filesystem;

    /* make args not worry about program name */
    argc--; argv++;
    create_directory("plugins");

    if (argc < 2) {
        cerr << RED("Error:") << " Need at least two arguments." << endl;
        print_usage(argv[-1]);
        return 5;
    }
    if (not strcmp(argv[1], "update") or not strcmp(argv[1], "u")) {
        try {
            update_packages(argc-2, argv+2);
        } catch(int i) {
            return i;
        }
    } else if (not strcmp(argv[1], "install") or not strcmp(argv[1], "i")) {
        if (argc < 3) {
            cerr << RED("Error:") << " Need at least three arguments "
                                     "when not updating."
                 << endl
                 << BOLD("Hint:") << " give some packages to install"
                 << endl;
            return 7;
        }
        try {
            install_packages(argc-2, argv+2);
        } catch(int i) {
            return i;
        }
    } else if (not strcmp(argv[1], "remove") or not strcmp(argv[1], "r")) {
        if (argc < 3) {
            cerr << RED("Error:") << " Need at least three arguments "
                                     "when not updating."
                 << endl
                 << BOLD("Hint:") << " give some packages to remove" << endl;
            return 14;
        }
        try {
            remove_packages(argc-2, argv+2);
        } catch(int i) {
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
