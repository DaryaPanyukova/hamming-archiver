#include "Archive.h"
#include <Hamming.h>

// TODO: delete[] char

Archive::Archive(std::string& path) {
    path_ = path;
    OpenInputStream();
    block_length_ = DecodeNum(sizeof(block_length_), *this);
    file_num_ = 0;

    while (input_stream_.peek() != EOF) {
        File new_file;
        new_file.name_len = DecodeNum(sizeof(new_file.name_len), *this);
        new_file.name = DecodeString(new_file.name_len, *this);
        new_file.size_e = DecodeNum(sizeof(new_file.size_e), *this);
        new_file.padding = DecodeNum(sizeof(new_file.padding), *this);
        files_list_.push_back(new_file);
        file_num_++;
        input_stream_.seekg(new_file.size_e, std::ios::cur);
    }
    CloseInputStream();
}

Archive::Archive(std::string& path, std::string* filepaths, uint64_t file_num,
                 uint16_t block_length) {
    path_ = path;
    block_length_ = block_length;
    file_num_ = file_num;
    OpenOutputStream();

    // archive header
    EncodeNum(block_length, *this);

    // write files into archive
    for (int i = 0; i < file_num_; ++i) {
        AddFile(filepaths[i]);
    }
    CloseOutputStream();
}

void Archive::AddFile(std::string& filepath) {
    File new_file(filepath);

    // FileHeader
    EncodeNum(new_file.name_len, *this); // length of decoded filename
    EncodeString(new_file.name, new_file.name_len, *this); // filename

    size_t ind = output_stream_.tellp();
    output_stream_.seekp(ind + sizeof(new_file.size_e) * 2 +
    sizeof(new_file.padding) * 2);

    EncodeFile(new_file, block_length_, *this);

    output_stream_.seekp(ind);
    EncodeNum(new_file.size_e, *this); // length of decoded filename
    EncodeNum(new_file.padding, *this);

    output_stream_.seekp(new_file.size_e, std::ios::cur);

    files_list_.push_back(new_file);
}

void Archive::PrintFileList() {
    for (size_t i = 0; i < file_num_; ++i) {
        std::cout << files_list_[i].name << '\n';
    }
}

void Archive::WriteChar(char sym) {
    char buf[1];
    buf[0] = sym;
    output_stream_.write(buf, 1);
}

void Archive::CloseInputStream() {
    if (!input_stream_.is_open()) {
        return;
    }
    input_stream_.close();
}

void Archive::CloseOutputStream() {
    if (!output_stream_.is_open()) {
        return;
    }
    output_stream_.close();
}

void Archive::OpenInputStream() {
    if (output_stream_.is_open()) {
        output_stream_.close();
        return;
    }
    input_stream_.open(path_, std::ios::in | std::ios::binary);
}

void Archive::OpenOutputStream() {
    if (input_stream_.is_open()) {
        input_stream_.close();
    }
    output_stream_.open(path_, std::ios::out | std::ios::binary);

}

void Archive::ReadChar(char& sym) {
    char buf[1];
    input_stream_.read(buf, 1);
    sym = buf[0];
}
