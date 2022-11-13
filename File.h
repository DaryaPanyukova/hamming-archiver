#ifndef LABWORK_4_DARYAPANYUKOVA_FILE_H
#define LABWORK_4_DARYAPANYUKOVA_FILE_H

#include <cstdint>
#include <iostream>
#include <fstream>


class File {
public:
    static const uint8_t kname_size = 9; // TODO: actually 255 or in fileheader: name_size, name

    std::string name;
    std::string path;
    uint64_t size;
    uint8_t padding;


    File(std::string& file_path);

    File(std::string
         & file_name,
         uint16_t file_size
    );


    void ReadChar(char& sym, std::ifstream* stream);

};


#endif //LABWORK_4_DARYAPANYUKOVA_FILE_H
