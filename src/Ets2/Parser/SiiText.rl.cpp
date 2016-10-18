#include "precomp.hpp"
#include "SiiText.hpp"

#include "../lib/Utf8ToUtf16.hpp"

#define RAGEL(...) /* __VA_ARGS__ */

#define PARSER_CALLBACK(...) if (!callback(__VA_ARGS__)) return false;

namespace Ets2::Parser::Sii {
	RAGEL(%%{
		machine siiParserText;

		include "SiiText.rl";
		main := file;
	}%%)

	%% write data;

	bool Text::parse(const std::string& data, const ParseCallback& callback) {
		const char * p;
		const char * pe;
		const char * eof;
		int cs;
		const char * start = data.data();

		%% write init;

		p = start;
		pe = start + data.length();
		eof = pe;

		std::string unitClass;
		const char * unitClassStart = nullptr;
		std::string unitInstance;
		const char * unitInstanceStart = nullptr;
		std::string attrName;
		const char * attrNameStart = nullptr;
		std::string attrRawValue;
		const char * attrBareValueStart = nullptr;
		const char * attrQuotedValueRawStart = nullptr;
		std::string attrSourceValue;
		const char * attrSourceValueStart = nullptr;

		bool success = false;

		%% write exec;

		//wxLogDebug("Parse %s at state %d", success ? "succeeded" : "failed", cs);
		return success;
	}
}
