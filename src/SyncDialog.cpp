#include "precomp.hpp"
#include "SyncDialog.hpp"

SyncDialog::SyncDialog(wxWindow * parent, const Ets2::Save * save, const Ets2::Save::DlcList& refusedDlcs, JobSyncer::SyncType syncType, int jobList)
	: wxDialog(parent, wxID_ANY, L"Job Sync") {
	mParent = parent;
	mSave = save;
	mRefusedDlcs = refusedDlcs;
	mSyncType = syncType;
	mJobList = jobList;

	mJobSyncer = new JobSyncer(this, syncType);
	Bind(EVT_JOB_SYNCER_UPDATE, [this](wxCommandEvent&) { onJobSyncerUpdate(); });

	wxPoint border = wxDLG_UNIT(this, wxPoint(7, 7));

	wxBoxSizer * borderSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(borderSizer);
	borderSizer->AddSpacer(border.x);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	borderSizer->Add(sizer, wxSizerFlags().Expand());

	sizer->AddSpacer(border.y);

	mStatus = new StatusText(this, wxID_ANY);
	mStatus->SetLabel(L"Starting…", StatusText::Type::STATUS);
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
	mJobSyncer->start(mSave, mRefusedDlcs, mJobList);
	ShowModal();
}

SyncDialog::~SyncDialog() {
	wxDELETE(mJobSyncer);
}

void SyncDialog::onClose() {
	JobSyncer::Status status = mJobSyncer->getStatus();
	switch (status.state) {
	case JobSyncer::State::STARTING:
	case JobSyncer::State::DOWNLOADING:
	case JobSyncer::State::INSERTING_JOBS:
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
	case JobSyncer::State::STARTING:
	case JobSyncer::State::DOWNLOADING:
		statusType = StatusText::Type::STATUS;
		statusMessage = L"Downloading job list…";
		subStatusMessage = status.message;
		progress = status.progress;
		break;
	case JobSyncer::State::INSERTING_JOBS:
		statusType = StatusText::Type::STATUS;
		statusMessage = (mSyncType == JobSyncer::SyncType::SYNC) ? L"Inserting jobs into the save…" : L"Removing all jobs from the save…";
		subStatusMessage = status.message;
		progress = status.progress;
		break;
	case JobSyncer::State::FINISHED:
		statusType = StatusText::Type::SUCCESS;
		statusMessage = (mSyncType == JobSyncer::SyncType::SYNC) ? L"Sync complete." : L"Jobs cleared.";
#if _DEBUG
		if (mSyncType == JobSyncer::SyncType::SYNC) {
			statusMessage.append(wxString::Format(L" Total: %i %s.", status.progress, status.progress == 1 ? L"job" : L"jobs"));
		}
#endif
		subStatusMessage = wxString::Format(L"In the game, load the save named “%s”.\n%s", mSave->getName(), (mSyncType == JobSyncer::SyncType::CLEAR) ? L"Then, call Assistance (F7) to generate new jobs." : L"").ToStdWstring();
		mCloseButton->SetLabel("Close");
		progress = 100;
		break;
	case JobSyncer::State::CANCELED:
		statusType = StatusText::Type::FAILURE;
		statusMessage = L"Sync canceled.";
		subStatusMessage = wxEmptyString;
		mCloseButton->SetLabel("Close");
		progress = 0;
		break;
	case JobSyncer::State::FAILED:
		statusType = StatusText::Type::FAILURE;
		statusMessage = L"Sync error.";
		subStatusMessage = status.message;
		mCloseButton->SetLabel("Close");
		progress = 0;
		break;
	default:
		statusType = StatusText::Type::FAILURE;
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