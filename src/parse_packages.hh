#ifndef HEADER_GUARD_PARSE_PACKAGES_H
#define HEADER_GUARD_PARSE_PACKAGES_H

#include <map>
#include <string>
#include <vector>

std::map<std::string, std::pair<std::string, std::vector<std::string> > >
parse_packages(std::string filename);

#endif
