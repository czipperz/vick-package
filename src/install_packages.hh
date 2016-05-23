/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_INSTALL_PACKAGES_H
#define HEADER_GUARD_INSTALL_PACKAGES_H

#include <boost/filesystem.hpp>

#ifdef __GNUC__
__attribute__ ((__nonnull__))
#endif
void install_packages(const boost::filesystem::path& package_list,
                      int num_packages, const char* const* packages);

#endif
