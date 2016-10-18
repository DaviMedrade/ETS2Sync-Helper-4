#pragma once

#include "Sii.hpp"

#include <map>
#include <stdexcept>

namespace Ets2::Parser::Sii {
	class Binary {
	public:
		static bool parse(const std::string& data, const ParseCallback& callback);

		Binary(const std::string& data, const ParseCallback& callback);
		~Binary();

		bool parseSuccessful();

	private:
		enum PropertyType {
			INVALID,
			STRING,
			STRING_ARRAY,
			TOKEN,
			TOKEN_ARRAY,
			FLOAT,
			FLOAT_ARRAY,
			FLOAT_TRIPLE,
			FLOAT_QUAD,
			UINT32_TRIPLE,
			INT32_TRIPLE,
			INT32_TRIPLE_ARRAY,
			FLOAT_QUAD_ARRAY,
			FLOAT_TRIPLE_QUAD, // Not a quad of triples, but a triple followed by four bytes (?) followed by a quad
			INT32,
			UINT32,
			UINT32_ARRAY,
			UINT16,
			UINT16_ARRAY,
			INT64,
			UINT64,
			INT64_ARRAY,
			BOOLEAN,
			BOOLEAN_ARRAY,
			UNIT,
			UNIT_ARRAY
		};

		static const int PropertyTypeByIndex[62];
		static const int PropertyArrayTypesToBase[62];

		#pragma pack(push, 1)
		struct UnitDeclarationHeader {
			int flag;
			int format;
			char index1;
			int index2;
			int nameLength;
		};

		struct PropertyDeclarationHeader {
			int type;
			int nameLength;
		};
		#pragma pack(pop)

		struct PropertyDeclaration {
			PropertyType type;
			std::string name;
		};

		struct UnitDeclaration {
			int index;
			std::string name;
			std::vector<PropertyDeclaration *> properties;
		};

		class EndOfData : public std::runtime_error {
		public:
			EndOfData() : runtime_error("no more data") {
			};
		};

		class ParseError : public std::runtime_error {
		public:
			ParseError(std::string what) : runtime_error("parse error: " + what) {
			}
		};

		bool mParseSuccessful;
		const std::string& mData;
		const ParseCallback& mCallback;
		const char * mCursor;
		const char * mEnd;

		std::map<int, UnitDeclaration *> mUnitDeclarations;

		bool parse();

		// Takes mCursor to be pointing to a record n bytes long.
		// Returns a pointer to the first byte of the record (i.e. mCursor when it is called).
		// Advances mCursor to the first byte after the record.
		// Throws EndOfData if reading n bytes would read past the end of data.
		void * getBytes(size_t n);

		int getInt();

		void readUnitDeclarations();
		void readPropertyDeclarations(UnitDeclaration * unit);

		bool readUnits();
		bool readProperty(PropertyType type, const std::string& name, bool arrayMember, uint32_t arrayIndex);

		void readValue(PropertyType type, std::string& dest, std::string * destRaw);

		void readUnitName(std::string& dest);
		int decodeToken(uint64_t token, char dest[12]);
	};
}
