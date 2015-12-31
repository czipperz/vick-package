#ifndef HEADER_GUARD_INSTALL_PACKAGES_H
#define HEADER_GUARD_INSTALL_PACKAGES_H

#ifdef __GNUC__
__attribute__ ((__nonnull__))
#endif
void install_packages(int num_packages, const char* const* packages);

#endif
