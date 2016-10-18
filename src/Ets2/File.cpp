#include "precomp.hpp"
#include "File.hpp"

#include "lib/file.hpp"
#include "../ShowLastWindowsError.hpp"

#include <zlib.h>

#include <algorithm>

namespace Ets2 {
	const std::string File::SII_MAGIC_ENCRYPTED = "ScsC";
	const std::string File::SII_MAGIC_TEXT = "SiiN";
	const std::string File::SII_MAGIC_BINARY = "BSII";

	const unsigned char File::AES_KEY[32] = {
		0x2A, 0x5F, 0xCB, 0x17, 0x91, 0xD2, 0x2F, 0xB6, 0x02, 0x45, 0xB3, 0xD8, 0x36, 0x9E, 0xD0, 0xB2, 0xC2, 0x73, 0x71, 0x56, 0x3F, 0xBF, 0x1F, 0x3C, 0x9E, 0xDF, 0x6B, 0x11, 0x82, 0x5A, 0x5D, 0x0A
	};

	bool File::read(const std::wstring fileName, std::string& contents) {
		std::string data;
		try {
			::read_file(fileName, data);
		} catch (int) {
			return false;
		}
		bool ret = unpack(data, contents, fileName);
		return ret;
	}

	bool File::parse(const std::string& contents, const Parser::Sii::ParseCallback& callback) {
		//wxLogDebug("\n\nFile Contents:========================\n%s\n====================================\n", contents);
		std::string magic(contents, 0, 4);
		if (magic == SII_MAGIC_TEXT) {
			return Parser::Sii::Text::parse(contents, callback);
		} else if (magic == SII_MAGIC_BINARY) {
			wxLogError(L"Unfortunately, binary saves aren't supported yet. Please create the save using a text format.");
			return false;
			//bool r = SiiParserBinary::parse(contents, callback);
			//return r;
		}
		return false;
	}

	bool File::unpack(const std::string& data, std::string& plaintext, const std::wstring& fileName) {
		std::string magic(data, 0, 4);
		plaintext.resize(0);
		if (magic == SII_MAGIC_TEXT || magic == SII_MAGIC_BINARY) { // text
			plaintext = std::move(data);
			return true;
		} else if (magic == SII_MAGIC_ENCRYPTED) { // encrypted
			std::string ciphertext;
			ciphertext.assign(&data[36], data.length() - 36);
			ENCRYPTED_DATA * encData = (ENCRYPTED_DATA *)&ciphertext[0];
			if (!decrypt(encData, ciphertext.length(), fileName)) {
				return false;
			}
			// decrypted, now we just need to inflate
			plaintext.resize(encData->dataSize);
			if (!inflate(encData, ciphertext.length(), plaintext, fileName)) {
				return false;
			}
			return true;
		}
		return false;
	}

	bool File::decrypt(ENCRYPTED_DATA * data, size_t dataSize, const std::wstring& fileName) {
		// Oh, boy...
		HCRYPTPROV providerHandle;
		if (!CryptAcquireContext(&providerHandle, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
			ShowLastWindowsError(L"CryptAcquireContext");
			return false;
		}

		AES256_KEY_BLOB keyStruct;
		keyStruct.header.bType = PLAINTEXTKEYBLOB;
		keyStruct.header.bVersion = CUR_BLOB_VERSION;
		keyStruct.header.aiKeyAlg = CALG_AES_256;
		keyStruct.header.reserved = 0;
		keyStruct.keySize = sizeof(AES_KEY);
		memcpy(keyStruct.key, AES_KEY, sizeof(AES_KEY));
		HCRYPTKEY keyHandle;
		
		if (!CryptImportKey(providerHandle, (BYTE *)&keyStruct, sizeof(keyStruct) + 32, NULL, 0, &keyHandle)) {
			ShowLastWindowsError(L"CryptImportKey");
		} else {
			if (!CryptSetKeyParam(keyHandle, KP_IV, data->iv, 0)) {
				ShowLastWindowsError(L"CryptSetKeyParam (IV)");
				return false;
			}

			DWORD cipherMode = CRYPT_MODE_CBC;
			if (!CryptSetKeyParam(keyHandle, KP_MODE, (BYTE *)&cipherMode, 0)) {
				ShowLastWindowsError(L"CryptSetKeyParam (Cipher Mode)");
				return false;
			}

			const size_t offset = offsetof(ENCRYPTED_DATA, ciphertext);
			const size_t blockSize = 16; // block size for AES

			size_t cypherTextSize = dataSize - offset;

			size_t numBlocks = cypherTextSize / 16;
			if (cypherTextSize % 16) {
				++numBlocks;
			}

			size_t currentBlock = 0;
			size_t nextBlockStart = (std::min)((currentBlock + 1) * blockSize, cypherTextSize);
			DWORD currentBlockSize = nextBlockStart - (currentBlock * blockSize);
			while (currentBlock < numBlocks) {
				if (!CryptDecrypt(keyHandle, NULL, currentBlock == (numBlocks - 1), NULL, data->ciphertext + currentBlock * blockSize, &currentBlockSize)) {
					ShowLastWindowsError((std::wstring(L"CryptDecrypt (file “") + fileName + std::wstring(L"”)")).data());
					return false;
				}
				++currentBlock;
				nextBlockStart = (std::min)((currentBlock + 1) * blockSize, cypherTextSize);
				currentBlockSize = nextBlockStart - (currentBlock * blockSize);
			}

			if (!CryptDestroyKey(keyHandle)) {
				ShowLastWindowsError(L"CryptReleaseContext");
				// CryptDecrypt didn't fail, so fall through
			}
		}
		if (!CryptReleaseContext(providerHandle, 0)) {
			ShowLastWindowsError(L"CryptReleaseContext");
			// CryptDecrypt didn't fail, so fall through
		}
		return true;
	}

	bool File::inflate(const ENCRYPTED_DATA * data, size_t dataSize, std::string& plaintext, const std::wstring& /* fileName */) {
		plaintext.resize(data->dataSize);
		z_stream stream;
		stream.zalloc = Z_NULL;
		stream.zfree = Z_NULL;
		stream.opaque = Z_NULL;
		stream.avail_in = dataSize - offsetof(ENCRYPTED_DATA, ciphertext);
		stream.next_in = (Bytef *)data->ciphertext;
		stream.avail_out = plaintext.length();
		stream.next_out = (Bytef *)&plaintext[0];

		if (inflateInit(&stream) != Z_OK) {
			return false;
		}
		int ret = ::inflate(&stream, Z_NO_FLUSH | Z_FINISH);
		if (ret != Z_STREAM_END) {
			return false;
		}
		inflateEnd(&stream);
		return true;
	}
}