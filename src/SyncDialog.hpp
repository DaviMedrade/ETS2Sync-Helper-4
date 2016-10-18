#pragma once

#include "Ets2/Save.hpp"
#include "StatusText.hpp"
#include "JobSyncer.hpp"

class SyncDialog : public wxDialog {
public:
	SyncDialog(wxWindow * parent, const Ets2::Save * save, int dlcs, bool clearJobs);
	~SyncDialog();

private:
	wxWindow * mParent;
	const Ets2::Save * mSave;
	int mDlcs;
	bool mClearJobs;
	StatusText * mStatus;
	wxStaticText * mSubStatus;
	wxGauge * mProgressBar;
	wxButton * mCloseButton;
	JobSyncer * mJobSyncer;

	void onClose();
	void onJobSyncerUpdate();
};