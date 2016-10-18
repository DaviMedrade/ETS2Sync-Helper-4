%%{
	machine siiParserText;

	action unitClassStart {
		unitClassStart = p;
	}

	action unitClassEnd {
		unitClass.assign(unitClassStart, p - unitClassStart);
	}

	action unitInstanceStart {
		unitInstanceStart = p;
	}

	action unitInstanceEnd {
		unitInstance.assign(unitInstanceStart, p - unitInstanceStart);
	}

	action unitStart {
		attrSourceValue.clear();
		PARSER_CALLBACK(CONTEXT_UNIT_START, unitClass, unitInstance, attrSourceValue, p - start);
	}

	action unitEnd {
		attrSourceValue.clear();
		PARSER_CALLBACK(CONTEXT_UNIT_END, unitClass, unitInstance, attrSourceValue, p - start);
	}

	action attrNameStart {
		attrNameStart = p;
	}

	action attrNameEnd {
		attrName.assign(attrNameStart, p - attrNameStart);
		//wxLogDebug("Attr Name: '%s'", attrName);
	}

	action attrValueStart {
		attrRawValue.clear();
		attrSourceValueStart = p;
	}

	action attrValueEnd {
		attrSourceValue.assign(attrSourceValueStart, p - attrSourceValueStart);
		PARSER_CALLBACK(CONTEXT_ATTRIBUTE, attrName, attrRawValue, attrSourceValue, p - start);
		//wxLogDebug("Attr Value: '%s' (state %d)", attrRawValue, cs);
	}

	action attrBareValueStart {
		//wxLogDebug("attrBareValueStart");
		attrBareValueStart = p;
	}

	action attrBareValueEnd {
		attrRawValue.assign(attrBareValueStart, p - attrBareValueStart);
		//wxLogDebug("attrBareValueEnd: %s", attrValue);
	}

	action attrQuotedValueRawStart {
		//wxLogDebug("QuotedValueRawStart: %p", p);
		attrQuotedValueRawStart = p;
	}

	action attrQuotedValueRawEnd {
		//wxLogDebug("QuotedValueRawEnd: %p '%s'", p, std::string(attrQuotedValueRawStart, p - attrQuotedValueRawStart));
		attrRawValue.append(attrQuotedValueRawStart, p - attrQuotedValueRawStart);
	}

	action attrQuotedValueEscapedChar {
		attrRawValue.push_back(*(p-1));
	}

	action attrQuotedValueEscapedHex {
		attrRawValue.push_back(((p[-2] & 0x40 ? 9 : 0) + (p[-2] & 0x0F)) << 4 | ((p[-1] & 0x40 ? 9 : 0) + (p[-1] & 0x0F)));
	}

	action success {
		success = true;
	}

	h_space = [ \t];
	identifier = (alpha | digit | '_')+;

	array_index = '[' . digit+ . ']';
	attribute_name = (identifier . array_index?) >attrNameStart %attrNameEnd;
	escape_hex = 'x' . xdigit{2};
	escape_seq = '\\' . (escape_hex %attrQuotedValueEscapedHex | [\\"] %attrQuotedValueEscapedChar);
	quoted_raw_chars = (print - [\\"])+ >attrQuotedValueRawStart %attrQuotedValueRawEnd;
	quoted_value = '"' . ('' | (quoted_raw_chars | escape_seq)**) . '"';
	bare_value = (print - ["])+ >attrBareValueStart %attrBareValueEnd;
	value = ((quoted_value . h_space*) | bare_value) >attrValueStart %attrValueEnd;

	attribute = (attribute_name . ':' . h_space+ . value . [\r\n]);

	unit_class = identifier >unitClassStart %unitClassEnd;
	unit_instance_name = (identifier . ('.' identifier)*) >unitInstanceStart %unitInstanceEnd;
	unit_head = (unit_class . h_space+ . ':' . h_space+ . unit_instance_name . h_space+ . '{') %unitStart;
	unit = unit_head . (space* . attribute)* . space* . '}' %unitEnd;

	wrapper = "SiiNunit" . space+ . '{' . (space* . unit . space*)+ . '}' %success;

	file = wrapper . space*;
}%%