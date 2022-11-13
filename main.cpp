#include <iostream>
#include <Archive.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <Hamming.h>
#include <File.h>

/*
int main(int argc, char** argv) {
    std::string type = "decode";
    if (type == "code") {
        std::string first = "D:\\ITMOlabs\\basics_c++\\file1.txt";
        std::string second = "D:\\ITMOlabs\\basics_c++\\file2.txt";

        std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";

        int file_num = 2;
        std::string* filenames = new std::string[file_num];
        filenames[0] = first;
        filenames[1] = second;

        Archive archive(archive_path, filenames, file_num, 111);
    } else {
        std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";
        Archive archive(archive_path);
        archive.PrintFileList();
    }
}
*/


int main() {
    std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";

    Archive archive(archive_path);

    archive.OpenOutputStream();
    uint32_t k = 18565;
    EncodeNum(k, archive);
    archive.CloseStream();

    archive.OpenInputStream();
    size_t res = DecodeNum(32, archive);
    archive.CloseStream();


    std::cout << '\n';
    std::cout << res;
    //  std::string path = "D:\\ITMOlabs\\basics_c++\\file1.txt";


}
