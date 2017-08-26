#pragma once

#include "Ets2/Save.hpp"
#include "StatusText.hpp"
#include "JobSyncer.hpp"

class SyncDialog : public wxDialog {
public:
	SyncDialog(wxWindow * parent, const Ets2::Save * save, const Ets2::Save::DlcList& refusedDlcs, JobSyncer::SyncType syncType, int jobList);
	~SyncDialog();

private:
	wxWindow * mParent;
	const Ets2::Save * mSave;
	std::vector<std::wstring> mRefusedDlcs;
	JobSyncer::SyncType mSyncType;
	int mJobList;
	StatusText * mStatus;
	wxStaticText * mSubStatus;
	wxGauge * mProgressBar;
	wxButton * mCloseButton;
	JobSyncer * mJobSyncer;

	void onClose();
	void onJobSyncerUpdate();
};