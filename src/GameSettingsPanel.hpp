#pragma once

#include "Ets2StaticBox.hpp"
#include "StatusText.hpp"
#include <wx/hyperlink.h>

wxDECLARE_EVENT(EVT_CONFIG_DIR_CHANGED, wxCommandEvent);

class GameSettingsPanel : public Ets2StaticBox {
public:
	GameSettingsPanel(wxWindow * parent, wxWindowID id);
	~GameSettingsPanel();

	enum MENU_ID {
		MENU_ID_CONFIG_DIR_OPEN = wxID_HIGHEST + 1,
		MENU_ID_CONFIG_DIR_OPEN_FILE,
		MENU_ID_CONFIG_DIR_CHANGE,
		MENU_ID_CONFIG_DIR_CHECK_AGAIN,
		MENU_ID_CONFIG_DIR_DEFAULT
	};

	void updateFromEts2Info() override;
	wxString getDirectory();

private:
	wxBoxSizer * mConfigDirSizer;
	wxStaticText * mConfigDirText;
	wxStaticText * mConfigDirSeparator;
	wxHyperlinkCtrl * mConfigDirOptionsButton;
	wxMenu * mConfigDirOptionsMenu;
	wxMenuItem * mConfigDirOptionOpen;
	wxMenuItem * mConfigDirOptionOpenFile;
	wxMenuItem * mConfigDirOptionDefault;
	wxBoxSizer * mSaveFormatSizer;
	wxStaticText * mSaveFormatText;
	wxStaticText * mSaveFormatSeparator;
	wxHyperlinkCtrl * mFixSaveFormatButton;
	StatusText * mStatusText;

	void onConfigDirOptions();
	void onConfigDirOptionSelected(wxCommandEvent& event);
	void setConfigDirText(const wxString& text);
};