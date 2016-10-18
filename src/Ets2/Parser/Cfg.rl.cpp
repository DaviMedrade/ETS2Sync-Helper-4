#include "precomp.hpp"
#include "Cfg.hpp"

#include "../lib/Utf8ToUtf16.hpp"

#define RAGEL(...) /* __VA_ARGS__ */

namespace Ets2::Parser {
	RAGEL(%%{
		machine cfgParser;

		include "Cfg.rl";
		main := line;
	}%%)

	%% write data;

	void Cfg::parse(const std::string& data, CALLBACK_FUNC callback) {
		const char * p;
		const char * pe;
		const char * eof;
		int cs;

		const char * bufferStart = data.data();
		const char * bufferEnd = bufferStart + data.length();
		const char * lineStart = bufferStart;
		const char * lineEnd = nullptr;
		const char * varNameStart = nullptr;
		const char * varValueRawStart = nullptr;
		std::string line;
		std::string varName;
		std::string varValue;
		std::string empty;

		while (lineStart < bufferEnd) {
			lineEnd = lineStart;
			while (lineEnd < bufferEnd && *(lineEnd) != '\r' && *(lineEnd) != '\n') {
				++lineEnd;
			}

			line.assign(lineStart, lineEnd - lineStart);
			p = lineStart;
			pe = lineEnd;
			eof = pe;
			varName.clear();
			varValue.clear();

			%% write init;
			%% write exec;

			if (cs >= cfgParser_first_final) {
				callback(line, varName, varValue);
			} else {
				callback(line, empty, empty);
			}

			lineStart = lineEnd;
			if (lineStart < bufferEnd && *lineStart == '\r') {
				++lineStart;
			}
			if (lineStart < bufferEnd && *lineStart == '\n') {
				++lineStart;
			}
		}
	}
}