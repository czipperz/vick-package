/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_REMOVE_PACKAGES_H
#define HEADER_GUARD_REMOVE_PACKAGES_H

#ifdef __GNUC__
__attribute__ ((__nonnull__))
#endif
void remove_packages(int num_packages, const char* const* packages);

#endif
