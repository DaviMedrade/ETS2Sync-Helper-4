#include "precomp.hpp"
#include "Object.hpp"

#include "lib/Utf8ToUtf16.hpp"
#include "File.hpp"

#include <wx/filename.h>
#include <regex>
#include <sstream>

namespace Ets2 {
	Object::Object(const std::wstring& siiFileBasename) : mSiiFileBasename(siiFileBasename) {
		mIsValid = false;
	}

	void Object::init(const std::wstring& directory) {
		mIsValid = false;
		mName.clear();
		mSaveTime = 0;
		mDirectory = directory;

		std::string data;
		if (!File::read(directory + L"\\" + mSiiFileBasename, data)) {
			return;
		}

		if (!File::parse(data, [this](Parser::Sii::Context context, const std::string& name, const std::string& value, const std::string& /* sourceValue */, unsigned long /* offset */) -> bool {
			processAttribute(context, name, value);
			return true;
		})) {
			return;
		}

		if (mSaveTime == 0) {
			return;
		}

		mIsValid = validate();
	}

	bool Object::isValid() {
		return mIsValid;
	}

	std::wstring Object::getDirectory() const {
		return mDirectory;
	}

	Game Object::getGame() const {
		return mGame;
	}

	std::wstring Object::getName() const {
		return mName;
	}

	time_t Object::getSaveTime() const {
		return mSaveTime;
	}

	const std::wstring Object::getSaveTimeCaption() const {
		wxDateTime now = wxDateTime::Now();
		wxDateTime saved = wxDateTime(getSaveTime());
		wxTimeSpan age = now - saved;
		if (age.GetMinutes() == 0) {
			return wxString::Format(age.GetSeconds() == 1 ? L"%lld second ago" : L"%lld seconds ago", age.GetSeconds()).ToStdWstring();
		} else if (age.GetHours() == 0) {
			return wxString::Format(age.GetMinutes() == 1 ? L"%d minute ago" : L"%d minutes ago", age.GetMinutes()).ToStdWstring();
		}
		return saved.Format().ToStdWstring();
	}
}