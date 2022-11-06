#ifndef LABWORK_4_DARYAPANYUKOVA_FILE_H
#define LABWORK_4_DARYAPANYUKOVA_FILE_H

#include <cstdint>
#include <iostream>
#include <fstream>


struct File {
    static const uint8_t kname_size = 8;
    static const uint8_t kmax_file_size = 64;

    std::string name;
    std::string path;
    uint64_t size;

    File(std::string& path);
};


#endif //LABWORK_4_DARYAPANYUKOVA_FILE_H
