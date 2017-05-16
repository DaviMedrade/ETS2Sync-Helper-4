#pragma once

class StatusText : public wxStaticText {
public:
	StatusText(wxWindow * wwParent, wxWindowID id);
	enum class Type {
		STATUS,
		SUCCESS,
		WARNING,
		FAILURE
	};
	Type getType() const;
	void SetLabel(const wxString& wsLabel, Type newType);

private:
	Type mType;
};