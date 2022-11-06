#include "Archive.h"

Archive::Archive(std::string& path) {
    std::ifstream archive;
    path_ = path;
    archive.open(path_, std::ios::in | std::ios::binary);
    if (!archive.is_open()) {
        file_num_ = 0;
    } else {
        // archive header
        char buf_block_length[] = {};
        archive.read(buf_block_length, sizeof(block_length_));
        block_length_ = atoi(buf_block_length);

        // file header

        // TODO: read file_name, file_size.
        //  for (!eof)    {Add file_name into files_list_, skip file_size}.
        //  Then update file_num

        // TODO: decode

        char buf_file_size[] = {};
        archive.read(buf_block_length, sizeof(block_length_));
        block_length_ = atoi(buf_block_length);
    }
}

Archive::Archive(std::string& path, std::string* filepaths, uint64_t file_num,
                   uint16_t block_length) {
    path_ = path;
    file_num_ = file_num;
    block_length_ = block_length;

    std::ofstream archive;

    // archive header
    archive.open(path_, std::ios::out | std::ios::binary);
    char tmp_block_length[] = {static_cast<char>(block_length)};
    archive.write(tmp_block_length, sizeof(block_length_));
    archive.close();

    // write files into archieve
    for (int i = 0; i < file_num_; ++i) {
        AddFile(filepaths[i]);
    }
}


void Archive::AddFile(std::string& filepath) {
    File new_file(filepath);
    file_num_++;
    files_list_.push_back(new_file);

    // write into file
    std::ofstream archive;
    archive.open(path_, std::ios::out | std::ios::binary);

    std::ifstream file;
    file.open(path_, std::ios::in | std::ios::binary);

    // FileHeader
    archive.write(new_file.name.c_str(), File::kname_size);
    char file_size[] = {static_cast<char>(new_file.size)};
    archive.write(file_size, File::kmax_file_size);

    // TODO: code file_name and file_size with Hamming code

    // FileData
    char buf[1];
    while (file >> buf) {
        archive.write(buf, 1);
    }
    // TODO: code file with Hamming code
    archive.close();
    file.close();
}
