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
	wxCheckBox * mCheckScandinavia;
	wxCheckBox * mCheckGoingEast;
	wxCheckBox * mCheckHighPowerCargo;
	wxCheckBox * mCheckFrance;
	wxCheckBox * mCheckHeavyCargo;
	const Ets2::Save * mSave;

	void setCheckboxDlcPresent(wxCheckBox * checkbox, bool present);
	void onSelectAll();
};