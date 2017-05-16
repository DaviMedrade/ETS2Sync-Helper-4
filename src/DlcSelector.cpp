#include "precomp.hpp"
#include "DlcSelector.hpp"

DlcSelector::DlcSelector(wxWindow * parent, wxWindowID id)
	: Ets2StaticBox(parent, id, "DLCs") {
	wxBoxSizer * contentSizer = getContentSizer();


	wxBoxSizer * controlsSizer = new wxBoxSizer(wxHORIZONTAL);
	contentSizer->Add(controlsSizer, wxSizerFlags().Expand());
	controlsSizer->Add(new wxStaticText(this, wxID_ANY, "Also include jobs that require:"));
	controlsSizer->AddStretchSpacer();
	mSelectAll = new wxHyperlinkCtrl(this, wxID_ANY, "Select all", wxEmptyString);
	mSelectAll->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent&) { onSelectAll(); });
	controlsSizer->Add(mSelectAll);

	contentSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 7)).y);

	wxBoxSizer * parentSizer = new wxBoxSizer(wxHORIZONTAL);
	contentSizer->Add(parentSizer, wxSizerFlags().Expand());

	controlsSizer = new wxBoxSizer(wxVERTICAL);
	parentSizer->Add(controlsSizer, wxSizerFlags().Proportion(2));

	mCheckScandinavia = new wxCheckBox(this, wxID_ANY, "Scandinavia");
	controlsSizer->Add(mCheckScandinavia, wxSizerFlags().Proportion(1));

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

	mCheckGoingEast = new wxCheckBox(this, wxID_ANY, "Going East!");
	controlsSizer->Add(mCheckGoingEast, wxSizerFlags().Proportion(1));

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

	mCheckFrance = new wxCheckBox(this, wxID_ANY, "Vive la France!");
	controlsSizer->Add(mCheckFrance, wxSizerFlags().Proportion(1));

	parentSizer->AddStretchSpacer(1);

	controlsSizer = new wxBoxSizer(wxVERTICAL);
	parentSizer->Add(controlsSizer, wxSizerFlags().Proportion(2));

	mCheckHighPowerCargo = new wxCheckBox(this, wxID_ANY, "High Power Cargo Pack");
	controlsSizer->Add(mCheckHighPowerCargo, wxSizerFlags().Proportion(1));

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

	mCheckHeavyCargo = new wxCheckBox(this, wxID_ANY, "Heavy Cargo Pack");
	controlsSizer->Add(mCheckHeavyCargo, wxSizerFlags().Proportion(1));

	parentSizer->AddStretchSpacer(1);
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
	setCheckboxDlcPresent(mCheckHeavyCargo, (dlcs & Ets2::Save::DLC_HEAVYCARGO) != 0);
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
	if (mCheckHeavyCargo->IsChecked()) {
		dlcs |= Ets2::Save::DLC_HEAVYCARGO;
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

void DlcSelector::onSelectAll() {
	bool clear = false;
	if ((!mCheckScandinavia->IsEnabled() || mCheckScandinavia->IsChecked())
		&& (!mCheckGoingEast->IsEnabled() || mCheckGoingEast->IsChecked())
		&& (!mCheckHighPowerCargo->IsEnabled() || mCheckHighPowerCargo->IsChecked())
		&& (!mCheckFrance->IsEnabled() || mCheckFrance->IsChecked())
		&& (!mCheckHeavyCargo->IsEnabled() || mCheckHeavyCargo->IsChecked())) {
		clear = true;
	}

	if (mCheckScandinavia->IsEnabled()) {
		mCheckScandinavia->SetValue(!clear);
	}
	if (mCheckGoingEast->IsEnabled()) {
		mCheckGoingEast->SetValue(!clear);
	}
	if (mCheckHighPowerCargo->IsEnabled()) {
		mCheckHighPowerCargo->SetValue(!clear);
	}
	if (mCheckFrance->IsEnabled()) {
		mCheckFrance->SetValue(!clear);
	}
	if (mCheckHeavyCargo->IsEnabled()) {
		mCheckHeavyCargo->SetValue(!clear);
	}
}