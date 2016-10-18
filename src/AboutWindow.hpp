#pragma once

#include "UpdateChecker.hpp"
#include "StatusText.hpp"

#include <wx/activityindicator.h>

// About Window
class AboutWindow : public wxDialog {
public:
	AboutWindow(class MainWindow * wndParent);
	~AboutWindow();

	MainWindow * GetParent();
private:
	UpdateChecker * mUpdateChecker;
	wxActivityIndicator * mCheckingAnimation;
	StatusText * mCheckStatusText;
	wxBoxSizer * mUpdateCheckSizer;
	wxButton * mUpdateCheckAgain;

	void onShow(wxShowEvent& event);
	void onOpenWebsite();
	void onUpdateCheckAgain();
	void onClose();
	void onUpdateStatus();

	void checkUpdates();
};