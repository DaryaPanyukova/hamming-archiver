#include <iostream>
#include <Archive.h>
#include <fstream>


int main(int argc, char** argv) {
    std::string type = "decode";
    if (type == "code") {
        std::string first = "D:\\ITMOlabs\\basics_c++\\file1.txt";
        std::string second = "D:\\ITMOlabs\\basics_c++\\file2.txt";
        std::string third = "D:\\ITMOlabs\\basics_c++\\final.bmp";

        std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";

        int file_num = 3;
        std::string* filenames = new std::string[file_num];
        filenames[0] = first;
        filenames[1] = second;
        filenames[2] = third;

        Archive archive(archive_path, filenames, file_num, 111);
    } else {
        std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";
        Archive archive(archive_path);
        archive.PrintFileList();
    }
}