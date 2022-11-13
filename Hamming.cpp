//#include <iostream>
//#include <vector>
//#include <cstring>
//#include "Archive.h"
//#include "File.h"
//
//#include <fstream>
//
//
//void AddChar(std::vector<bool>& dest, char sym) {
//    for (uint8_t i = 8; i-- > 0;) {
//        int res = (sym & (1 << i)) >> i;
//        dest.push_back(res);
//    }
//}
//
//void EncodeBlock(std::vector<bool>& block, size_t block_beg,
//                 size_t block_end, std::vector<bool>& result) {
//    uint64_t parity = 1;
//
//    size_t begin = result.size();
//    result.push_back(false);
//    size_t size = 1;
//
//    size_t ind_res = 1;
//    size_t ind_block = block_beg;
//    for (; ind_block < block_end; ind_res++) {
//        if (ind_res == parity) {
//            result.push_back(false);
//            size++;
//            parity = parity << 1;
//        } else {
//            result.push_back(block[ind_block++]);
//            size++;
//        }
//    }
//
//    parity = 1;
//    for (; parity < result.size(); parity = parity << 1) {
//        bool flag = false;
//        for (size_t j = parity; j < size; j += parity << 1) {
//            for (size_t i = 0; i + j + begin < size && i < parity; ++i) {
//                flag = flag ^ result[i + j + begin];
//            }
//        }
//        result[parity + begin] = flag;
//    }
//
//    for (size_t i = 1; i < result.size(); ++i) {
//        result[begin] = result[begin] ^ result[i];
//    }
//}
//
//void
//EncodeString(char* str, size_t size, Archive& dest, uint16_t block_length = 4) {
//    // block_length = 4 for File Headers
//
//    size_t size_bits = size * 8;
//    std::vector<bool> decoded;
//    for (size_t i = 0; i < size; ++i) {
//        AddChar(decoded, str[i]);
//    }
//
//    std::vector<bool> encoded;
//    for (size_t block_beg = 0;
//         block_beg < size_bits; block_beg += block_length) {
//        size_t end = (block_beg + block_length <= size_bits ?
//                      block_beg + block_length : size_bits);
//        EncodeBlock(decoded, block_beg, end, encoded);
//
//        size_t i = 0;
//        for (; i < encoded.size(); i += 8) {
//            char tmp = 0;
//            for (uint8_t j = 0; j < 8; j++) {
//                bool value = encoded[i + 7 - j];
//                if (value) {
//                    tmp = tmp | (1 << j);
//                } else {
//                    tmp = tmp & (~(1 << j));
//                }
//            }
//            dest.WriteChar(tmp);
//        }
//        for (uint8_t j = 0; i < encoded.size(); ++i, ++j) {
//            encoded[j] = encoded[i];
//        }
//        encoded.resize(encoded.size() % 8);
//    }
//}
//
//
//void EncodeFile(File& file, uint16_t block_length, Archive& dest) {
//    size_t size_bits = file.size * 8;
//
//    std::vector<bool> encoded;
//    std::ifstream input;
//    input.open(file.path, std::ios::out | std::ios::binary);
//
//    for (size_t block_beg = 0;
//         block_beg < size_bits; block_beg += block_length) {
//        size_t end = (block_beg + block_length <= size_bits ?
//                      block_beg + block_length : size_bits);
//        std::vector<bool> decoded;
//        for (size_t i = 0; i < end - block_beg; ++i) {
//            char sym;
//            file.ReadChar(sym, &input);
//            AddChar(decoded, sym);
//        }
//        EncodeBlock(decoded, 0, block_length, encoded);
//
//        size_t i = 0;
//        for (i = 0; i < encoded.size(); i += 8) {
//            char tmp = 0;
//            for (uint8_t j = 0; j < 8; j++) {
//                bool value = encoded[i + 7 - j];
//                if (value) {
//                    tmp = tmp | (1 << j);
//                } else {
//                    tmp = tmp & (~(1 << j));
//                }
//            }
//            dest.WriteChar(tmp);
//        }
//        for (uint8_t j = 0; i < encoded.size(); ++i, ++j) {
//            encoded[j] = encoded[i];
//        }
//        encoded.resize(encoded.size() % 8);
//    }
//    file.padding = encoded.size();
//}
//
//
//template<typename Type>
//void EncodeNum(Type num, Archive& archive, uint16_t block_length = 4) {
//    // block_length = 4 for File Headers
//    uint8_t size = sizeof(num);
//    char str[size];
//    for (uint8_t i = 0; i < size * 8; i += 8) {
//        char sym = 0;
//        for (uint8_t j = 0; j < 8; j++) {
//            uint8_t ind_in_sym = 7 - j;
//            uint8_t ind = i + ind_in_sym;
//            bool bit = (num & (1 << ind)) >> ind;
//            if (bit) {
//                sym = sym | (1 << ind_in_sym);
//            } else {
//                sym = sym & (~(1 << ind_in_sym));
//            }
//        }
//        str[size - 1 - i / 8] = sym;
//    }
//    EncodeString(str, size, archive);
//}
