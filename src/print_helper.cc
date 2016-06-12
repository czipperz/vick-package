/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include <cstdio>

#include "color.hh"
#include "print_helper.hh"

using namespace boost::filesystem;
using namespace std;

void print_usage(const char* program_name) {
    fprintf(stderr,
            R"(%sUsage:%s
  %s <command> <options> <packages>
    <command is `update`, `install`, `remove`, `search`, or
                `create`, `init`, or `new` (which are synonyms).
      - Abbreviations of `u`, `i`, `r`, and `s`, respectively, can be
        used.
      - When given `i`, `install`, `s`, or `search`, the option
        `-p` or `--package-list` can be given to specify where to look
        for package information.  It is automatically inferred based
        on the program's name.  For this instance it would be:
          %s.
      - There are no other options.
    <packages> are the packages you want to <command>.
      - Can be empty if <command> is update, in which case it updates
        all the packages.
)",
            color::bold, color::clear, program_name,
            (path(program_name).parent_path() / "packagelist")
                .c_str());
}
