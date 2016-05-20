/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
         << "  <command> is update, install, remove, search, or new "
            "or init."
         << endl
         << "    - Abreviations of u, i, r, and s, respectively, can "
            "be "
            "used."
         << endl
         << "  <packages> are the packages you want to <command>."
         << endl
         << "    - Can be empty if <command> is update, in which "
            "case it "
            "updates all the packages."
         << endl;
}
