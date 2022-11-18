#ifndef LABWORK_4_DARYAPANYUKOVA_FILE_H
#define LABWORK_4_DARYAPANYUKOVA_FILE_H

#include <cstdint>
#include <iostream>
#include <fstream>


class File {
public:
    std::string name;
    std::string path;
    uint64_t size_d; // size of encoded file, bytes
    uint64_t size_e; // size of encoded file, bytes
    uint8_t padding; // bits
    uint8_t name_len; // length of the decoded filename
    uint64_t shift_filedata; // position in archive
    uint64_t shift_fileheader;
    File() = default;

    File(const std::string& file_path);

    File(std::string& file_name, uint16_t file_size);

    void ReadChar(char& sym, std::ifstream* stream);

    void WriteChar(char& sym,  std::ofstream* stream);

    void SetPath(std::string& archive_path);

};


#endif //LABWORK_4_DARYAPANYUKOVA_FILE_H
