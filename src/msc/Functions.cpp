//
// Created by ddl_blue on 7.10.18.
//

#include "../../include/msc/Functions.hpp"
#include <fstream>

using namespace NMsc;

/*----------------------------------------------------------------------*/
bool Functions::FileExtists(const std::string &path) {
    std::ifstream stream(path);

    return stream.good();

}