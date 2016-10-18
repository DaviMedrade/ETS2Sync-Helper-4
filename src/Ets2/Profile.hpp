#pragma once

#include "Object.hpp"
#include "Save.hpp"

#include <memory>

namespace Ets2 {
	class Profile : public Object {
	public:
		Profile(const std::wstring directory);

		const SaveList& getSaves() const;

	private:
		static const std::wstring SII_BASENAME;
		static const std::string NAME_ATTRIBUTE;
		static const std::string SAVE_TIME_ATTRIBUTE;
		static const std::string MAP_ATTRIBUTE;
		static const std::string MAP_VALUE;

		SaveList mSaves;
		bool mMapValid;

		virtual void processAttribute(Parser::Sii::Context context, const std::string& name, const std::string& value) override;
		virtual bool validate() override;
	};

	typedef ObjectList<Profile> ProfileList;
}