#ifndef LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
#define LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H

#include <vector>
#include <iostream>
#include <File.h>


class Archive {
private:
    std::vector <File> files_list_;
    uint64_t file_num_;
    std::string path_;
    uint16_t block_length_;
    std::fstream stream;

public:
    Archive(std::string& path);

    Archive(std::string& path, std::string* filepaths, uint64_t file_num,
            uint16_t block_length);

    void AddFile(std::string& filepath);

    void PrintFileList();

    void WriteCString(char* str, size_t size);

    void WriteChar(char sym);

    void ReadChar(char& sym);

    Archive Merge(Archive& lhs, Archive& rhs, std::string& path);

    void OpenInputStream();
    void CloseStream();
    void OpenOutputStream();
};




#endif //LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
