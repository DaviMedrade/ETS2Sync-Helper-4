#pragma once

#include <wininet.h>
#include "ShowLastWindowsError.hpp"

wxDECLARE_EVENT(EVT_UPDATE_CHECKER, wxCommandEvent);

class UpdateChecker : public wxEvtHandler, public wxThreadHelper {
public:
	// Update check possible states
	enum class State {
		UNKNOWN,
		RUNNING,
		FINISHED,
		CANCELED,
		FAILED
	};
	// Update check possible results
	enum class Result {
		UNKNOWN,
		UP_TO_DATE,
		NEW_BUGFIX,
		NEW_VERSION
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
