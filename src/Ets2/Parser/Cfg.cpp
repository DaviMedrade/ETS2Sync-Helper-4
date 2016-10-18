
/* #line 1 "src\\Ets2\\Parser\\Cfg.rl.cpp" */
#include "precomp.hpp"
#include "Cfg.hpp"

#include "../lib/Utf8ToUtf16.hpp"

#define RAGEL(...) /* __VA_ARGS__ */

namespace Ets2::Parser {
	RAGEL(
/* #line 14 "src\\Ets2\\Parser\\Cfg.rl.cpp" */
)

	
/* #line 17 "src\\Ets2\\Parser\\Cfg.cpp" */
static const char _cfgParser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 2, 4, 2, 
	2, 5, 2
};

static const int cfgParser_start = 1;
static const int cfgParser_first_final = 16;
static const int cfgParser_error = 0;

static const int cfgParser_en_main = 1;


/* #line 17 "src\\Ets2\\Parser\\Cfg.rl.cpp" */

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

			
/* #line 64 "src\\Ets2\\Parser\\Cfg.cpp" */
	{
	cs = cfgParser_start;
	}

/* #line 49 "src\\Ets2\\Parser\\Cfg.rl.cpp" */
			
/* #line 71 "src\\Ets2\\Parser\\Cfg.cpp" */
	{
	const char *_acts;
	unsigned int _nacts;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	switch ( cs ) {
case 1:
	switch( (*p) ) {
		case 9: goto tr0;
		case 32: goto tr0;
		case 117: goto tr2;
	}
	goto tr1;
case 0:
	goto _out;
case 2:
	if ( (*p) == 115 )
		goto tr3;
	goto tr1;
case 3:
	if ( (*p) == 101 )
		goto tr4;
	goto tr1;
case 4:
	if ( (*p) == 116 )
		goto tr5;
	goto tr1;
case 5:
	switch( (*p) ) {
		case 9: goto tr6;
		case 32: goto tr6;
	}
	goto tr1;
case 6:
	switch( (*p) ) {
		case 9: goto tr6;
		case 32: goto tr6;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr7;
	} else if ( (*p) >= 65 )
		goto tr7;
	goto tr1;
case 7:
	switch( (*p) ) {
		case 9: goto tr8;
		case 32: goto tr8;
		case 95: goto tr9;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr9;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr9;
	} else
		goto tr9;
	goto tr1;
case 8:
	switch( (*p) ) {
		case 9: goto tr10;
		case 32: goto tr10;
		case 34: goto tr11;
	}
	goto tr1;
case 9:
	switch( (*p) ) {
		case 34: goto tr13;
		case 92: goto tr14;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr12;
	goto tr1;
case 10:
	switch( (*p) ) {
		case 34: goto tr16;
		case 92: goto tr17;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr15;
	goto tr1;
case 16:
	switch( (*p) ) {
		case 9: goto tr13;
		case 32: goto tr13;
		case 35: goto tr28;
	}
	goto tr1;
case 17:
	goto tr28;
case 11:
	if ( (*p) == 120 )
		goto tr19;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr18;
	goto tr1;
case 12:
	switch( (*p) ) {
		case 34: goto tr21;
		case 92: goto tr22;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr20;
	goto tr1;
case 13:
	switch( (*p) ) {
		case 34: goto tr21;
		case 92: goto tr22;
	}
	if ( (*p) < 65 ) {
		if ( (*p) < 48 ) {
			if ( 32 <= (*p) && (*p) <= 47 )
				goto tr20;
		} else if ( (*p) > 57 ) {
			if ( 58 <= (*p) && (*p) <= 64 )
				goto tr20;
		} else
			goto tr23;
	} else if ( (*p) > 70 ) {
		if ( (*p) < 97 ) {
			if ( 71 <= (*p) && (*p) <= 96 )
				goto tr20;
		} else if ( (*p) > 102 ) {
			if ( 103 <= (*p) && (*p) <= 126 )
				goto tr20;
		} else
			goto tr23;
	} else
		goto tr23;
	goto tr1;
case 14:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr24;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr24;
	} else
		goto tr24;
	goto tr1;
case 15:
	switch( (*p) ) {
		case 34: goto tr26;
		case 92: goto tr27;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr25;
	goto tr1;
	}

	tr1: cs = 0; goto _again;
	tr0: cs = 1; goto _again;
	tr2: cs = 2; goto _again;
	tr3: cs = 3; goto _again;
	tr4: cs = 4; goto _again;
	tr5: cs = 5; goto _again;
	tr6: cs = 6; goto _again;
	tr9: cs = 7; goto _again;
	tr7: cs = 7; goto f0;
	tr10: cs = 8; goto _again;
	tr8: cs = 8; goto f1;
	tr11: cs = 9; goto _again;
	tr15: cs = 10; goto _again;
	tr12: cs = 10; goto f2;
	tr20: cs = 10; goto f4;
	tr25: cs = 10; goto f6;
	tr14: cs = 11; goto _again;
	tr17: cs = 11; goto f3;
	tr22: cs = 11; goto f5;
	tr27: cs = 11; goto f7;
	tr18: cs = 12; goto _again;
	tr19: cs = 13; goto _again;
	tr23: cs = 14; goto _again;
	tr24: cs = 15; goto _again;
	tr13: cs = 16; goto _again;
	tr16: cs = 16; goto f3;
	tr21: cs = 16; goto f5;
	tr26: cs = 16; goto f7;
	tr28: cs = 17; goto _again;

	f0: _acts = _cfgParser_actions + 1; goto execFuncs;
	f1: _acts = _cfgParser_actions + 3; goto execFuncs;
	f2: _acts = _cfgParser_actions + 5; goto execFuncs;
	f3: _acts = _cfgParser_actions + 7; goto execFuncs;
	f5: _acts = _cfgParser_actions + 9; goto execFuncs;
	f7: _acts = _cfgParser_actions + 11; goto execFuncs;
	f4: _acts = _cfgParser_actions + 13; goto execFuncs;
	f6: _acts = _cfgParser_actions + 16; goto execFuncs;

execFuncs:
	_nacts = *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
/* #line 4 "src\\Ets2\\Parser\\Cfg.rl" */
	{
		varNameStart = p;
	}
	break;
	case 1:
/* #line 8 "src\\Ets2\\Parser\\Cfg.rl" */
	{
		varName.assign(varNameStart, p - varNameStart);
	}
	break;
	case 2:
/* #line 12 "src\\Ets2\\Parser\\Cfg.rl" */
	{
		varValueRawStart = p;
		// wxLogDebug("valueRawStart");
	}
	break;
	case 3:
/* #line 17 "src\\Ets2\\Parser\\Cfg.rl" */
	{
		varValue.append(varValueRawStart, p - varValueRawStart);
		// wxLogDebug("valueRawEnd (%d chars)", p - varValueRawStart);
	}
	break;
	case 4:
/* #line 22 "src\\Ets2\\Parser\\Cfg.rl" */
	{
		varValue.push_back(*(p-1));
	}
	break;
	case 5:
/* #line 26 "src\\Ets2\\Parser\\Cfg.rl" */
	{
		varValue.push_back(((p[-2] & 0x40 ? 9 : 0) + (p[-2] & 0x0F)) << 4 | ((p[-1] & 0x40 ? 9 : 0) + (p[-1] & 0x0F)));
	}
	break;
/* #line 308 "src\\Ets2\\Parser\\Cfg.cpp" */
		}
	}
	goto _again;

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

/* #line 50 "src\\Ets2\\Parser\\Cfg.rl.cpp" */

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