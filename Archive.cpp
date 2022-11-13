#include "Archive.h"
// TODO: delete[] char

template<typename Type>
char* f(Type peremennaya) {
    char* masssivchik = new char[sizeof(peremennaya)];
    for (size_t i = 0; i < sizeof(peremennaya); ++i) {
        masssivchik[i] = static_cast<char>(peremennaya >> (i * 8));
    }
    return masssivchik;
}

size_t BufToInt(char* buf, size_t buf_size) {
    char* end = buf + buf_size - 1;
    size_t result = 0;
    while (end != buf) {
        result = (result << 8) + static_cast<uint8_t>(*end);
        end--;
    }
    result = (result << 8) + static_cast<uint8_t>(*end);
    return result;
}

Archive::Archive(std::string& path) {
    std::ifstream archive;
    path_ = path;
    archive.open(path_, std::ios::in | std::ios::binary);
    if (!archive.is_open()) {
        file_num_ = 0;
    } else {
        // read archive header
        char block_length_buf[sizeof(block_length_)];
        archive.read(block_length_buf, sizeof(block_length_));
        block_length_ = BufToInt(block_length_buf, sizeof(block_length_));

        std::cout << "----------------------------------------\n";
        std::cout << "block length for Hamming code: " << block_length_ << '\n';
        std::cout << "Files:" << '\n';
        // TODO: decode

        uint64_t cur_pos = archive.tellg();
        while (archive.peek() != EOF) {
            // read file_name
            char buf_file_name[File::kname_size];
            archive.read(buf_file_name, File::kname_size);
            cur_pos += File::kname_size;

            // read file_size
            uint64_t file_size; // TODO: могут быть проблемы если поменять тип size внутри структуры File
            char file_size_buf[sizeof(File::size)];
            archive.read(file_size_buf, sizeof(File::size));
            file_size = BufToInt(file_size_buf, sizeof(File::size));
            cur_pos += sizeof(File::size);

            std::string file_name(buf_file_name, File::kname_size);
            File file(file_name, file_size);
            files_list_.push_back(file);

            cur_pos += file_size;
            archive.seekg(cur_pos);
        }
        file_num_ = files_list_.size();
        archive.close();
    }
}

Archive::Archive(std::string& path, std::string* filepaths, uint64_t file_num,
                 uint16_t block_length) {
    path_ = path;
    block_length_ = block_length;
    file_num_ = 0;
    OpenInputStream();

    // archive header
    char* block_size_buf = f(block_length);
    stream.write(block_size_buf, sizeof(block_length));
    // TODO: code block_size

    // write files into archieve
    for (int i = 0; i < file_num; ++i) {
        AddFile(filepaths[i]);
    }

    file_num_ = file_num;
    CloseStream();
}


void Archive::AddFile(std::string& filepath) {
    File new_file(filepath);
    std::ifstream file;
    file.open(filepath, std::ios::in | std::ios::binary);

    file_num_++;
    files_list_.push_back(new_file);

    // FileHeader

    // filename
    stream.write(new_file.name.c_str(), File::kname_size);

    // size of file
    char* file_size_buf = f(new_file.size);
    stream.write(file_size_buf, sizeof(new_file.size));

    // TODO: code file_name and file_size with Hamming code

    char buf[1];
    for (int i = 0; i < new_file.size; ++i) {
        file.read(buf, 1);
        stream.write(buf, 1);
    }
    // TODO: code file with Hamming code
    file.close();
}

void Archive::PrintFileList() {
    for (size_t i = 0; i < file_num_; ++i) {
        std::cout << files_list_[i].name << '\n';
    }
}

void Archive::WriteCString(char* str, size_t size) {
    stream.write(str, size);
}

void Archive::WriteChar(char sym) {
    char buf[1];
    buf[0] = sym;
    stream.write(buf, 1);
}

void Archive::CloseStream() {
    if (!stream.is_open()) {
        return;
    }
    stream.close();
}

void Archive::OpenInputStream() {
    if (stream.is_open()) {
        return;
    }
    stream.open(path_, std::ios::in | std::ios::binary);
}
void Archive::OpenOutputStream() {
    if (stream.is_open()) {
        return;
    }
    stream.open(path_, std::ios::out | std::ios::binary);
}

void Archive::ReadChar(char& sym) {
    char buf[1];
    stream.read(buf, 1);
    sym = buf[0];
}
/*
Archive Archive::Merge(Archive& lhs, Archive& rhs, std::string& path) {
    uint16_t block_length = 15; // TODO: какой в реальности должен быть размер?
    Archive result;
    // decode lhs, rhs
    //

    // write archive header - block_length
    // записать тупо информацию друг за другом, обновить размеры
}


*/