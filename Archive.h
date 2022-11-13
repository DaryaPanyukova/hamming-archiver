#ifndef LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
#define LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H

#include <vector>
#include <iostream>
#include <File.h>
#include <map>


class Archive {
private:
   // std::vector<File> files_list_;
    std::map <std::string, File> files;
    uint64_t file_num_;
    std::string path_;
    uint16_t block_length_;
    std::ifstream input_stream_;
    std::ofstream output_stream_;

public:
    Archive() = default;

    Archive(std::string& path);

    ~Archive();

    void Create(std::string& path, std::vector<std::string>& filepaths, uint16_t block_length);

    void AddFile(std::string& filepath);

    void PrintFileList();

    void WriteChar(char sym);

    void ReadChar(char& sym);

    void OpenInputStream();

    void CloseInputStream();

    void CloseOutputStream();

    void OpenOutputStream();

    void Extract(std::string& filename);
};


#endif //LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
