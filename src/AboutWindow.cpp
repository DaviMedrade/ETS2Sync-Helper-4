#include "precomp.hpp"
#include "AboutWindow.hpp"

#include "version.hpp"
#include "MainWindow.hpp"

#include <wx/hyperlink.h>
#include <wx/statline.h>

AboutWindow::AboutWindow(MainWindow * wndParent)
	: wxDialog(wndParent, wxID_ANY, "About " + APP_DISPLAY_NAME) {

	SetIcon(GetParent()->GetIcon());
	Bind(wxEVT_SHOW, &AboutWindow::onShow, this);

	wxPoint border = wxDLG_UNIT(this, wxPoint(7, 7));
	wxPoint internalBorder = wxDLG_UNIT(this, wxPoint(4, 4));

	mUpdateChecker = new UpdateChecker(this);
	mUpdateChecker->Bind(EVT_UPDATE_CHECKER, [this](wxCommandEvent&) { onUpdateStatus(); });

	wxBoxSizer * canvas = new wxBoxSizer(wxHORIZONTAL);
	wxBitmap logo("app_logo", wxBITMAP_TYPE_PNG_RESOURCE);
	canvas->Add(new wxStaticBitmap(this, wxID_ANY, logo), wxSizerFlags().Border(wxRIGHT, border.x));

	wxBoxSizer * infoSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText * appName = new wxStaticText(this, wxID_ANY, APP_DISPLAY_NAME);
	appName->SetFont(appName->GetFont().Scale(3.0).MakeBold());
	infoSizer->Add(appName);

	wxStaticText * appVersion = new wxStaticText(this, wxID_ANY, "Version " + APP_VERSION);
	appVersion->SetFont(appVersion->GetFont().Scale(1.7f).MakeBold());
	infoSizer->Add(appVersion, wxSizerFlags());

	infoSizer->AddSpacer(internalBorder.y);

	mUpdateCheckSizer = new wxBoxSizer(wxHORIZONTAL);

	mCheckingAnimation = new wxActivityIndicator(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(10, 10)));
	mUpdateCheckSizer->Add(mCheckingAnimation, wxSizerFlags().CenterVertical().Proportion(0).Border(wxRIGHT, 5));

	mCheckStatusText = new StatusText(this, wxID_ANY);
	mUpdateCheckSizer->Add(mCheckStatusText, wxSizerFlags().CenterVertical(). Proportion(1));

	mUpdateCheckSizer->AddSpacer(internalBorder.x);

	mUpdateCheckAgain = new wxButton(this, wxID_ANY, "Stop");
	mUpdateCheckAgain->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {onUpdateCheckAgain(); });
	mUpdateCheckSizer->Add(mUpdateCheckAgain, wxSizerFlags().CenterVertical().Proportion(0));

	infoSizer->Add(mUpdateCheckSizer, wxSizerFlags().Expand());

	infoSizer->AddSpacer(internalBorder.y);

	wxBoxSizer * websiteSizer = new wxBoxSizer(wxHORIZONTAL);
	websiteSizer->Add(new wxStaticText(this, wxID_ANY, "Github: "));
	websiteSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, APP_URL_WEBSITE, APP_URL_WEBSITE));
	infoSizer->Add(websiteSizer);

	infoSizer->AddSpacer(border.y);

	wxBoxSizer * creatorSizer = new wxBoxSizer(wxHORIZONTAL);
	creatorSizer->Add(new wxStaticText(this, wxID_ANY, "Developesr: "));
	creatorSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, "Inklay & David Santos", "https://github.com/Inklay/"));
	infoSizer->Add(creatorSizer);

	/*wxBoxSizer * jobListSizer = new wxBoxSizer(wxHORIZONTAL);
	jobListSizer->Add(new wxStaticText(this, wxID_ANY, "Job List: "));
	jobListSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, "ETS2 Job Sync", "http://www.ets2sync.com/"));
	infoSizer->Add(jobListSizer);

	wxBoxSizer * translatorSizer = new wxBoxSizer(wxHORIZONTAL);
	translatorSizer->Add(new wxStaticText(this, wxID_ANY, "English Translation: "));
	translatorSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, "David Santos", "https://github.com/davidsantos-br/"));
	infoSizer->Add(translatorSizer);*/

	infoSizer->AddSpacer(border.y);

	infoSizer->Add(new wxStaticText(this, wxID_ANY, wxGetLibraryVersionInfo().ToString().Trim()));

	canvas->Add(infoSizer);

	wxBoxSizer * windowSizer = new wxBoxSizer(wxVERTICAL);
	windowSizer->AddSpacer(border.y);
	windowSizer->Add(canvas, wxSizerFlags().Border(wxLEFT|wxRIGHT, border.x));
	windowSizer->AddSpacer(border.y);
	windowSizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
	windowSizer->AddSpacer(border.y);

	wxBoxSizer * closeSizer = new wxBoxSizer(wxHORIZONTAL);
	closeSizer->Add(new wxStaticText(this, wxID_ANY, "Built on " __DATE__ " " __TIME__ " ("
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
	" build)"), wxSizerFlags().CenterVertical());
	closeSizer->AddStretchSpacer();
	wxButton * closeButton = new wxButton(this, wxID_CLOSE, "Close");
	closeSizer->Add(closeButton);
	windowSizer->Add(closeSizer, wxSizerFlags().Expand().Border(wxLEFT|wxRIGHT, border.x));
	closeButton->SetFocus();
	closeButton->SetDefault();
	closeButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { Close(); });

	windowSizer->AddSpacer(border.y);
	SetSizerAndFit(windowSizer);
}

AboutWindow::~AboutWindow() {
	delete mUpdateChecker;
}

MainWindow * AboutWindow::GetParent() {
	return static_cast<MainWindow *>(wxDialog::GetParent());
}

void AboutWindow::checkUpdates() {
	mUpdateChecker->start();
}

void AboutWindow::onUpdateStatus() {
	UpdateChecker::Status status = mUpdateChecker->getStatus();

	StatusText::Type statusTextType;
	if (status.state == UpdateChecker::State::RUNNING) {
		statusTextType = StatusText::Type::STATUS;
	} else if (status.state == UpdateChecker::State::FINISHED && status.result == UpdateChecker::Result::UP_TO_DATE) {
		statusTextType = StatusText::Type::SUCCESS;
	} else {
		statusTextType = StatusText::Type::FAILURE;
	}
	mCheckStatusText->SetLabel(UpdateChecker::getStatusDescription(status), statusTextType);
	if (status.state != UpdateChecker::State::RUNNING) {
		if (mCheckingAnimation->IsShown()) {
			mCheckingAnimation->Stop();
			mCheckingAnimation->Hide();
			mUpdateCheckAgain->SetLabel("Check Again");
		}
	} else {
		if (!mCheckingAnimation->IsShown()) {
			mCheckingAnimation->Show();
			mCheckingAnimation->Start();
			mUpdateCheckAgain->SetLabel("Stop");
		}
	}
	mUpdateCheckSizer->Layout();
}

void AboutWindow::onShow(wxShowEvent& weEvent) {
	if (weEvent.IsShown()) {
		Center();
		checkUpdates();
	}
}

void AboutWindow::onUpdateCheckAgain() {
	if (mUpdateChecker->getStatus().state == UpdateChecker::State::RUNNING) {
		mUpdateChecker->cancel();
	} else {
		checkUpdates();
	}
}

void AboutWindow::onOpenWebsite() {
	GetParent()->openWebsite();
}

void AboutWindow::onClose() {
	Close();
}