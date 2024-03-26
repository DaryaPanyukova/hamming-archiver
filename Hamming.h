#include <cstring>
#include "Archive.h"
#include <cmath>
#include <fstream>


inline void AddChar(std::vector<bool>& dest, char sym) {
    for (uint8_t i = 8; i-- > 0;) {
        int res = (sym & (1 << i)) >> i;
        dest.push_back(res);
    }
}

void EncodeBlock(const std::vector<bool>& block, size_t block_beg,
                 size_t block_end, std::vector<bool>& result) {
    uint64_t pow = 1;

    size_t begin = result.size();
    result.push_back(false);
    size_t size = 1;

    size_t ind_res = 1;
    size_t ind_block = block_beg;
    for (; ind_block < block_end; ind_res++) {
        if (ind_res == pow) {
            result.push_back(false);
            size++;
            pow = pow << 1;
        } else {
            result.push_back(block[ind_block++]);
            size++;
        }
    }

    pow = 1;
    for (; pow < result.size(); pow = pow << 1) {
        bool flag = false;
        for (size_t j = pow; j < size; j += pow << 1) {
            for (size_t i = 0; i + j < size && i < pow; ++i) {
                flag = flag ^ result[i + j + begin];
            }
        }
        result[pow + begin] = flag;
    }

    for (size_t i = 1; i < result.size(); ++i) {
        result[begin] = result[begin] ^ result[i];
    }
}

void EncodeString(const std::string& str, size_t size,
                  Archive& dest, uint16_t block_length = 4) {
    // block_length = 4 for File Headers

    size_t size_bits = size * 8;
    std::vector<bool> decoded;
    for (size_t i = 0; i < size; ++i) {
        AddChar(decoded, str[i]);
    }

    std::vector<bool> encoded;
    for (size_t block_beg = 0;
         block_beg < size_bits; block_beg += block_length) {
        size_t end = (block_beg + block_length <= size_bits ?
                      block_beg + block_length : size_bits);
        EncodeBlock(decoded, block_beg, end, encoded);

        size_t i = 0;
        for (; i < encoded.size(); i += 8) {
            char tmp = 0;
            for (uint8_t j = 0; j < 8; j++) {
                bool value = encoded[i + 7 - j];
                if (value) {
                    tmp = tmp | (1 << j);
                } else {
                    tmp = tmp & (~(1 << j));
                }
            }
            dest.WriteChar(tmp);
        }
        for (uint8_t j = 0; i < encoded.size(); ++i, ++j) {
            encoded[j] = encoded[i];
        }
        encoded.resize(encoded.size() % 8);
    }
}

void EncodeFile(File& file, uint16_t block_length, Archive& dest) {
    size_t size_bits = file.size_d * 8;
    size_t blocks_num = ceil((double) size_bits / block_length);

    std::vector<bool> encoded;
    std::ifstream input;
    input.open(file.path, std::ios::in | std::ios::binary);

    std::vector<bool> decoded;
    for (size_t i = 0; i < blocks_num; ++i) {

        size_t cur_block_size = size_bits - block_length * i < block_length ?
                                size_bits - block_length * i : block_length;
        while (decoded.size() < cur_block_size) {
            char sym;
            file.ReadChar(sym, input);
            AddChar(decoded, sym);
        }
        EncodeBlock(decoded, 0, cur_block_size, encoded);

        size_t k = 0;
        for (k = 0; k + 7 < encoded.size(); k += 8) {
            char tmp = 0;
            for (uint8_t j = 0; j < 8; j++) {
                bool value = encoded[k + 7 - j];
                if (value) {
                    tmp = tmp | (1 << j);
                } else {
                    tmp = tmp & (~(1 << j));
                }
            }
            dest.WriteChar(tmp);
            file.size_e++;
        }
        for (uint8_t j = 0; k < encoded.size(); ++k, ++j) {
            encoded[j] = encoded[k];
        }
        for (size_t j = cur_block_size; j < decoded.size(); ++j) {
            decoded[j - cur_block_size] = decoded[j];
        }
        encoded.resize(encoded.size() % 8);
        decoded.resize(decoded.size() - cur_block_size);
    }
    file.padding = (8 - encoded.size()) % 8;
    if (file.padding == 0) {
        return;
    }
    for (uint8_t i = file.padding; i < 8; ++i) {
        encoded.push_back(false);
    }
    char tmp = 0;
    for (uint8_t j = 0; j < 8; j++) {
        bool value = encoded[7 - j];
        if (value) {
            tmp = tmp | (1 << j);
        } else {
            tmp = tmp & (~(1 << j));
        }
    }
    dest.WriteChar(tmp);
    file.size_e++;
}


template<typename Type>
void EncodeNum(Type num, Archive& archive, uint16_t block_length = 4) {
    // block_length = 4 for File Headers
    uint8_t size = sizeof(num);
    char str[size];
    for (uint8_t i = 0; i < size * 8; i += 8) {
        char sym = 0;
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t ind_in_sym = 7 - j;
            uint8_t ind = i + ind_in_sym;
            bool bit = (num & (1 << ind)) >> ind;
            if (bit) {
                sym = sym | (1 << ind_in_sym);
            } else {
                sym = sym & (~(1 << ind_in_sym));
            }
        }
        str[size - 1 - i / 8] = sym;
    }
    std::string tmp(str, size);
    EncodeString(tmp, size, archive, block_length);
}


std::vector<bool>
DecodeBlock(const std::vector<bool>& encoded, size_t block_beg,
            size_t block_end, std::vector<bool>& result) {
    size_t block_size = block_end - block_beg;

    bool is_even = false; // even amount of mistakes
    for (size_t i = block_beg + 1; i < block_end; ++i) {
        is_even = is_even ^ encoded[i];
    }
    is_even = (is_even == encoded[block_beg]);

    size_t pow = 1;

    size_t ind_mistake = 0; // sum of wrong' indexes == index of the mistake

    for (; pow < block_size; pow <<= 1) {
        bool flag = false;
        for (size_t i = pow + 1; i < block_size; i += pow << 1) {
            for (size_t j = 0; i + j < block_size && j < pow; ++j) {
                flag = flag ^ encoded[i + j + block_beg];
            }
        }
        if (flag != encoded[pow + block_beg]) {
            ind_mistake += pow;
        }
    }

    pow = 1;
    for (size_t i = 1; i < block_size; ++i) {
        if (i != pow) {
            result.push_back(encoded[i + block_beg]);
        } else {
            pow = pow << 1;
        }
    }

    if (ind_mistake != 0) {
        if (!is_even) {
            std::cerr << "File corruption" << '\n';
        } else {
            //      result[ind_mistake] = !result[ind_mistake];
        }
    }
    return result;
}

std::string DecodeString(size_t size_chars_d,
                         Archive& source, uint16_t block_length = 8) {
    // block_length = 4 for File Headers

    size_t size_chars_e =
            size_chars_d * 2; // len(encoded string) = 2 * len(decoded string)
    std::vector<bool> decoded;
    std::vector<bool> encoded;

    for (size_t i = 0; i < size_chars_e; ++i) {
        char sym;
        source.ReadChar(sym);
        AddChar(encoded, sym);
    }

    size_t blocks_num = encoded.size() / block_length;

    for (size_t block_ind = 0; block_ind < blocks_num; ++block_ind) {
        size_t beg = block_ind * block_length;
        size_t end = (block_ind + 1) * (block_length);
        DecodeBlock(encoded, beg, end, decoded);
    }

    std::string result(decoded.size() / 8, 0);
    for (size_t i = 0; i < decoded.size(); i += 8) {
        char tmp = 0;
        for (uint8_t j = 0; j < 8; j++) {
            bool value = decoded[i + 7 - j];
            if (value) {
                tmp = tmp | (1 << j);
            } else {
                tmp = tmp & (~(1 << j));
            }
        }
        result[i / 8] = tmp;
    }
    return result;
}


size_t
DecodeNum(uint8_t num_bytes_d, Archive& archive, uint16_t block_length = 8) {
    size_t result = 0;
    std::string str = DecodeString(num_bytes_d, archive, block_length);
    for (uint8_t i = 0; i < str.size(); ++i) {
        result = (result << 8) + static_cast<uint8_t >(str[i]);
    }
    return result;
}


void DecodeFile(File& dest, uint16_t block_length,
                Archive& source, size_t size_e) {

    size_t num_bits_e = size_e * 8;
    size_t blocks_num = ceil((double) num_bits_e / block_length);

    std::vector<bool> decoded;
    std::ofstream output;
    output.open(dest.path, std::ios::out | std::ios::binary);

    std::vector<bool> encoded;
    for (size_t i = 0; i < blocks_num; ++i) {

        size_t cur_block_size = block_length;
        if (i == blocks_num - 1) {
            cur_block_size = block_length - dest.padding;
        }

        while (encoded.size() < cur_block_size) {
            char sym;
            source.ReadChar(sym);
            AddChar(encoded, sym);
        }
        DecodeBlock(encoded, 0, cur_block_size, decoded);

        size_t k = 0;
        for (k = 0; k + 7 < decoded.size(); k += 8) {
            char tmp = 0;
            for (uint8_t j = 0; j < 8; j++) {
                bool value = decoded[k + 7 - j];
                if (value) {
                    tmp = tmp | (1 << j);
                } else {
                    tmp = tmp & (~(1 << j));
                }
            }
            dest.WriteChar(tmp, output);
        }

        for (uint8_t j = 0; k < decoded.size(); ++k, ++j) {
            decoded[j] = decoded[k];
        }
        for (size_t j = cur_block_size; j < encoded.size(); ++j) {
            encoded[j - cur_block_size] = encoded[j];
        }
        decoded.resize(decoded.size() % 8);
        encoded.resize(encoded.size() - cur_block_size);
    }
    output.close();
}