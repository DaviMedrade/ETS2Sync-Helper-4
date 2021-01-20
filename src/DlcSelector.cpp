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

	wxCheckBox * cb;
	std::vector<wxCheckBox *>* vec = &mCheckboxesByGame[Ets2::Game::ETS2];
	cb = new wxCheckBox(this, wxID_ANY, "Scandinavia");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"north";

	cb = new wxCheckBox(this, wxID_ANY, "Going East!");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"east";

	cb = new wxCheckBox(this, wxID_ANY, "Vive la France!");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"fr";

	cb = new wxCheckBox(this, wxID_ANY, "Italia");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"it";

	cb = new wxCheckBox(this, wxID_ANY, "High Power Cargo Pack");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"trailers";

	cb = new wxCheckBox(this, wxID_ANY, "Heavy Cargo Pack");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"heavy_cargo";

	cb = new wxCheckBox(this, wxID_ANY, "Beyond the Baltic Sea");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"balt";

	cb = new wxCheckBox(this, wxID_ANY, "Road to the Black Sea");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"balkane";

	cb = new wxCheckBox(this, wxID_ANY, "Iberia");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"?";

	vec = &mCheckboxesByGame[Ets2::Game::ATS];
	cb = new wxCheckBox(this, wxID_ANY, "New Mexico");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"nwmexico";

	cb = new wxCheckBox(this, wxID_ANY, "Oregon");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"oregon";

	cb = new wxCheckBox(this, wxID_ANY, "Washington");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"wa";

	cb = new wxCheckBox(this, wxID_ANY, "Heavy Cargo Pack");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"heavy";

	cb = new wxCheckBox(this, wxID_ANY, "Forest Machinery");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"forest";

	cb = new wxCheckBox(this, wxID_ANY, "Utah");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"utah";

	cb = new wxCheckBox(this, wxID_ANY, "Idaho");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"idaho";

	cb = new wxCheckBox(this, wxID_ANY, "Colorado");
	vec->push_back(cb);
	mDlcByCheckbox[cb] = L"colorado";

	wxBoxSizer * parentSizer = new wxBoxSizer(wxHORIZONTAL);
	contentSizer->Add(parentSizer, wxSizerFlags().Expand());
	for (size_t i = 0; i < std::max(mCheckboxesByGame[Ets2::Game::ETS2].size(), mCheckboxesByGame[Ets2::Game::ATS].size()); i++) {
		if ((i % 5) == 0) {
			if (i > 0) {
				parentSizer->AddStretchSpacer(1);
			}

			controlsSizer = new wxBoxSizer(wxVERTICAL);
			parentSizer->Add(controlsSizer, wxSizerFlags().Proportion(2));
		} else {
			controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);
		}
		for (auto& m : mCheckboxesByGame) {
			if (i < m.second.size()) {
				controlsSizer->Add(m.second[i]);
			}
		}
	}

	setSave(nullptr);

	parentSizer->AddStretchSpacer(1);
}

void DlcSelector::setSave(const Ets2::Save * save) {
	mSave = save;
	const Ets2::Save::DlcList * dlcs = nullptr;
	Ets2::Game game = Ets2::Game::ETS2;
	if (mSave != nullptr) {
		game = mSave->getGame();
		dlcs = &(mSave->getDlcs());
	}

	bool present = false;
	for (auto& m : mCheckboxesByGame) {
		for (auto& cb : m.second) {
			cb->Show(m.first == game);
			present = (dlcs != nullptr) && (m.first == game ? (std::find(dlcs->begin(), dlcs->end(), mDlcByCheckbox[cb]) != dlcs->end()) : false);
			if (present && cb->GetLabelText().compare("?") != 0) {
				cb->Enable();
			} else {
				cb->SetValue(false);
				cb->Disable();
			}
		}
	}
	Layout();
}

const Ets2::Save::DlcList DlcSelector::getDlcs() const {
	Ets2::Save::DlcList dlcs = {};
	if (mSave == nullptr) {
		return dlcs;
	}

	for (auto& cb : mCheckboxesByGame.at(mSave->getGame())) {
		if (cb->IsEnabled() && cb->IsChecked()) {
			dlcs.push_back(mDlcByCheckbox.at(cb));
		}
	}
	return dlcs;
}

void DlcSelector::onSelectAll() {
	Ets2::Game game = (mSave == nullptr ? Ets2::Game::ETS2 : mSave->getGame());
	bool clear = false;

	for (auto& cb : mCheckboxesByGame[game]) {
		if (cb->IsChecked()) {
			clear = true;
			break;
		}
	}

	for (auto& cb : mCheckboxesByGame[game]) {
		if (cb->IsEnabled()) {
			cb->SetValue(!clear);
		}
	}
}