#pragma once

#include "Profile.hpp"
#include "lib/file.hpp"
#include <string>
#include <regex>

namespace Ets2 {
	// Information from a ETS2 Settings directory
	class Info {
	public:
		class SaveFormatChangeFailed : public std::exception {
			const char * what() const throw() { return "Save Format change failed."; }
		};

		// Returns the default ETS2 Settings directory for the current user
		// i.e. the current user's Documents\\Euro Truck Simulator 2
		static std::wstring getDefaultDirectory();
		enum SaveFormat {
			SAVE_FORMAT_BINARY = 0,
			SAVE_FORMAT_TEXT = 2,
			SAVE_FORMAT_BOTH = 3,
			SAVE_FORMAT_INVALID = -1,
			SAVE_FORMAT_NOT_FOUND = -2
		};

		Info(const std::wstring directory = getDefaultDirectory());
		Info(Info& info);

		bool isValid();
		std::wstring getDirectory();
		std::wstring getConfigFileName();
		SaveFormat getSaveFormat();
		void changeSaveFormat(SaveFormat newFormat);
		std::wstring getRawSaveFormat();
		const ProfileList& getProfiles() const;

	private:
		static const std::string SAVE_FORMAT_VAR_NAME;

		bool mIsValid;
		std::wstring mDirectory;
		std::wstring mConfigFileName;
		ProfileList mProfiles;
		std::wstring mRawSaveFormat;
		SaveFormat mSaveFormat;

		// If called with SAVE_FORMAT_INVALID, reads the save format and
		// sets mSaveFormat and mSaveFormatLine, otherwise changes the
		// save format in the file.
		void processSaveFormat(SaveFormat newFormat = SAVE_FORMAT_INVALID);
	};
}