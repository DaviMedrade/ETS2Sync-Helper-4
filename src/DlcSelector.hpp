#pragma once

#include "Ets2StaticBox.hpp"

#include <wx/hyperlink.h>

class DlcSelector : public Ets2StaticBox {
public:
	DlcSelector(wxWindow * parent, wxWindowID);

	void setSave(const Ets2::Save * save);
	int getDlcs();

private:
	wxHyperlinkCtrl * mSelectAll;

	std::map<Ets2::Game, std::vector<wxCheckBox *>> mCheckboxesByGame;
	std::map<wxCheckBox *, int> mDlcByCheckbox;
	const Ets2::Save * mSave;

	void onSelectAll();
};