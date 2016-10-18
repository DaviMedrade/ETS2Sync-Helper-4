#include "precomp.hpp"
#include "StatusText.hpp"

StatusText::StatusText(wxWindow * wwParent, wxWindowID id)
	: wxStaticText(wwParent, id, wxEmptyString) {

	// set font and color in case the default is different
	mType = TYPE_SUCCESS;
	SetLabel(wxEmptyString, TYPE_STATUS); 
}

StatusText::Type StatusText::getType() const {
	return mType;
}

void StatusText::SetLabel(const wxString& wsLabel, Type newType) {
	if (newType != mType) {
		mType = newType;
		wxFont font = GetFont().GetBaseFont();
		if (mType != TYPE_STATUS)
			font = font.MakeBold();
		SetFont(font);

		wxColor color;
		switch (mType) {
		case TYPE_STATUS:
			color = wxColor(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
			break;
		case TYPE_SUCCESS:
			color = wxColor(0x008800);
			break;
		case TYPE_WARNING:
			color = wxColor(0x22aadd);
			break;
		case TYPE_ERROR:
			color = wxColor(0x0000ff);
			break;
		}
		SetForegroundColour(color);
	}
	wxStaticText::SetLabel(wsLabel);
}
