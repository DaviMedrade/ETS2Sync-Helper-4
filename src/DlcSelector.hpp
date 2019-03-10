#pragma once

#include "Ets2StaticBox.hpp"

#include <wx/hyperlink.h>

class DlcSelector : public Ets2StaticBox {
public:
	DlcSelector(wxWindow * parent, wxWindowID);

	void setSave(const Ets2::Save * save);
	const Ets2::Save::DlcList getDlcs() const;

private:
	wxHyperlinkCtrl * mSelectAll;

	std::map<Ets2::Game, std::vector<wxCheckBox *>> mCheckboxesByGame;
	std::map<wxCheckBox *, std::wstring> mDlcByCheckbox;
	const Ets2::Save * mSave;

	void onSelectAll();
};