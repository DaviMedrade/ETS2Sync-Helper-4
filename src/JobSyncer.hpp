#pragma once

#include "Ets2\Save.hpp"
#include <wininet.h>

wxDECLARE_EVENT(EVT_JOB_SYNCER_UPDATE, wxCommandEvent);

#define SYNC_DEBUG_LOG(fmt, ...) DEBUG_LOG(wxString(L"Save \"%s\": ")+wxString(fmt), mSave == nullptr ? L"null" : mSave->getName(), __VA_ARGS__)

class JobSyncer : public wxEvtHandler, public wxThreadHelper {
public:
	JobSyncer(wxEvtHandler * eventHandler, bool clearJobs);
	~JobSyncer();

	enum STATE {
		STATE_NOT_STARTED,
		STATE_STARTING,
		STATE_DOWNLOADING,
		STATE_INSERTING_JOBS,
		STATE_FINISHED,
		STATE_CANCELED,
		STATE_ERROR
	};

	struct Status {
		STATE state;
		int progress;
		wxString message;
	};

	void start(const Ets2::Save * save, int dlcs);
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
	bool mClearJobs;
	HINTERNET mInternetHandle;
	Status mStatus;
	wxCriticalSection mStatusLock;
	bool mCancel;
	wxCriticalSection mCancelLock;
	const Ets2::Save * mSave;
	int mDlcs;

	void setStatus(int flags, STATE state, int progress, wxString message);
	wxThread::ExitCode Entry();
	bool getJobs(Ets2::Save::JobList& jobs);
};
