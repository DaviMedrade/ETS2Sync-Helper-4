#include "precomp.hpp"
#include "SiiBinary.hpp"

#define PARSER_CALLBACK(...) if (!mCallback(__VA_ARGS__)) return false;

namespace Ets2::Parser::Sii {
	const int Binary::PropertyTypeByIndex[62] = {
		0,
		/* 1 */ PropertyType::STRING,
		/* 2 */ PropertyType::STRING_ARRAY,
		/* 3 */ PropertyType::TOKEN,
		/* 4 */ PropertyType::TOKEN_ARRAY,
		/* 5 */ PropertyType::FLOAT,
		/* 6 */ PropertyType::FLOAT_ARRAY,
		0, 0,
		/* 9 */ PropertyType::FLOAT_TRIPLE,
		0, 0, 0, 0, 0, 0, 0,
		/* 17 */ PropertyType::UINT32_TRIPLE,
		/* 18 */ PropertyType::INT32_TRIPLE_ARRAY,
		0, 0, 0, 0, 0,
		/* 24 */ PropertyType::FLOAT_QUAD_ARRAY,
		/* 25 */ PropertyType::FLOAT_TRIPLE_QUAD,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 37 */ PropertyType::UINT32,
		/* 38 */ PropertyType::UINT32_ARRAY,
		/* 39 */ PropertyType::UINT32,
		/* 40 */ PropertyType::UINT32_ARRAY,
		0, 0,
		/* 43 */ PropertyType::UINT16,
		/* 44 */ PropertyType::UINT16_ARRAY,
		0, 0, 0, 0,
		/* 49 */ PropertyType::INT64,
		0,
		/* 51 */ PropertyType::UINT64,
		/* 52 */ PropertyType::INT64_ARRAY,
		/* 53 */ PropertyType::BOOLEAN,
		/* 54 */ PropertyType::BOOLEAN_ARRAY,
		0, 0,
		/* 57 */ PropertyType::UNIT,
		/* 58 */ PropertyType::UNIT_ARRAY,
		/* 59 */ PropertyType::UNIT,
		/* 60 */ PropertyType::UNIT_ARRAY,
		/* 61 */ PropertyType::UNIT
	};

	const int Binary::PropertyArrayTypesToBase[62] = {
		0,
		/* STRING = 1, */ 0,
		/* STRING_ARRAY, */ PropertyType::STRING,
		/* TOKEN, */ 0,
		/* TOKEN_ARRAY, */ PropertyType::TOKEN,
		/* FLOAT, */ 0,
		/* FLOAT_ARRAY, */ PropertyType::FLOAT,
		/* FLOAT_TRIPLE, */ PropertyType::FLOAT,
		/* FLOAT_QUAD, */ PropertyType::FLOAT,
		/* UINT32_TRIPLE, */ PropertyType::UINT32,
		/* INT32_TRIPLE */ PropertyType::INT32,
		/* INT32_TRIPLE_ARRAY, */ PropertyType::INT32_TRIPLE,
		/* FLOAT_QUAD_ARRAY, */ PropertyType::FLOAT_QUAD,
		/* FLOAT_TRIPLE_QUAD, */ 0, // Not a triple of quads, but a triple followed by a quad
		/* INT32, */ 0,
		/* UINT32, */ 0,
		/* UINT32_ARRAY, */ PropertyType::UINT32,
		/* UINT16, */ 0,
		/* UINT16_ARRAY, */ PropertyType::UINT16,
		/* INT64, */ 0,
		/* UINT64, */ 0,
		/* INT64_ARRAY, */ PropertyType::INT64,
		/* BOOLEAN, */ 0,
		/* BOOLEAN_ARRAY, */ PropertyType::BOOLEAN,
		/* UNIT, */ 0,
		/* UNIT_ARRAY */ PropertyType::UNIT
	};

	bool Binary::parse(const std::string& data, const ParseCallback& callback) {
		Binary parser(data, callback);
		return parser.parseSuccessful();
	}

	Binary::Binary(const std::string& data, const ParseCallback& callback) : mData(data), mCallback(callback) {
		mParseSuccessful = parse();
	}

	Binary::~Binary() {
		for (const auto& unitDeclaration : mUnitDeclarations) {
			for (const auto& propertyDeclaration : unitDeclaration.second->properties) {
				delete(propertyDeclaration);
			}
			delete(unitDeclaration.second);
		}
	}

	bool Binary::parseSuccessful() {
		return mParseSuccessful;
	}

	bool Binary::parse() {
		mCursor = &mData[0];
		mEnd = mCursor + mData.length();
		try {
			if (memcmp((char *)getBytes(4), "BSII", 4) != 0) {
				throw(std::runtime_error("Invalid magic."));
			}
			readUnitDeclarations();
			readUnits();

			//throw(ParseError("Success!!"));
			return true;
		} catch (std::exception& e) {
			DEBUG_LOG("%s", e.what());
			return false;
		}
	}

	void * Binary::getBytes(size_t n) {
		if ((mEnd - mCursor) < 0 || (size_t)(mEnd - mCursor) < n) {
			throw(EndOfData());
		}
		void * ret = (void *)mCursor;
		mCursor += n;
		return ret;
	}

	int Binary::getInt() {
		return *(int *)getBytes(sizeof(int));
	}

	void Binary::readUnitDeclarations() {
		std::string name;
		while (true) {
			UnitDeclarationHeader * header = (UnitDeclarationHeader *)getBytes(sizeof(UnitDeclarationHeader));
			if (header->format != 0) {
				// This is not an unit declaration, so we are done; rewind to the format and bail out
				mCursor = (char *)&(header->format);
				return;
			}
			if (header->nameLength > 64) {
				throw(ParseError("unit declaration name too long"));
			}
			name = std::string((char *)getBytes(header->nameLength), header->nameLength);
			UnitDeclaration * declaration = new UnitDeclaration;
			declaration->index = header->index2;
			declaration->name = name;
			mUnitDeclarations[declaration->index] = declaration;

			readPropertyDeclarations(declaration);
		}
	}

	void Binary::readPropertyDeclarations(UnitDeclaration * unit) {
		std::string name;
		while (true) {
			PropertyDeclarationHeader * header = (PropertyDeclarationHeader *)getBytes(sizeof(PropertyDeclarationHeader));
			if (header->type == 0) {
				// This is not a property declaration, so we are done; rewind and bail out
				mCursor = (char *)header;
				return;
			}
			if (header->type >= (sizeof(PropertyTypeByIndex) / sizeof(*PropertyTypeByIndex)) || PropertyTypeByIndex[header->type] == 0) {
				throw(ParseError("property declaration of unknown type"));
			}
			name = std::string((char *)getBytes(header->nameLength), header->nameLength);
			PropertyDeclaration * declaration = new PropertyDeclaration;
			declaration->type = (PropertyType)PropertyTypeByIndex[header->type];
			declaration->name = name;
			unit->properties.push_back(declaration);
		}
	}

	bool Binary::readUnits() {
		std::string name;
		std::string value;
		std::string sourceValue;
		int declarationIndex;
		UnitDeclaration * unitDeclaration;
		while (true) {
			declarationIndex = getInt();
			if (declarationIndex == 0) {
				break; // no more properties for this unit
			}
			try {
				unitDeclaration = mUnitDeclarations.at(declarationIndex);
			} catch (std::out_of_range) {
				throw(ParseError("invalid unit declaration index"));
			}
			sourceValue.clear();
			readUnitName(value);
			PARSER_CALLBACK(Parser::Sii::CONTEXT_UNIT_START, unitDeclaration->name, value, sourceValue, (unsigned long)(mCursor - &mData[0]));
			for (const auto& propertyDeclaration : unitDeclaration->properties) {
				if (!readProperty(propertyDeclaration->type, propertyDeclaration->name, 0, false)) {
					return false;
				}
			}
			PARSER_CALLBACK(Parser::Sii::CONTEXT_UNIT_END, unitDeclaration->name, value, sourceValue, (unsigned long)(mCursor - &mData[0]));
		}
		return true;
	}

	bool Binary::readProperty(PropertyType type, const std::string& propertyName, bool arrayMember, uint32_t arrayIndex) {
		// declare them static to avoid thousands (millions?) of allocations/constructions
		static std::string localName;
		static std::string value;
		static std::string sourceValue;
		const std::string * name;
		if (arrayMember) {
			localName.assign(propertyName);
			localName.append(1, '[');
			localName.append(std::to_string(arrayIndex));
			localName.append(1, ']');
			name = &localName;
		} else {
			name = &propertyName;
		}
		//DEBUG_LOG("Reading value for %s at 0x%x", *name, (unsigned long)(mCursor - &mData[0]));
		switch (type) {
		case PropertyType::UINT16:
		case PropertyType::UINT32:
		case PropertyType::UINT64:
		case PropertyType::INT32:
		case PropertyType::INT64:
		case PropertyType::BOOLEAN:
		case PropertyType::UINT32_TRIPLE:
		case PropertyType::INT32_TRIPLE:
		case PropertyType::TOKEN:
		case PropertyType::UNIT:
			readValue(type, value, nullptr);
			PARSER_CALLBACK(Parser::Sii::CONTEXT_ATTRIBUTE, *name, value, value, (unsigned long)(mCursor - &mData[0]));
			break;
		case PropertyType::STRING:
		case PropertyType::FLOAT:
		case PropertyType::FLOAT_TRIPLE:
		case PropertyType::FLOAT_QUAD:
		case PropertyType::FLOAT_TRIPLE_QUAD:
			if (type == PropertyType::FLOAT_TRIPLE_QUAD)
				DEBUG_LOG("Reading value for %s at 0x%x", *name, (unsigned long)(mCursor - &mData[0]));
			//if (*name == "my_truck_placement")
			//	__debugbreak();
			readValue(type, value, &sourceValue);
			PARSER_CALLBACK(Parser::Sii::CONTEXT_ATTRIBUTE, *name, value, sourceValue, (unsigned long)(mCursor - &mData[0]));
			break;
		case PropertyType::TOKEN_ARRAY:
		case PropertyType::INT32_TRIPLE_ARRAY:
		case PropertyType::UINT32_ARRAY:
		case PropertyType::UINT16_ARRAY:
		case PropertyType::INT64_ARRAY:
		case PropertyType::BOOLEAN_ARRAY:
		case PropertyType::UNIT_ARRAY:
		case PropertyType::STRING_ARRAY:
		case PropertyType::FLOAT_ARRAY:
		case PropertyType::FLOAT_QUAD_ARRAY:
		{
			uint32_t numItems = *(uint32_t *)getBytes(sizeof(uint32_t));
			value = std::to_string(numItems);
			PARSER_CALLBACK(Parser::Sii::CONTEXT_ATTRIBUTE, *name, value, value, (unsigned long)(mCursor - &mData[0]));
			for (uint32_t i = 0; i < numItems; ++i) {
				readProperty((PropertyType)PropertyArrayTypesToBase[type], propertyName, true, i);
			}
		}
		break;
		default:
			throw(ParseError("unknown property type " + std::to_string(type)));
		}
		return true;
	}

	void Binary::readValue(PropertyType type, std::string& dest, std::string* destRaw) {
		bool rawFilled = false;
		switch (type) {
		case PropertyType::UINT16:
		{
			uint16_t value = *(uint16_t *)getBytes(sizeof(uint16_t));
			dest.assign(value == 0xffff ? "nil" : std::to_string(value));
		}
		break;
		case PropertyType::INT32:
		{
			int32_t value = *(int32_t *)getBytes(sizeof(int32_t));
			dest.assign(std::to_string(value));
		}
		break;
		case PropertyType::UINT32:
		{
			uint32_t value = *(uint32_t *)getBytes(sizeof(uint32_t));
			dest.assign(value == 0xffffffff ? "nil" : std::to_string(value));
		}
		break;
		case PropertyType::INT64:
		{
			int64_t value = *(int64_t *)getBytes(sizeof(int64_t));
			dest.assign(std::to_string(value));
		}
		break;
		case PropertyType::UINT64:
		{
			uint64_t value = *(uint64_t *)getBytes(sizeof(uint64_t));
			dest.assign(value == 0xffffffffffffffff ? "nil" : std::to_string(value));
		}
		break;
		case PropertyType::BOOLEAN:
		{
			char value = *(char *)getBytes(1);
			dest.assign(value == 0 ? "false" : "true");
		}
		break;
		case PropertyType::FLOAT:
		{
			float value = *(float *)getBytes(sizeof(float));
			double intPart = 0;
			dest.resize(31);
			_locale_t locale = _create_locale(LC_ALL, "C");
			int size = _snprintf_l(&dest[0], 31, "%f", locale, value);
			if (size > 31) {
				dest.resize(size + 1);
				size = _snprintf_l(&dest[0], 31, "%f", locale, value);
			}
			dest.resize(size);
			_free_locale(locale);
			if (destRaw != nullptr) {
				rawFilled = true;
				if (!std::isnan(value) && !std::isinf(value) && value < 9'000'000.0 && value > -9'000'000.0 && std::modf(value, &intPart) == 0.0) {
					destRaw->assign(std::to_string((int64_t)intPart));
				} else {
					destRaw->resize(9);
					snprintf(&(*destRaw)[0], 10, "&%08x", *(uint32_t *)&value);
				}
			}
		}
		break;
		case PropertyType::STRING:
		{
			uint32_t length = *(uint32_t *)getBytes(sizeof(uint32_t));
			dest.assign((char *)getBytes(length), length);
			char * p = &dest[0];
			char * pe = p + length;
			bool needsQuote = false;
			if (length == 0) {
				needsQuote = true;
			} else {
				for (; p < pe; ++p) {
					if (*p < '0' || (*p > '9' && *p < 'A') || (*p > 'Z' && *p < 'a' && *p != '_') || *p > 'z') {
						needsQuote = true;
						break;
					}
				}
			}
			if (needsQuote) {
				char hex[5];
				destRaw->assign(1, '"');
				for (p = &dest[0]; p < pe; ++p) {
					if (*p == 0) {
						break;
					} else if (*p < 32 || *p > 127) {
						snprintf(hex, 5, "\\x%02x", (unsigned char)*p);
						destRaw->append(hex, 4);
					} else {
						if (*p == '"' || *p == '\\') {
							destRaw->append(1, '\\');
						}
						destRaw->append(1, *p);
					}
				}
				destRaw->append(1, '"');
				rawFilled = true;
			}
		}
		//if (dest.find("GE<offset", 0) == 0)
		//	__debugbreak();
		break;
		case PropertyType::TOKEN:
		{
			uint64_t value = *(uint64_t *)getBytes(sizeof(uint64_t));
			dest.resize(12);
			dest.resize(decodeToken(value, &dest[0]));
			if (dest.empty()) {
				dest.assign(2, '\"');
			}
		}
		break;
		case PropertyType::UNIT:
			readUnitName(dest);
			break;
		case PropertyType::FLOAT_TRIPLE:
		case PropertyType::UINT32_TRIPLE:
		case PropertyType::INT32_TRIPLE:
		case PropertyType::FLOAT_QUAD:
		{
			std::string temp;
			std::string tempRaw;
			dest.assign(1, '(');
			if (destRaw != nullptr) {
				destRaw->assign(1, '(');
				rawFilled = true;
			}
			for (int i = 0; i < (type == PropertyType::FLOAT_QUAD ? 4 : 3); ++i) {
				if (i != 0) {
					dest.append(type == PropertyType::FLOAT_QUAD && i == 1 ? "; " : ", ");
					if (destRaw != nullptr) {
						destRaw->append(type == PropertyType::FLOAT_QUAD && i == 1 ? "; " : ", ");
					}
				}
				readValue((PropertyType)PropertyArrayTypesToBase[type], temp, destRaw == nullptr ? nullptr : &tempRaw);
				dest.append(temp);
				if (destRaw != nullptr) {
					destRaw->append(tempRaw);
				}
			}
			dest.append(1, ')');
			if (destRaw != nullptr) {
				destRaw->append(1, ')');
			}
		}
		break;
		case PropertyType::FLOAT_TRIPLE_QUAD:
		{
			std::string temp;
			std::string tempRaw;
			const char * p = mCursor;
			readValue(PropertyType::FLOAT_TRIPLE, dest, destRaw);
			dest.append(1, ' ');
			if (destRaw != nullptr) {
				destRaw->append(1, ' ');
				rawFilled = true;
			}
			// need to skip 4 bytes
			getBytes(4);
			readValue(PropertyType::FLOAT_QUAD, temp, destRaw == nullptr ? nullptr : &tempRaw);
			dest.append(temp);
			if (destRaw != nullptr) {
				destRaw->append(tempRaw);
				destRaw->append("           ");
				sprintf(&(*destRaw)[destRaw->length() - 10], "%X", p - &mData[0]);
			}
		}
		break;
		default:
			throw(ParseError("don't know how to read property type " + std::to_string(type)));
		}
		if (!rawFilled && destRaw != nullptr) {
			destRaw->assign(dest);
		}
	}

	void Binary::readUnitName(std::string& dest) {
		int numTokens = *(char *)getBytes(1);
		if (numTokens == -1) {
			uint64_t id = *(uint64_t *)getBytes(sizeof(uint64_t));
			dest.assign("_nameless.    .    .    .    ");
			snprintf(&dest[10], 20, "%04X.%04X.%04X.%04X", (uint16_t)(id >> 48), (uint16_t)(id >> 32), (uint16_t)(id >> 16), (uint16_t)id);
		} else if (numTokens < 1) {
			dest.assign("null");
		} else {
			char tokenValue[12];
			dest.clear();
			int numChars;
			for (int i = 0; i < numTokens; i++) {
				if (i != 0) {
					dest.append(1, '.');
				}
				numChars = decodeToken(*(uint64_t *)getBytes(sizeof(uint64_t)), tokenValue);
				dest.append(tokenValue, numChars);
			}
		}
	}

	int Binary::decodeToken(uint64_t token, char dest[12]) {
		char * currentChar = dest;
		while (token > 0) {
			if ((currentChar - dest) >= 12) {
				throw(std::out_of_range("token yields more than 12 characters"));
			}
			*currentChar = token % 38;
			if (*currentChar == 0) {
				throw(ParseError("invalid character in token"));
			}
			if (*currentChar <= 10) {
				*currentChar += '0' - 1;
			} else if (*currentChar <= 36) {
				*currentChar += 'a' - 11;
			} else {
				*currentChar = '_';
			}
			++currentChar;
			token /= 38;
		}
		return currentChar - dest;
	}
}
