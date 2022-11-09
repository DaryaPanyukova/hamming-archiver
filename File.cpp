#include "File.h"


File::File(std::string& file_path) {
    path = file_path;
    name = path.substr(path.find_last_of("/\\") + 1);;
    std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
    size = file.tellg(); // TODO: Is it actually the size of file?-
    file.close();
}

File::File(std::string& file_name, uint16_t file_size) : name(file_name),
                                                         size(file_size) {}
