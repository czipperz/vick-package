/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <iostream>
#include <boost/filesystem.hpp>

#include "color.hh"
#include "print_helper.hh"

using namespace boost::filesystem;
using namespace std;

void print_usage(const char* program_name)
{
    cerr << BOLD("Usage:") << endl
         << "  " << program_name << " <command> <options> <packages>\n"
         << "    <command> is `update`, `install`, `remove`, `search`, or\n"
         << "                 `create`, `init` or `new` (which are synonyms).\n"
         << "      - Abreviations of `u`, `i`, `r`, and `s`, respectively, can be\n"
         << "        used.\n"
         << "      - When given `i`, `install`, `s`, or `search`, the option\n"
         << "        `-p` or `--package-list` can be given to specify where to look\n"
         << "        for package information.  It is automatically inferred based\n"
         << "        on the program's name.  For this instance it would be:\n"
         << "          " << path(program_name).parent_path() / "packagelist" << ".\n"
         << "      - There are no other options.\n"
         << "    <packages> are the packages you want to <command>.\n"
         << "      - Can be empty if <command> is update, in which case it updates\n"
         << "        all the packages.\n";
}
