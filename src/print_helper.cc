#include <iostream>

#include "print_helper.hh"

void print_usage(const char* program_name)
{
    using namespace std;
    cout << "Usage:" << endl
         << "  " << program_name << " <packagelist> <command> <packages>"
         << endl
         << "  <packagelist> is a file that has a list of package, url pairs."
         << endl
         << "  <command> is update, install, or remove."
         << endl
         << "    - Abreviations of u, i, and r, respectively, can be used."
         << endl
         << "  <packages> are the packages you want to <command>." << endl
         << "    - Can be empty if <command> is update, in which case it "
            "updates all the packages."
         << endl;
}

template<class T>
static void printargs(int argc, T&& argv)
{
    using namespace std;
    switch (argc) {
        case 0:
            return;
        case 1:
            cout << argv[0];
            return;
        case 2:
            cout << argv[0] << " and " << argv[1];
            return;
        default:
            for (int i = 0; i != argc - 1; ++i) {
                cout << argv[i] << ", ";
            }
            cout << "and " << argv[argc - 1];
            return;
    }
}

void print_args(const std::vector<std::string>& argv) {
    printargs(argv.size(), argv);
}

void print_args(int argc, const char* const* argv) {
    printargs(argc, argv);
}
