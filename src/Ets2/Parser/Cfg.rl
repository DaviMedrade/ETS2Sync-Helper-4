%%{
	machine cfgParser;

	action nameStart {
		varNameStart = p;
	}

	action nameEnd {
		varName.assign(varNameStart, p - varNameStart);
	}

	action valueRawStart {
		varValueRawStart = p;
		// wxLogDebug("valueRawStart");
	}

	action valueRawEnd {
		varValue.append(varValueRawStart, p - varValueRawStart);
		// wxLogDebug("valueRawEnd (%d chars)", p - varValueRawStart);
	}

	action valueEscapedChar {
		varValue.push_back(*(p-1));
	}

	action valueEscapedHex {
		varValue.push_back(((p[-2] & 0x40 ? 9 : 0) + (p[-2] & 0x0F)) << 4 | ((p[-1] & 0x40 ? 9 : 0) + (p[-1] & 0x0F)));
	}

	text_char = print;
	comment = '#' . any*;
	whitespace = [ \t];
	varname = (alpha . (alpha | digit | '_')*) >nameStart %nameEnd;
	escape_hex = 'x' . xdigit{2};
	escape_seq = '\\' . (escape_hex %valueEscapedHex | text_char %valueEscapedChar);
	value_raw_chars = (text_char - [\\"])+ >valueRawStart %valueRawEnd;
	varvalue = ('"' . ('' | ( escape_seq | value_raw_chars)**) . '"');
	uset = ("uset" . whitespace+ . varname . whitespace+ . varvalue . whitespace* . comment?);
	line = whitespace* . uset;
}%%