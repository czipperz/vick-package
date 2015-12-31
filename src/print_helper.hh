#ifndef HEADER_GUARD_PRINT_HELPER_H
#define HEADER_GUARD_PRINT_HELPER_H

#include <vector>
#include <string>

void print_usage(const char* program_name);
void print_args(int argc, const char* const* argv);
void print_args(const std::vector<std::string>& argv);

#endif
