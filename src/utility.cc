#include "utility.hh"
#include <boost/filesystem.hpp>

bool exists_plugin(const std::string& name) {
    using namespace boost::filesystem;
    return exists(path("plugins") / name);
}
