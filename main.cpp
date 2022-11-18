#include <iostream>
#include <Archive.h>
#include <vector>
#include <cstring>


struct OptionFlags {
    bool create, list, extract, append, del, concatenate;

    OptionFlags() : create(false), list(false),
                    extract(false), append(false),
                    del(false), concatenate(false) {};
};


int main(int argc, char** argv) {
    OptionFlags options;
    std::vector<std::string> filepaths;
    std::string archive_filepath;
    uint16_t block_size = 4;

    for (size_t i = 1; i < argc; ++i) {
        char* term = argv[i];
        if (strcmp(term, "--create") == 0 || strcmp(term, "-c") == 0) {
            options.create = true;
        } else if (strcmp(term, "--list") == 0 || strcmp(term, "-l") == 0) {
            options.list = true;
        } else if (strcmp(term, "--extract") == 0 || strcmp(term, "-x") == 0) {
            options.extract = true;
        } else if (strcmp(term, "--append") == 0 || strcmp(term, "-a") == 0) {
            options.append = true;
        } else if (strcmp(term, "--delete") == 0 || strcmp(term, "-d") == 0) {
            options.del = true;
        } else if (strcmp(term, "--concatenate") == 0 ||
                   strcmp(term, "-A") == 0) {
            options.concatenate = true;
        } else if (strcmp(term, "--hamming") == 0 ||
                   strcmp(term, "-h") == 0) {
            block_size = atoi(argv[++i]);
        } else if (strcmp(term, "-f") == 0) {
            archive_filepath = argv[++i];
        } else {
            std::string tmp = term;
            if (tmp.size() <= 7 || tmp.substr(7) != "--file=") {
                filepaths.push_back(term);
            } else {
                archive_filepath = tmp.substr(7, tmp.size());
            }
        }
    }


    Archive archive(archive_filepath);
    if (options.create) {
        archive.Create(archive_filepath, filepaths, block_size);
    } else if (options.list) {
        archive.PrintFileList();
    } else if (options.extract) {
        for (auto path: filepaths) {
            archive.Extract(path);
        }
    } else if (options.append) {
        for (auto path: filepaths) {
            archive.AddFile(path);
        }
    } else if (options.del) {
        for (auto path: filepaths) {
            archive.RemoveFile(path);
        }
    } else if (options.concatenate) {
        Archive first(filepaths[0]);
        Archive second(filepaths[1]);
        archive.Merge(first, second, block_size);
    }
}