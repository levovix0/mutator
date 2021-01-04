#pragma once

#include <vector>

#include "basic.h"

_bit_vector arithmetic_coding(const std::vector<uchar>& data);
void arithmetic_decoding(_bit_vector& data, std::vector<uchar>& res);
void arithmetic_decoding2(_bit_vector& data, std::vector<uchar>& res);

uchar ppm(const std::vector<uchar>& data, std::vector<uchar>& res, u64 g = 0);

void AC_pak(const std::vector<uchar>& data, std::vector<uchar>& res);
void AC_pak64(const std::vector<uchar>& data, std::vector<uchar>& res);
std::vector<unsigned char> AC_unpak(std::vector<unsigned char>& A);