#include <iostream>
#include <Archive.h>
#include <vector>



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

        Archive archive(archive_path, filenames, file_num, 17);
    } else {
        std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";
        Archive archive(archive_path);
        archive.PrintFileList();
    }
}


/*
int main() {
    std::string archive_path = "D:\\ITMOlabs\\basics_c++\\archive.haf";

    Archive archive(archive_path);

    archive.OpenOutputStream();
    std::string filename = "D:\\ITMOlabs\\basics_c++\\file2.txt";
    File original(filename);
    EncodeFile(original, 13, archive);
    archive.CloseStream();

    archive.OpenInputStream();
    std::string filename_decoded =  "D:\\ITMOlabs\\basics_c++\\file2_decoded.txt";
    File decoded(filename_decoded);
    decoded.padding = original.padding;
    DecodeFile(decoded, 19, archive, 120);
    archive.CloseStream();

    //  std::string path = "D:\\ITMOlabs\\basics_c++\\file1.txt";


}
*/