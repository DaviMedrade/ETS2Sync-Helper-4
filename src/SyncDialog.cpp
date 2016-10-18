#include "precomp.hpp"
#include "SyncDialog.hpp"

SyncDialog::SyncDialog(wxWindow * parent, const Ets2::Save * save, int dlcs, bool clearJobs)
	: wxDialog(parent, wxID_ANY, L"Job Sync") {
	mParent = parent;
	mSave = save;
	mDlcs = dlcs;
	mClearJobs = clearJobs;

	mJobSyncer = new JobSyncer(this, mClearJobs);
	Bind(EVT_JOB_SYNCER_UPDATE, [this](wxCommandEvent&) { onJobSyncerUpdate(); });

	wxPoint border = wxDLG_UNIT(this, wxPoint(7, 7));

	wxBoxSizer * borderSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(borderSizer);
	borderSizer->AddSpacer(border.x);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	borderSizer->Add(sizer, wxSizerFlags().Expand());

	sizer->AddSpacer(border.y);

	mStatus = new StatusText(this, wxID_ANY);
	mStatus->SetLabel(L"Starting…", StatusText::TYPE_STATUS);
	mStatus->SetFont(mStatus->GetFont().MakeLarger());
	sizer->Add(mStatus, wxSizerFlags().Expand());

	sizer->AddSpacer(border.y);

	mSubStatus = new wxStaticText(this, wxID_ANY, L" \n ");
	sizer->Add(mSubStatus, wxSizerFlags().Expand());

	sizer->AddSpacer(wxDLG_UNIT(this, wxPoint(4, 4)).y);

	mProgressBar = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxSize(wxDLG_UNIT(this, wxSize(250, -1))), wxGA_PROGRESS | wxGA_SMOOTH | wxGA_HORIZONTAL);
	mProgressBar->Pulse();
	sizer->Add(mProgressBar);

	sizer->AddSpacer(border.y);

	mCloseButton = new wxButton(this, wxID_CLOSE, L"Cancel");
	mCloseButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { onClose(); });
	sizer->Add(mCloseButton, wxSizerFlags().Right());

	sizer->AddSpacer(border.y);
	borderSizer->AddSpacer(border.x);
	Fit();
	Center();
	mJobSyncer->start(mSave, mDlcs);
	ShowModal();
}

SyncDialog::~SyncDialog() {
	wxDELETE(mJobSyncer);
}

void SyncDialog::onClose() {
	JobSyncer::Status status = mJobSyncer->getStatus();
	switch (status.state) {
	case JobSyncer::STATE_STARTING:
	case JobSyncer::STATE_DOWNLOADING:
	case JobSyncer::STATE_INSERTING_JOBS:
		mJobSyncer->cancel();
		break;
	default:
		Close();
	}
}

void SyncDialog::onJobSyncerUpdate() {
	int progress;
	JobSyncer::Status status = mJobSyncer->getStatus();
	DEBUG_LOG(L"Sync status update: state: %d, progress: %d, message: \"%s\"", status.state, status.progress, status.message);
	std::wstring statusMessage = mStatus->GetLabelText();
	std::wstring subStatusMessage = mSubStatus->GetLabelText();
	StatusText::Type statusType = mStatus->getType();

	switch (status.state) {
	case JobSyncer::STATE_STARTING:
	case JobSyncer::STATE_DOWNLOADING:
		statusType = StatusText::TYPE_STATUS;
		statusMessage = L"Downloading job list…";
		subStatusMessage = status.message;
		progress = status.progress;
		break;
	case JobSyncer::STATE_INSERTING_JOBS:
		statusType = StatusText::TYPE_STATUS;
		statusMessage = mClearJobs ? L"Removing all jobs from the save…" : L"Inserting jobs into the save…";
		subStatusMessage = status.message;
		progress = status.progress;
		break;
	case JobSyncer::STATE_FINISHED:
		statusType = StatusText::TYPE_SUCCESS;
		statusMessage = mClearJobs ? L"Jobs cleared." : L"Sync complete.";
		subStatusMessage = wxString::Format(L"In the game, load the save named “%s”.\n%s", mSave->getName(), mClearJobs ? L"Then, call Assistance (F7) to generate new jobs." : L"").ToStdWstring();
		mCloseButton->SetLabel("Close");
		progress = 100;
		break;
	case JobSyncer::STATE_CANCELED:
		statusType = StatusText::TYPE_ERROR;
		statusMessage = L"Sync canceled.";
		subStatusMessage = wxEmptyString;
		mCloseButton->SetLabel("Close");
		progress = 0;
		break;
	case JobSyncer::STATE_ERROR:
		statusType = StatusText::TYPE_ERROR;
		statusMessage = L"Sync error.";
		subStatusMessage = status.message;
		mCloseButton->SetLabel("Close");
		progress = 0;
		break;
	default:
		statusType = StatusText::TYPE_ERROR;
		statusMessage = L"Sync error.";
		subStatusMessage = wxString::Format(L"Unknown state: %d", status.state).ToStdWstring();
		mCloseButton->SetLabel("Close");
		progress = 0;
	}
	if (statusType != mStatus->getType() || statusMessage != mStatus->GetLabelText()) {
		mStatus->SetLabel(statusMessage, statusType);
	}
	if (subStatusMessage != mSubStatus->GetLabelText()) {
		mSubStatus->SetLabel(subStatusMessage);
	}
	if (progress == -1) {
		mProgressBar->Pulse();
	} else {
		mProgressBar->SetValue(progress);
	}
}