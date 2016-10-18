#pragma once

#include "Parser/Sii.hpp"

#include <string>
#include <memory>

namespace Ets2 {
	class Object {
	public:
		Object(const std::wstring& siiFileBasename);

		// This is used to load and process the file passed as siiFileBasename to the constructor from the directory.
		// Should be called from the constructor of a derived class.
		void init(const std::wstring& directory);

		enum ATTRIBUTE_PROCESS_RESULT {
			PROCESS_RESULT_INCOMPLETE, // Keep processing
			PROCESS_RESULT_COMPLETE, // All wanted attributes have been found; stop.
			PROCESS_RESULT_INVALID // The attribute makes this object invalid.
		};

		// For ObjectList::sort
		static bool compareForSort(const Object * left, const Object * right) {
			return left->mSaveTime > right->mSaveTime;
		}

		bool isValid();
		std::wstring getDirectory() const;
		std::wstring getName() const;
		time_t getSaveTime() const;
		const std::wstring getSaveTimeCaption() const;

	protected:
		std::wstring mDirectory;
		std::wstring mName;
		time_t mSaveTime;

		// This function is called during init() for every attribute/value pair read from the file.
		// It should set mName (else the object will be named "[no name]") and
		// it must set mSaveTime (else the object will be considered invalid)
		virtual void processAttribute(Parser::Sii::Context context, const std::string& name, const std::string& value) = 0;

		// This function is called after all attributes have been sent to processAttribute.
		// Its return value determines whether the object is valid (true) or not (false).
		// Note that an object that fails parsing or has no save time is always invalid.
		virtual bool validate() = 0;

	private:
		const std::wstring mSiiFileBasename;
		const std::wstring mNameAttribute;
		const std::wstring mSaveTimeAttribute;

		bool mIsValid;
	};
}
