#pragma once

#include <string>
#include <vector>
#include <functional>

namespace Ets2::Parser::Sii {
	enum class Context {
		UNIT_START,
		ATTRIBUTE,
		UNIT_END
	};

	typedef std::function<bool(Context context, const std::string& name,
		const std::string& value, const std::string& sourceValue, unsigned long offset)> ParseCallback;
}

#include "SiiText.hpp"
#include "SiiBinary.hpp"