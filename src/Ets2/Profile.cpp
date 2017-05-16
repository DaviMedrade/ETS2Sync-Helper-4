#include "precomp.hpp"
#include "Profile.hpp"
#include "Info.hpp"
#include "lib/Utf8ToUtf16.hpp"

#include <wx/dir.h>

namespace Ets2 {
	const std::wstring Profile::SII_BASENAME = L"profile.sii";
	const std::string Profile::NAME_ATTRIBUTE = "profile_name";
	const std::string Profile::SAVE_TIME_ATTRIBUTE = "save_time";
	const std::string Profile::MAP_ATTRIBUTE = "map_path";
	const std::string Profile::MAP_VALUE_ETS2 = "/map/europe.mbd";
	const std::string Profile::MAP_VALUE_ATS = "/map/usa.mbd";

	Profile::Profile(const std::wstring directory)
		: Object(SII_BASENAME) {
		//wxStopWatch initTime;
		mMapValid = false;
		init(directory);

		mSaves = SaveList();

		wxDir savesDir(mDirectory + L"\\save\\");
		if (!savesDir.IsOpened()) {
			return;
		}

		wxArrayString saveDirs;
		wxString saveDir;
		bool fileFound = savesDir.GetFirst(&saveDir, L"*", wxDIR_DIRS);
		while (fileFound) {
			if (saveDir.find("autosave") == std::string::npos) { // skip autosaves
				Save * save = new Save((savesDir.GetNameWithSep() + saveDir).ToStdWstring());
				if (save->isValid()) {
					save->setGame(mGame);
					mSaves.push_back(save);
				} else {
					delete save;
				}
			}
			fileFound = savesDir.GetNext(&saveDir);
		}
		mSaves.sort();
		//DEBUG_LOG(L"%s: Initialized in %lld µs", mName, initTime.TimeInMicro());
	};

	const SaveList& Profile::getSaves() const {
		return mSaves;
	}

	void Profile::processAttribute(Parser::Sii::Context context, const std::string& attribute, const std::string& value) {
		if (context != Parser::Sii::Context::ATTRIBUTE) {
			return;
		}
		if (attribute == MAP_ATTRIBUTE) {
			if (value == MAP_VALUE_ETS2) {
				mMapValid = true;
				mGame = Game::ETS2;
			} else if (value == MAP_VALUE_ATS) {
				mMapValid = true;
				mGame = Game::ATS;
			}
		}
		if (mName.empty() && attribute == NAME_ATTRIBUTE) {
			Utf8ToUtf16(value.data(), value.length(), mName);
		} else if (mSaveTime == 0 && attribute == SAVE_TIME_ATTRIBUTE) {
			try {
				mSaveTime = std::stoll(value);
			} catch (std::invalid_argument) {
				mSaveTime = 0;
			}
		}
	}

	bool Profile::validate() {
		return mMapValid;
	}
}