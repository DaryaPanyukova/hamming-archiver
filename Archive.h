#ifndef LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
#define LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H

#include <vector>
#include <iostream>
#include <File.h>



class Archive {
private:
    std::vector<File> files_list_;
    uint64_t file_num_;
    std::string path_;
    uint16_t block_length_;
    std::ifstream input_stream_;
    std::ofstream output_stream_;

public:
    Archive(std::string& path);

    Archive(std::string& path, std::string* filepaths, uint64_t file_num,
            uint16_t block_length);

    void AddFile(std::string& filepath);

    void PrintFileList();

    void WriteChar(char sym);

    void ReadChar(char& sym);

    void OpenInputStream();

    void CloseInputStream();

    void CloseOutputStream();

    void OpenOutputStream();
};


#endif //LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
