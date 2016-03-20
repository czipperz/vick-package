#include <iostream>

#include "color.hh"
#include "print_helper.hh"

void print_usage(const char* program_name)
{
    using namespace std;
    cerr << BOLD("Usage:") << endl
         << "  " << program_name
         << " <packagelist> <command> <packages>" << endl
         << "  <packagelist> is a file that has a list of package, "
            "url pairs."
         << endl
         << "  <command> is update, install, or remove." << endl
         << "    - Abreviations of u, i, and r, respectively, can be "
            "used."
         << endl
         << "  <packages> are the packages you want to <command>."
         << endl
         << "    - Can be empty if <command> is update, in which "
            "case it "
            "updates all the packages."
         << endl;
}
