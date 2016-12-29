#pragma once

#include "Ets2StaticBox.hpp"

class DlcSelector : public Ets2StaticBox {
public:
	DlcSelector(wxWindow * parent, wxWindowID);

	void setSave(const Ets2::Save * save);
	int getDlcs();

private:
	wxCheckBox * mCheckScandinavia;
	wxCheckBox * mCheckGoingEast;
	wxCheckBox * mCheckHighPowerCargo;
	wxCheckBox * mCheckFrance;
	const Ets2::Save * mSave;

	void setCheckboxDlcPresent(wxCheckBox * checkbox, bool present);
};