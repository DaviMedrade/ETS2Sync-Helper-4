#pragma once

#include "Parser/Sii.hpp"

#include <string>

namespace Ets2 {
	class File {
	public:
		// Reads a game file and, if encrypted or binary, convert to plaintext
		// contents: string in which to place the file contents
		static bool read(const std::wstring fileName, std::string& contents);

		// Holds the pointers to the output buffer for each attribute
		struct ATTRIBUTE_POINTERS {
			char * attrStart;
			size_t attrLength;
			char * valueStart;
			size_t valueLength;
			bool valueUtf8;
		};

		// Parses a plaintext game file
		//static bool parse(const std::string& contents, std::string& output, std::vector<ATTRIBUTE_POINTERS>& attributes);
		static bool parse(const std::string& contents, const Parser::Sii::ParseCallback& callback);

	private:
		// Convenience map to the encrypted data
		struct ENCRYPTED_DATA {
			unsigned char iv[16];
			unsigned int dataSize;
			unsigned char ciphertext[1]; // It's not actually just one character, you know...
		};

		// Stores an AES-256 key blob in the format expected by Windows
		struct AES256_KEY_BLOB {
			PUBLICKEYSTRUC header;
			DWORD keySize;
			BYTE key[32];
		};

		// Magic headers
		static const std::string SII_MAGIC_ENCRYPTED;
		static const std::string SII_MAGIC_TEXT;
		static const std::string SII_MAGIC_BINARY;

		// The key used to encrypt/decrypt files with "ScsC" magic
		static const unsigned char AES_KEY[32];

		// Unpacks game file data and returns its plaintext version (regardless of whether it's encrypted or binary)
		// data: file contents (modified but meaningless on return)
		// plaintext: output
		// fileName: file name (for error messages)
		static bool unpack(const std::string& data, std::string& plaintext, const std::wstring& fileName);

		// Decrypts the data (in-place)
		static bool decrypt(ENCRYPTED_DATA * data, size_t dataSize, const std::wstring& fileName);

		// Inflates the data
		// data: deflated data
		// plaintext: inflated data
		// fileName: file name (for error messages)
		static bool inflate(const ENCRYPTED_DATA * data, size_t dataSize, std::string& plaintext, const std::wstring& fileName);
	};
}
