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

std::string GetPath(const std::string& full_path) {
    size_t found;
    found = full_path.find_last_of("/\\");
    return full_path.substr(0, found + 1);
}

Archive::Archive(std::string& path) {
    filename_ = path;
    path_ = GetPath(path);

    OpenInputStream();
    if (!input_stream_.is_open()) {
        return;
    }
    block_length_ = DecodeNum(sizeof(block_length_), *this);
    block_length_ = GetEncodedLength(block_length_);


    while (input_stream_.peek() != EOF) {
        File new_file;
        new_file.shift_fileheader = input_stream_.tellg();
        new_file.name_len = DecodeNum(sizeof(new_file.name_len), *this);
        new_file.name = DecodeString(new_file.name_len, *this);
        new_file.size_e = DecodeNum(sizeof(new_file.size_e), *this);
        new_file.padding = DecodeNum(sizeof(new_file.padding), *this);
        new_file.SetPath(path_);
        new_file.shift_filedata = input_stream_.tellg();

        files[new_file.name] = new_file;
        input_stream_.seekg(new_file.size_e, std::ios::cur);
    }
    CloseInputStream();
}

void Archive::Create(std::string& path, std::vector<std::string>& filepaths,
                     uint16_t block_length) {
    filename_ = path;
    block_length_ = block_length;
    OpenOutputStream();

    // archive header
    EncodeNum(block_length_, *this);

    // write files into archive
    for (int i = 0; i < filepaths.size(); ++i) {
        AddFile(filepaths[i]);
    }
    CloseOutputStream();
}

void Archive::AddFile(const std::string& filepath) {
    OpenOutputStream();
    File new_file(filepath);
    new_file.shift_fileheader = output_stream_.tellp();

    // FileHeader
    EncodeNum(new_file.name_len, *this); // length of decoded filename
    EncodeString(new_file.name, new_file.name_len, *this); // filename

    size_t ind = output_stream_.tellp();
    output_stream_.seekp(ind + sizeof(new_file.size_e) * 2 +
                         sizeof(new_file.padding) * 2);
    new_file.shift_filedata = output_stream_.tellp();
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
    input_stream_.open(filename_, std::ios::in | std::ios::binary);
}

void Archive::OpenOutputStream() {
    if (input_stream_.is_open()) {
        input_stream_.close();
    }
    if (output_stream_.is_open()) {
        return;
    }
    output_stream_.open(filename_, std::ios::out | std::ios::binary);

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
    input_stream_.seekg(file.shift_filedata);
    DecodeFile(file, block_length_, *this, file.size_e);
    CloseInputStream();
}

void Archive::Merge(Archive& first, Archive& second, uint16_t block_length) {
    block_length_ = block_length;
    files.clear();

    OpenOutputStream();
    EncodeNum(block_length, *this);

    AddArchive(first);
    AddArchive(second);

    CloseOutputStream();
}

void Archive::ExtractTo(const std::string& filename,
                        const std::string& where_to) {
    File file = files[filename];
    file.path = where_to;
    OpenInputStream();
    input_stream_.seekg(file.shift_filedata);
    DecodeFile(file, block_length_, *this, file.size_e);
    CloseInputStream();
}

void Archive::AddArchive(Archive& new_archive) {
    new_archive.OpenInputStream();

    for (auto elem: new_archive.files) {
        std::string tmp_file = path_ + elem.first;
        new_archive.ExtractTo(elem.first, tmp_file);
        AddFile(tmp_file);
        std::remove(tmp_file.c_str());
    }
    new_archive.CloseInputStream();
}


void Archive::RemoveFile(const std::string& filename) {

    File remove_file = files[filename];
    files.erase(filename);
    OpenInputStream();

    uint64_t erase_from = remove_file.shift_fileheader;
    uint64_t erase_to = remove_file.shift_filedata + remove_file.size_e;


    std::string new_archive_name =
            filename_.substr(0, filename_.find_last_of("/\\") + 1) + "tmp.haf";
    std::ofstream new_archive;
    new_archive.open(new_archive_name, std::ios::out | std::ios::binary);

    while (input_stream_.tellg() < erase_from) {
        char buf[1];
        input_stream_.read(buf, 1);
        new_archive.write(buf, 1);
    }
    input_stream_.seekg(erase_to);
    while (input_stream_.peek() != EOF) {
        char buf[1];
        input_stream_.read(buf, 1);
        new_archive.write(buf, 1);
    }
    new_archive.close();
    CloseInputStream();

    std::remove(filename_.c_str());
    std::rename(new_archive_name.c_str(), filename_.c_str());
}
