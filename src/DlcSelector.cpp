#include "precomp.hpp"
#include "DlcSelector.hpp"

DlcSelector::DlcSelector(wxWindow * parent, wxWindowID id)
	: Ets2StaticBox(parent, id, "DLCs") {
	wxBoxSizer * contentSizer = getContentSizer();
	contentSizer->Add(new wxStaticText(this, wxID_ANY, "Also include jobs that require:"));

	contentSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 7)).y);

	wxBoxSizer * controlsSizer = new wxBoxSizer(wxHORIZONTAL);
	contentSizer->Add(controlsSizer, wxSizerFlags().Expand());

	mCheckScandinavia = new wxCheckBox(this, wxID_ANY, "Scandinavia");
	controlsSizer->Add(mCheckScandinavia, wxSizerFlags().Proportion(1));

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

	mCheckGoingEast = new wxCheckBox(this, wxID_ANY, "Going East!");
	controlsSizer->Add(mCheckGoingEast, wxSizerFlags().Proportion(1));

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

	mCheckFrance = new wxCheckBox(this, wxID_ANY, "Vive la France!");
	controlsSizer->Add(mCheckFrance, wxSizerFlags().Proportion(1));

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

	mCheckHighPowerCargo = new wxCheckBox(this, wxID_ANY, "High Power Cargo Pack");
	controlsSizer->Add(mCheckHighPowerCargo, wxSizerFlags().Proportion(1));
}

void DlcSelector::setSave(const Ets2::Save * save) {
	mSave = save;
	int dlcs = 0;
	if (mSave == nullptr) {
		return;
	}
	dlcs = mSave->getDlcs();
	setCheckboxDlcPresent(mCheckScandinavia, (dlcs & Ets2::Save::DLC_SCANDINAVIA) != 0);
	setCheckboxDlcPresent(mCheckGoingEast, (dlcs & Ets2::Save::DLC_GOINGEAST) != 0);
	setCheckboxDlcPresent(mCheckHighPowerCargo, (dlcs & Ets2::Save::DLC_HIGHPOWERCARGO) != 0);
	setCheckboxDlcPresent(mCheckFrance, (dlcs & Ets2::Save::DLC_FRANCE) != 0);
}

int DlcSelector::getDlcs() {
	int dlcs = 0;
	if (mCheckScandinavia->IsChecked()) {
		dlcs |= Ets2::Save::DLC_SCANDINAVIA;
	}
	if (mCheckGoingEast->IsChecked()) {
		dlcs |= Ets2::Save::DLC_GOINGEAST;
	}
	if (mCheckHighPowerCargo->IsChecked()) {
		dlcs |= Ets2::Save::DLC_HIGHPOWERCARGO;
	}
	if (mCheckFrance->IsChecked()) {
		dlcs |= Ets2::Save::DLC_FRANCE;
	}
	DEBUG_LOG("Selected DLCs: 0x%x", dlcs);
	return dlcs;
}

void DlcSelector::setCheckboxDlcPresent(wxCheckBox * checkbox, bool present) {
	if (present) {
		checkbox->Enable();
	} else {
		checkbox->SetValue(false);
		checkbox->Disable();
	}
}
