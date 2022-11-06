#include "File.h"


File::File(std::string& path) {
    path = path;
    name = path.substr(path.find_last_of("/\\") + 1);;
    std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
    size = file.tellg(); // TODO: Is it actually the size of file?-
}
