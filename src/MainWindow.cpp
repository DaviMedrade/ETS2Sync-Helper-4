#include "precomp.hpp"
#include "MainWindow.hpp"

#include "version.hpp"

MainWindow::MainWindow(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	mEts2Info = nullptr;
	mFSWatcher = nullptr;
	mAboutWindow = nullptr;

	SetIcon(wxIcon("app_icon"));

	wxMenu * helpMenu = new wxMenu;
	helpMenu->Append(ID_OPEN_WEBSITE, "Open website…");
	Bind(wxEVT_MENU, [this](wxCommandEvent&) { openWebsite(); }, ID_OPEN_WEBSITE);
	helpMenu->AppendSeparator();
	helpMenu->Append(wxID_ABOUT, "About…");
	Bind(wxEVT_MENU, [this](wxCommandEvent&) { onAbout(); }, wxID_ABOUT);

	wxMenuBar * menuBar = new wxMenuBar;
	menuBar->Append(helpMenu, "Help");

	SetMenuBar(menuBar);

	wxBoxSizer * winSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel * panel = new wxPanel(this, wxID_ANY);
	winSizer->Add(panel, wxSizerFlags().Expand());

	wxBoxSizer * borderSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

	borderSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, wxDLG_UNIT(panel, wxPoint(7, 7)).x).Expand());

	mGameSettingsPanel = new GameSettingsPanel(panel, wxID_ANY);
	mGameSettingsPanel->Bind(EVT_CONFIG_DIR_CHANGED, [this](wxCommandEvent&) { onConfigDirChanged(); });
	mEts2Children.push_back(mGameSettingsPanel);
	mainSizer->Add(mGameSettingsPanel, wxSizerFlags().Expand());

	mainSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(0, 7)).y);

	mProfileSelector = new ProfileSelector(panel, wxID_ANY);
	mEts2Children.push_back(mProfileSelector);
	mainSizer->Add(mProfileSelector, wxSizerFlags().Expand());
	mProfileSelector->Bind(EVT_PROFILE_SELECTED, [this](wxCommandEvent&) { onProfileChanged(); });

	mainSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(0, 4)).y);

	mSaveSelector = new SaveSelector(panel, wxID_ANY);
	mEts2Children.push_back(mSaveSelector);
	mainSizer->Add(mSaveSelector, wxSizerFlags().Expand());
	mSaveSelector->Bind(EVT_SAVE_SELECTED, [this](wxCommandEvent&) { onSaveChanged(); });

	mainSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(7, 7)).y);

	mDlcSelector = new DlcSelector(panel, wxID_ANY);
	mEts2Children.push_back(mDlcSelector);
	mainSizer->Add(mDlcSelector, wxSizerFlags().Expand());

	mainSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(7, 7)).y);

	mJobListSelector = new JobListSelector(panel, wxID_ANY);
	mEts2Children.push_back(mJobListSelector);
	mainSizer->Add(mJobListSelector, wxSizerFlags().Expand());

	mainSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(7, 7)).y);

	wxBoxSizer * closeSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(closeSizer, wxSizerFlags().Expand());

	mSyncButton = new wxButton(panel, wxID_ANY, L"Sync Jobs");
	mSyncButton->SetFont(mSyncButton->GetFont().MakeBold().MakeLarger());
	mSyncButton->SetForegroundColour(wxColor(0x008800));
	closeSizer->Add(mSyncButton);
	mSyncButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { onSync(); });

	closeSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(4, 4)).y);

	mClearJobsButton = new wxButton(panel, wxID_ANY, L"Clear Jobs");
	closeSizer->Add(mClearJobsButton);
	mClearJobsButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { onClearJobs(); });
	mClearJobsButton->SetToolTip(L"Clears the Freight Market, allowing the game to generate new, unsynced jobs.");

	closeSizer->AddSpacer(wxDLG_UNIT(panel, wxPoint(4, 4)).y);

	mResetEconomyButton = new wxButton(panel, wxID_ANY, L"Reset Economy");
	closeSizer->Add(mResetEconomyButton);
	mResetEconomyButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { onResetEconomy(); });
	mResetEconomyButton->SetToolTip(L"Makes the save trigger an economy reset when loaded. That generates new, unsynced jobs and moves your truck to your home garage.");

	closeSizer->AddStretchSpacer();

	wxButton * closeButton = new wxButton(panel, wxID_CLOSE, L"Close");
	closeButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { Close(); }, wxID_CLOSE);
	closeSizer->Add(closeButton);

	panel->SetSizerAndFit(borderSizer);
	SetSizerAndFit(winSizer);
	Center();

	mFSTimer = new wxTimer();
	mFSTimer->SetOwner(this);
	Bind(wxEVT_TIMER, [this](wxTimerEvent&) { onFileSystemTimer(); }, mFSTimer->GetId());

	updateEts2Info(Ets2::Info::getDefaultDirectory(mGameSettingsPanel->getGame()));

#if false // if working on the About window
	// Just show the About window, then quit
	onAbout();
	Close();
#endif
}

MainWindow::~MainWindow() {
	wxDELETE(mFSTimer);
	wxDELETE(mFSWatcher);
	wxDELETE(mEts2Info);
	wxDELETE(mAboutWindow);
}

void MainWindow::updateEts2Info(std::wstring directory) {
	wxDELETE(mEts2Info);
	mEts2Info = new Ets2::Info(mGameSettingsPanel->getGame(), directory);

	mFSTimer->Stop();
	wxDELETE(mFSWatcher);
	if (mEts2Info->isValid()) {
		mFSWatcher = new wxFileSystemWatcher();
		mFSWatcher->Bind(wxEVT_FSWATCHER, [this](wxFileSystemWatcherEvent&) { onFileSystemWatcher(); });
		mFSWatcher->AddTree(wxFileName::DirName(mEts2Info->getDirectory()));
	}

	for (auto& w : mEts2Children) {
		w->setEts2Info(mEts2Info);
	}
	onSaveChanged();
	mJobListSelector->setJobList(-1);
}

void MainWindow::openWebsite() {
	wxLaunchDefaultBrowser(APP_URL_WEBSITE);
}

void MainWindow::onAbout() {
	if (!mAboutWindow)
		mAboutWindow = new AboutWindow(this);

	mAboutWindow->ShowModal();
}

void MainWindow::onConfigDirChanged() {
	updateEts2Info(mGameSettingsPanel->getDirectory().ToStdWstring());
}

void MainWindow::onFileSystemWatcher() {
	DEBUG_LOG(L"Got file system watcher event. %s the timer.", mFSTimer->IsRunning() ? L"Restarting" : L"Starting");
	Disable();
	mFSTimer->StartOnce(FSTIMER_DELAY);
}

void MainWindow::onFileSystemTimer() {
	DEBUG_LOG(L"Got file system timer event. Reloading Ets2::Info.");
	updateEts2Info(mEts2Info->getDirectory());
	Enable();
}

const Ets2::Profile * MainWindow::getSelectedProfile() const {
	const Ets2::ProfileList& profiles = mEts2Info->getProfiles();
	size_t n = mProfileSelector->GetSelection();
	const Ets2::Profile * profile = nullptr;
	if (n != wxNOT_FOUND && n < profiles.size()) {
		profile = profiles[n];
	}
	DEBUG_LOG(L"%d profile(s); selected index %d: %s", profiles.size(), n, profile == nullptr ? L"null" : profile->getName());
	return profile;
}

const Ets2::Save * MainWindow::getSelectedSave() const {
	const Ets2::Profile * profile = getSelectedProfile();
	const Ets2::Save * save = nullptr;
	if (profile != nullptr) {
		const Ets2::SaveList& saves = profile->getSaves();
		size_t n = mSaveSelector->GetSelection();
		if (n != wxNOT_FOUND && n < saves.size()) {
			save = saves[n];
		}
		DEBUG_LOG(L"%d save(s); selected index %d: %s", saves.size(), n, save == nullptr ? L"null" : save->getName());
	} else {
		DEBUG_LOG(L"selected profile is null, so selected save is null");
	}
	return save;
}

void MainWindow::onProfileChanged() {
	const Ets2::Profile * profile = getSelectedProfile();
	DEBUG_LOG(L"Profile changed to %s", profile == nullptr ? L"null" : profile->getName());
	if (profile != nullptr) {
		mSaveSelector->setList(&profile->getSaves());
	}
	onSaveChanged();
}

void MainWindow::onSaveChanged() {
	const Ets2::Save * save = getSelectedSave();
	DEBUG_LOG(L"Save changed to %s", save == nullptr ? L"null" : save->getName());
	mDlcSelector->setSave(save);
	mSyncButton->Enable(save != nullptr);
}

void MainWindow::onSync() {
	mFSTimer->Stop();
	wxDELETE(mFSWatcher);
	mSyncDialog = new SyncDialog(this, getSelectedSave(), mDlcSelector->getRefusedDlcs(), JobSyncer::SyncType::SYNC, mJobListSelector->getJobList());
	wxDELETE(mSyncDialog);
	updateEts2Info(mEts2Info->getDirectory());
}

void MainWindow::onClearJobs() {
	mFSTimer->Stop();
	wxDELETE(mFSWatcher);
	mSyncDialog = new SyncDialog(this, getSelectedSave(), mDlcSelector->getRefusedDlcs(), JobSyncer::SyncType::CLEAR, -1);
	wxDELETE(mSyncDialog);
	updateEts2Info(mEts2Info->getDirectory());
}

void MainWindow::onResetEconomy() {
	mFSTimer->Stop();
	wxDELETE(mFSWatcher);
	mSyncDialog = new SyncDialog(this, getSelectedSave(), mDlcSelector->getRefusedDlcs(), JobSyncer::SyncType::RESET_ECONOMY, -1);
	wxDELETE(mSyncDialog);
	updateEts2Info(mEts2Info->getDirectory());
}
