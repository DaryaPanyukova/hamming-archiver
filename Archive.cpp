#include "Archive.h"
#include <Hamming.h>

uint16_t GetEncodedLength(uint16_t block_length_d) {
    uint16_t pow = 1;
    uint16_t result = 0;
    for (uint16_t i = 0;; ++i) {
        if (block_length_d == 0) {
            break;
        }
        if (pow == i) {
            pow = pow << 1;
        } else {
            block_length_d--;
        }
        result++;
    }
    return result + 1;
}


Archive::Archive(std::string& path) {
    path_ = path;
    file_num_ = 0;
    OpenInputStream();
    if (!input_stream_.is_open()) {
        return;
    }
    block_length_ = DecodeNum(sizeof(block_length_), *this);
    block_length_ = GetEncodedLength(block_length_);


    while (input_stream_.peek() != EOF) {
        File new_file;
        new_file.name_len = DecodeNum(sizeof(new_file.name_len), *this);
        new_file.name = DecodeString(new_file.name_len, *this);
        new_file.size_e = DecodeNum(sizeof(new_file.size_e), *this);
        new_file.padding = DecodeNum(sizeof(new_file.padding), *this);
        new_file.SetPath(path_);
        new_file.shift = input_stream_.tellg();

        files[new_file.name] = new_file;
        file_num_++;
        input_stream_.seekg(new_file.size_e, std::ios::cur);
    }
    CloseInputStream();
}

void Archive::Create(std::string& path, std::vector<std::string>& filepaths,
                     uint16_t block_length) {
    path_ = path;
    block_length_ = block_length;GetEncodedLength(block_length);
    file_num_ = filepaths.size();
    OpenOutputStream();

    // archive header
    EncodeNum(block_length_, *this);

    // write files into archive
    for (int i = 0; i < file_num_; ++i) {
        AddFile(filepaths[i]);
    }
    CloseOutputStream();
}

void Archive::AddFile(std::string& filepath) {
    OpenOutputStream();
    File new_file(filepath);

    // FileHeader
    EncodeNum(new_file.name_len, *this); // length of decoded filename
    EncodeString(new_file.name, new_file.name_len, *this); // filename

    size_t ind = output_stream_.tellp();
    output_stream_.seekp(ind + sizeof(new_file.size_e) * 2 +
                         sizeof(new_file.padding) * 2);

    new_file.shift = output_stream_.tellp();
    EncodeFile(new_file, block_length_, *this);

    output_stream_.seekp(ind);
    EncodeNum(new_file.size_e, *this); // length of decoded filename
    EncodeNum(new_file.padding, *this);

    output_stream_.seekp(new_file.size_e, std::ios::cur);

    files[new_file.name] = new_file;
}

void Archive::PrintFileList() {
    for (auto elem: files) {
        std::cout << elem.second.name << '\n';
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
    if (input_stream_.is_open()) {
        return;
    }
    input_stream_.open(path_, std::ios::in | std::ios::binary);
}

void Archive::OpenOutputStream() {
    if (input_stream_.is_open()) {
        input_stream_.close();
    }
    if (output_stream_.is_open()) {
        return;
    }
    output_stream_.open(path_, std::ios::out | std::ios::binary);

}

void Archive::ReadChar(char& sym) {
    char buf[1];
    input_stream_.read(buf, 1);
    sym = buf[0];
}

Archive::~Archive() {
    input_stream_.close();
    output_stream_.close();
}

void Archive::Extract(std::string& filename) {
    File file = files[filename];
    OpenInputStream();
    input_stream_.seekg(file.shift);
    DecodeFile(file, block_length_, *this, file.size_e);
    CloseInputStream();
}
