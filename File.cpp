#include "File.h"


File::File(const std::string& file_path) {
    padding = 0;
    size_e = 0;

    path = file_path;
    name = path.substr(path.find_last_of("/\\") + 1);
    name_len = name.size();

    std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
    size_d = file.tellg();
    file.close();
}

File::File(std::string& file_name, uint16_t file_size) : name(file_name),
                                                         size_e(file_size) {}

void File::ReadChar(char& sym, std::ifstream* stream) {
    char buf[1];
    stream->read(buf, 1);
    sym = buf[0];
}

void File::WriteChar(char& sym, std::ofstream* stream) {
    char buf[1];
    buf[0] = sym;
    stream->write(buf, 1);
}

void File::SetPath(std::string& archive_path) {
    size_t found;
    found = archive_path.find_last_of("/\\");
    path = archive_path.substr(0, found + 1) + name;
}
