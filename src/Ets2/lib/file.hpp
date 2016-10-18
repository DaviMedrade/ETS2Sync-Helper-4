#pragma once

#include <string>

void read_file(const std::wstring& filename, std::string& contents);
void write_file(const std::string& contents, const std::wstring& filename);