#pragma once

class StatusText : public wxStaticText {
public:
	StatusText(wxWindow * wwParent, wxWindowID id);
	enum Type {
		TYPE_STATUS,
		TYPE_SUCCESS,
		TYPE_WARNING,
		TYPE_ERROR
	};
	Type getType() const;
	void SetLabel(const wxString& wsLabel, Type newType);

private:
	Type mType;
};