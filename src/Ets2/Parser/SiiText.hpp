#pragma once
#include "Sii.hpp"

#include <string>
#include <vector>
#include <functional>

namespace Ets2::Parser::Sii {
	class Text {
	public:
		static bool parse(const std::string& data, const ParseCallback& callback);
	};
}
