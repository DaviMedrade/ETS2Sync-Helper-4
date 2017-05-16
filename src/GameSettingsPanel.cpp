#include "precomp.hpp"

#include "GameSettingsPanel.hpp"
#include "ShowLastWindowsError.hpp"

#include <wx/filename.h>

wxDEFINE_EVENT(EVT_CONFIG_DIR_CHANGED, wxCommandEvent);

GameSettingsPanel::GameSettingsPanel(wxWindow * parent, wxWindowID id)
	: Ets2StaticBox(parent, id, L"Game Settings") {
	mConfigDirOptionsMenu = new wxMenu();

	mConfigDirOptionOpen = new wxMenuItem(mConfigDirOptionsMenu, MENU_ID_CONFIG_DIR_OPEN, L"View folder");
	mConfigDirOptionsMenu->Append(mConfigDirOptionOpen);
	mConfigDirOptionOpenFile = new wxMenuItem(mConfigDirOptionsMenu, MENU_ID_CONFIG_DIR_OPEN_FILE, L"Open game config file");
	mConfigDirOptionsMenu->Append(mConfigDirOptionOpenFile);
	mConfigDirOptionsMenu->AppendSeparator();
	mConfigDirOptionsMenu->Append(MENU_ID_CONFIG_DIR_CHANGE, L"Change…");
	mConfigDirOptionsMenu->Append(MENU_ID_CONFIG_DIR_CHECK_AGAIN, L"Check again");
	mConfigDirOptionDefault = new wxMenuItem(mConfigDirOptionsMenu, MENU_ID_CONFIG_DIR_DEFAULT, L"Set to default");
	mConfigDirOptionsMenu->Append(mConfigDirOptionDefault);

	mConfigDirOptionsMenu->Bind(wxEVT_MENU, &GameSettingsPanel::onConfigDirOptionSelected, this);

	wxPoint spacer(wxDLG_UNIT(this, wxPoint(4, 4)));

	wxBoxSizer * contentSizer = getContentSizer();

	wxFlexGridSizer * mainSizer = new wxFlexGridSizer(2, spacer.y, spacer.x);
	mainSizer->AddGrowableCol(0, 0);
	mainSizer->AddGrowableCol(1, 1);
	contentSizer->Add(mainSizer, wxSizerFlags().Expand());

	// Game
	mainSizer->Add(new wxStaticText(this, wxID_ANY, L"Game:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), wxSizerFlags().CenterVertical().Expand());
	wxSizer * gameSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(gameSizer);
	mGameEts2 = new wxRadioButton(this, wxID_ANY, L"ETS2");
	mGameAts = new wxRadioButton(this, wxID_ANY, L"ATS");
	gameSizer->Add(mGameEts2);
	gameSizer->Add(mGameAts);

	mGameEts2->Bind(wxEVT_RADIOBUTTON, [this](wxCommandEvent&) { setGame(Ets2::Game::ETS2); });
	mGameAts->Bind(wxEVT_RADIOBUTTON, [this](wxCommandEvent&) { setGame(Ets2::Game::ATS); });

	// Settings Folder
	mainSizer->Add(new wxStaticText(this, wxID_ANY, L"Settings Folder:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), wxSizerFlags().CenterVertical().Expand());

	mConfigDirSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(mConfigDirSizer, wxSizerFlags().Expand());

	mConfigDirText = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_MIDDLE);
	mConfigDirSeparator = new wxStaticText(this, wxID_ANY, L" - ");
	mConfigDirOptionsButton = new wxHyperlinkCtrl(this, wxID_ANY, L"Options", wxEmptyString);
	mConfigDirOptionsButton->SetMinSize(mConfigDirOptionsButton->GetClientSize());
	mConfigDirOptionsButton->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent&) { onConfigDirOptions(); });

	mConfigDirSizer->Add(mConfigDirText, wxSizerFlags().CenterVertical());
	mConfigDirSizer->Add(mConfigDirSeparator, wxSizerFlags().CenterVertical());
	mConfigDirSizer->Add(mConfigDirOptionsButton, wxSizerFlags().CenterVertical());

	// Save Format
	mainSizer->Add(new wxStaticText(this, wxID_ANY, L"Save Format:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), wxSizerFlags().CenterVertical().Expand());

	mSaveFormatSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(mSaveFormatSizer, wxSizerFlags().CenterVertical().Expand());

	mSaveFormatText = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_MIDDLE);
	mSaveFormatSeparator = new wxStaticText(this, wxID_ANY, L" - ");
	//mFixSaveFormatButton = new wxHyperlinkCtrl(this, wxID_ANY, L"Set to “Binary”", wxEmptyString);
	//mFixSaveFormatButton->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent&) { getEts2Info()->changeSaveFormat(Ets2::Info::SaveFormat::BINARY); });
	mFixSaveFormatButton = new wxHyperlinkCtrl(this, wxID_ANY, L"Set to “Text”", wxEmptyString);
	mFixSaveFormatButton->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent&) { getEts2Info()->changeSaveFormat(Ets2::Info::SaveFormat::TEXT); });

	mSaveFormatSizer->Add(mSaveFormatText, wxSizerFlags().CenterVertical());
	mSaveFormatSizer->Add(mSaveFormatSeparator, wxSizerFlags().CenterVertical());
	mSaveFormatSizer->Add(mFixSaveFormatButton, wxSizerFlags().CenterVertical());

	contentSizer->AddSpacer(spacer.y);

	mStatusText = new StatusText(this, wxID_ANY);
	contentSizer->Add(mStatusText);

	setGame(Ets2::Game::ETS2, false);
}

GameSettingsPanel::~GameSettingsPanel() {
	wxDELETE(mConfigDirOptionsMenu);
}

Ets2::Game GameSettingsPanel::getGame() {
	return mGame;
}

void GameSettingsPanel::setGame(Ets2::Game game, bool sendEvent) {
	mGame = game;
	if (game == Ets2::Game::ATS) {
		mGameAts->SetValue(true);
		mGameEts2->SetValue(false);
	} else {
		mGameAts->SetValue(false);
		mGameEts2->SetValue(true);
	}
	if (sendEvent) {
		setConfigDirText(Ets2::Info::getDefaultDirectory(game));
		QueueEvent(new wxCommandEvent(EVT_CONFIG_DIR_CHANGED, GetId()));
	}
}

void GameSettingsPanel::onConfigDirOptions() {
	wxPoint position = mConfigDirOptionsButton->GetPosition();
	PopupMenu(mConfigDirOptionsMenu, position.x, position.y + mConfigDirOptionsButton->GetSize().y);
}

void GameSettingsPanel::onConfigDirOptionSelected(wxCommandEvent& event) {
	wxString dir;
	switch (event.GetId()) {
	case MENU_ID_CONFIG_DIR_OPEN:
		wxLaunchDefaultApplication(getDirectory());
		return;
	case MENU_ID_CONFIG_DIR_OPEN_FILE:
		wxLaunchDefaultApplication(getEts2Info()->getConfigFileName());
		return;
	case MENU_ID_CONFIG_DIR_DEFAULT:
		dir = Ets2::Info::getDefaultDirectory(getGame());
		break;
	case MENU_ID_CONFIG_DIR_CHECK_AGAIN:
		dir = mConfigDirText->GetLabel();
		break;
	case MENU_ID_CONFIG_DIR_CHANGE:
		wxDirDialog dlg(this, L"Select the Game Settings folder", getDirectory(), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (dlg.ShowModal() == wxID_OK) {
			dir = dlg.GetPath();
		} else {
			return;
		}
		break;
	}
	/*mConfigDirText->SetLabel(dir);
	mConfigDirText->SetMaxSize(wxSize(mConfigDirSizer->GetSize().x - mConfigDirOptionsButton->GetSize().x - mConfigDirSeparator->GetSize().x, -1));*/
	setConfigDirText(dir);
	QueueEvent(new wxCommandEvent(EVT_CONFIG_DIR_CHANGED, GetId()));
}

void GameSettingsPanel::updateFromEts2Info() {
	Ets2::Info * ets2Info = getEts2Info();

	wxFileName dir = wxFileName::DirName(ets2Info->getDirectory());
	bool isDefaultDir = dir.SameAs(wxFileName::DirName(Ets2::Info::getDefaultDirectory(getGame())));
	mConfigDirOptionOpen->Enable(dir.Exists());
	mConfigDirOptionOpenFile->Enable(ets2Info->isValid());
	mConfigDirOptionDefault->Enable(!isDefaultDir);
	MenuId id = MENU_ID_CONFIG_DIR_OPEN;
	if (!ets2Info->isValid()) {
		id = isDefaultDir ? MENU_ID_CONFIG_DIR_CHANGE : MENU_ID_CONFIG_DIR_DEFAULT;
	}
	SetMenuDefaultItem((HMENU)mConfigDirOptionsMenu->GetHMenu(), id, FALSE);

	if (mConfigDirText->GetLabel() != ets2Info->getDirectory()) {
		setConfigDirText(ets2Info->getDirectory());
	}
	if (ets2Info->isValid()) {
		//if (ets2Info->getSaveFormat() == Ets2::Info::SaveFormat::BINARY) {
		if (ets2Info->getSaveFormat() == Ets2::Info::SaveFormat::TEXT) {
			mSaveFormatSeparator->Hide();
			mFixSaveFormatButton->Hide();
		} else {
			mSaveFormatSeparator->Show();
			mFixSaveFormatButton->Show();
		}
		mSaveFormatText->SetToolTip(L"");
		switch (ets2Info->getSaveFormat()) {
		case Ets2::Info::SaveFormat::BINARY:
			mSaveFormatText->SetLabel(L"Binary");
			//mStatusText->SetLabel(L"Settings OK", StatusText::Type::SUCCESS);
			mStatusText->SetLabel("The recommended save format is “Text”.", StatusText::Type::WARNING);
			break;
		case Ets2::Info::SaveFormat::TEXT:
			mSaveFormatText->SetLabel(L"Text");
			//mStatusText->SetLabel("The recommended save format is “Binary”.", StatusText::Type::WARNING);
			mStatusText->SetLabel(L"Settings OK", StatusText::Type::SUCCESS);
			break;
		case Ets2::Info::SaveFormat::BOTH:
			mSaveFormatText->SetLabel(L"Hybrid");
			//mStatusText->SetLabel("The recommended save format is “Binary”.", StatusText::Type::WARNING);
			mStatusText->SetLabel("The recommended save format is “Text”.", StatusText::Type::WARNING);
			break;
		case Ets2::Info::SaveFormat::NOT_FOUND:
			mSaveFormatText->SetLabel(L"Not found");
			mStatusText->SetLabel("The save format was not found in the settings file.", StatusText::Type::FAILURE);
			break;
		default:
			mStatusText->SetLabel("Unknown save format.", StatusText::Type::FAILURE);
			mSaveFormatText->SetLabel(wxString::Format(L"Unknown (%s)", ets2Info->getRawSaveFormat()));
			mSaveFormatText->SetToolTip(ets2Info->getRawSaveFormat());
		}
		mSaveFormatText->SetMaxSize(wxSize(mSaveFormatSizer->GetSize().x - mFixSaveFormatButton->GetSize().x - mSaveFormatSeparator->GetSize().x, -1));
		Layout();
	} else {
		mSaveFormatText->SetLabel(L"Not found");
		mSaveFormatSeparator->Hide();
		mFixSaveFormatButton->Hide();
		mStatusText->SetLabel(L"Game settings were not found in the directory above.", StatusText::Type::FAILURE);
		Layout();
	}
}

wxString GameSettingsPanel::getDirectory() {
	return mConfigDirText->GetLabel();
}

void GameSettingsPanel::setConfigDirText(const wxString& text) {
	mConfigDirText->SetLabel(text);
	int maxWidth = mConfigDirSizer->GetSize().x - mConfigDirOptionsButton->GetSize().x - mConfigDirSeparator->GetSize().x;
	if (mConfigDirText->GetSize().x > maxWidth) {
		mConfigDirText->SetToolTip(text);
	} else {
		mConfigDirText->SetToolTip(L"");
	}
	mConfigDirText->SetMaxSize(wxSize(maxWidth, -1));
	Layout();
}
