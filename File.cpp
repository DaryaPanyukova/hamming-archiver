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

File::File(const std::string& file_name, uint16_t file_size) : name(file_name),
                                                               size_e(file_size) {}

void File::ReadChar(char& sym, std::ifstream& stream) {
    stream.read(&sym, 1);
}

void File::WriteChar(char& sym, std::ofstream& stream) {
    stream.write(&sym, 1);
}

void File::SetPath(const std::string& archive_path) {
    path = archive_path + name;
}
