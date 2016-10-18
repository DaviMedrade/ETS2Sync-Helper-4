#include "precomp.hpp"
#include "Ets2SyncHelperApp.hpp"

#include "version.hpp"
#include "MainWindow.hpp"

wxIMPLEMENT_APP(Ets2SyncHelperApp);

bool Ets2SyncHelperApp::OnInit() {
	mLocale = new wxLocale(wxLANGUAGE_DEFAULT);

	wxApp::OnInit();

	SetAppName(APP_NAME);
	SetAppDisplayName(APP_DISPLAY_NAME);
	wxInitAllImageHandlers();
	
	MainWindow *mainWindow = new MainWindow(APP_DISPLAY_NAME);
	SetTopWindow(mainWindow);
	mainWindow->Show(true);

	return true;
}

int Ets2SyncHelperApp::OnExit() {
	delete mLocale;
	return wxApp::OnExit();
}
