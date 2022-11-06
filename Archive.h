#ifndef LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
#define LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H

#include <fstream>
#include <File.h>
#include <vector>

class Archive {
private:
    std::vector <File> files_list_;
    uint64_t file_num_;
    std::string path_;
    uint16_t block_length_;

public:
    Archive(std::string& path);

    Archive(std::string& path, std::string* filepaths, uint64_t file_num,
             uint16_t block_length);

    void AddFile(std::string& filepath);
};


#endif //LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
