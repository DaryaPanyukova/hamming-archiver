#ifndef LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
#define LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H

#include <vector>
#include <iostream>
#include <File.h>
#include <map>


class Archive {
private:
    std::map <std::string, File> files;
    std::string filename_;
    std::string path_;
    uint16_t block_length_;
    std::ifstream input_stream_;
    std::ofstream output_stream_;

public:
    Archive() = default;

    Archive(std::string& path);

    ~Archive();

    void Create(std::string& path, std::vector<std::string>& filepaths, uint16_t block_length = 4);

    void AddFile(const std::string& filepath);

    void AddArchive(Archive& new_archive);

    void PrintFileList();

    void WriteChar(char sym);

    void ReadChar(char& sym);

    void OpenInputStream();

    void CloseInputStream();

    void CloseOutputStream();

    void OpenOutputStream();

    void Extract(std::string& filename);

    void ExtractTo(const std::string& filename, const std::string& where_to);

    void Merge(Archive& first, Archive& second, uint16_t block_length = 4);

    void RemoveFile(const std::string& filename);
};


#endif //LABWORK_4_DARYAPANYUKOVA_ARCHIVE_H
