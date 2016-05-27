/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_PARSE_PACKAGES_H
#define HEADER_GUARD_PARSE_PACKAGES_H

#include <map>
#include <string>
#include <vector>

std::map<std::string,
         std::pair<std::string, std::vector<std::string> > >
parse_packages(std::string filename);

#endif
