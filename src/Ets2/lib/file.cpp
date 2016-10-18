#include "precomp.hpp"
#include "file.hpp"

#include <fstream>
#include <cerrno>

void read_file(const std::wstring& filename, std::string& contents) {
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (file) {
		file.seekg(0, std::ios::end);
		std::streampos size = file.tellg();
		if (size > std::numeric_limits<size_t>::max()) {
			throw std::length_error("The file is too big.");
		}
		contents.resize((size_t) size);
		file.seekg(0, std::ios::beg);
		file.read(&contents[0], contents.size());
		file.close();
		return;
	}
	throw(errno);
}

void write_file(const std::string& contents, const std::wstring& filename) {
	std::ofstream file(filename, std::ios::out | std::ios::binary);
	if (file) {
		file << contents;
		return;
	}
	throw(errno);
}