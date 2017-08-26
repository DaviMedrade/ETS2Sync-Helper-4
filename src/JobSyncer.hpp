#pragma once

#include "Ets2\Save.hpp"
#include <wininet.h>

wxDECLARE_EVENT(EVT_JOB_SYNCER_UPDATE, wxCommandEvent);

#define SYNC_DEBUG_LOG(fmt, ...) DEBUG_LOG(wxString(L"Save \"%s\": ")+wxString(fmt), mSave == nullptr ? L"null" : mSave->getName(), __VA_ARGS__)

class JobSyncer : public wxEvtHandler, public wxThreadHelper {
public:
	enum class SyncType {
		SYNC,
		CLEAR,
		RESET_ECONOMY
	};
	JobSyncer(wxEvtHandler * eventHandler, SyncType syncType);
	~JobSyncer();

	enum class State {
		NOT_STARTED,
		STARTING,
		DOWNLOADING,
		INSERTING_JOBS,
		FINISHED,
		CANCELED,
		FAILED,
		OUTDATED
	};

	struct Status {
		State state;
		int progress;
		wxString message;
	};

	void start(const Ets2::Save * save, const Ets2::Save::DlcList& refusedDlcs, int jobList);
	const Status getStatus();
	void cancel();

private:
	static const int SET_STATE = 1 << 0;
	static const int SET_PROGRESS = 1 << 1;
	static const int SET_MESSAGE = 1 << 2;
	static const int SET_ALL = SET_STATE | SET_PROGRESS | SET_MESSAGE;

	static const int PROGRESS_UNDEFINED = -1;

	static const int DOWNLOAD_CHUNK = 16384;

	wxEvtHandler * mEventHandler;
	SyncType mSyncType;
	HINTERNET mInternetHandle;
	Status mStatus;
	wxCriticalSection mStatusLock;
	bool mCancel;
	wxCriticalSection mCancelLock;
	const Ets2::Save * mSave;
	Ets2::Save::DlcList mRefusedDlcs;
	int mJobList;

	void setStatus(int flags, State state, int progress, wxString message);
	wxThread::ExitCode Entry();
	bool getJobs(Ets2::Save::JobList& jobs);
};
