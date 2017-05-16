#pragma once

#include "AboutWindow.hpp"
#include "Ets2StaticBox.hpp"
#include "GameSettingsPanel.hpp"
#include "ProfileSelector.hpp"
#include "SaveSelector.hpp"
#include "DlcSelector.hpp"
#include "SyncDialog.hpp"

#include <wx/fswatcher.h>
#include <wx/progdlg.h>

// Main Application Window
class MainWindow : public wxFrame {
public:
	MainWindow(const wxString& title);
	~MainWindow();

	void openWebsite();

private:
	enum ids {
		ID_OPEN_WEBSITE = wxID_HIGHEST + 1
	};
	// How long to wait after a FileSystemWatcher event before re-reading the game folder
	static const int FSTIMER_DELAY = 2000;

	Ets2::Info * mEts2Info;
	wxFileSystemWatcher * mFSWatcher;
	wxTimer * mFSTimer;
	class AboutWindow * mAboutWindow;
	std::vector<Ets2StaticBox *> mEts2Children;
	GameSettingsPanel * mGameSettingsPanel;
	ProfileSelector * mProfileSelector;
	SaveSelector * mSaveSelector;
	DlcSelector * mDlcSelector;
	wxButton * mSyncButton;
	wxButton * mClearJobsButton;
	wxButton * mResetEconomyButton;
	SyncDialog * mSyncDialog;

	const Ets2::Profile * getSelectedProfile() const;
	const Ets2::Save * getSelectedSave() const;

	void updateEts2Info(std::wstring directory);
	void onAbout();
	void onConfigDirChanged();
	void onFileSystemWatcher();
	void onFileSystemTimer();
	void onProfileChanged();
	void onSaveChanged();
	void onSync();
	void onClearJobs();
	void onResetEconomy();
};

