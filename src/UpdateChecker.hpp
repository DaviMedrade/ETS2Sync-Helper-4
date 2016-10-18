#pragma once

#include <wininet.h>
#include "ShowLastWindowsError.hpp"

wxDECLARE_EVENT(EVT_UPDATE_CHECKER, wxCommandEvent);

class UpdateChecker : public wxEvtHandler, public wxThreadHelper {
public:
	// Update check possible states
	enum State {
		STATE_UNKNOWN,
		STATE_RUNNING,
		STATE_FINISHED,
		STATE_CANCELED,
		STATE_ERROR
	};
	// Update check possible results
	enum Result {
		RESULT_UNKNOWN,
		RESULT_UP_TO_DATE,
		RESULT_NEW_BUGFIX,
		RESULT_NEW_VERSION
	};
	struct Status {
		bool cancel;
		State state;
		Result result;
	};

	static std::wstring getStatusDescription(Status state);

	UpdateChecker(wxEvtHandler * eventHandler);
	~UpdateChecker();
	void start();
	void cancel();
	Status getStatus();

private:
	// Flags determining which fields of the status are to be set
	static const int SET_CANCEL = 0x01;
	static const int SET_STATE = 0x02;
	static const int SET_RESULT = 0x04;
	static const int SET_ALL = 0x7;

	void setStatus(int flags, bool cancel, State state, Result result);
	void sendUpdate();
	wxThread::ExitCode Entry();

	wxEvtHandler * mEventHandler;
	Status mStatus;
	wxCriticalSection mStatusLock;
	HINTERNET mInternetHandle;
};
