#include "precomp.hpp"
#include "UpdateChecker.hpp"

#include "version.hpp"

wxDEFINE_EVENT(EVT_UPDATE_CHECKER, wxCommandEvent);

std::wstring UpdateChecker::getStatusDescription(Status status) {
	switch (status.state) {
	case STATE_RUNNING:
		return L"Checking for new versions…";
	case STATE_CANCELED:
		return L"Check for new versions canceled.";
	case STATE_ERROR:
		return L"Error checking for new versions.";
	case STATE_FINISHED:
		switch (status.result) {
		case RESULT_NEW_VERSION:
			return L"A newer version is available.";
		case RESULT_NEW_BUGFIX:
			return L"A bugfix version is available";
		case RESULT_UP_TO_DATE:
			return L"Your version is up-to-date.";
		}
		return L"Unknown result.";
	}
	return L"Unknown state.";
}

UpdateChecker::UpdateChecker(wxEvtHandler * eventHandler) {
	mEventHandler = eventHandler;

	mInternetHandle = InternetOpen(APP_NAME + "/" + APP_VERSION, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (mInternetHandle == NULL) {
		ShowLastWindowsError(L"InternetOpen");
	}
}

UpdateChecker::~UpdateChecker() {
	cancel();
	if (mInternetHandle && !InternetCloseHandle(mInternetHandle)) {
		ShowLastWindowsError(L"InternetCloseHandle");
	}
}

UpdateChecker::Status UpdateChecker::getStatus() {
	wxCriticalSectionLocker locker(mStatusLock);
	return Status(mStatus);
}

void UpdateChecker::start() {
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
		wxLogError("Error checking for new versions\n\nCould not create worker thread.");
		setStatus(SET_ALL, false, STATE_ERROR, RESULT_UNKNOWN);
		return;
	}
	if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
		wxLogError("Error checking for new versions\n\nCould not run worked thread.");
		setStatus(SET_ALL, false, STATE_ERROR, RESULT_UNKNOWN);
		return;
	}
	setStatus(SET_ALL, false, STATE_RUNNING, RESULT_UNKNOWN);
}

void UpdateChecker::cancel() {
	wxThread * thread = GetThread();
	if (thread && thread->IsRunning()) {
		setStatus(SET_CANCEL, true, STATE_UNKNOWN, RESULT_UNKNOWN);
		thread->Wait();
	}
}

void UpdateChecker::setStatus(int flags, bool cancel, State state, Result result) {
	wxCriticalSectionLocker locker(mStatusLock);
	if (flags & SET_CANCEL)
		mStatus.cancel = cancel;
	if (flags & SET_STATE)
		mStatus.state = state;
	if (flags & SET_RESULT)
		mStatus.result = result;

	// the cancel flag is meant only to notify the thread; the thread will set the status again once it's done
	if (flags != SET_CANCEL) 
		sendUpdate();
}

void UpdateChecker::sendUpdate() {
	QueueEvent(new wxCommandEvent(EVT_UPDATE_CHECKER));
}

wxThread::ExitCode UpdateChecker::Entry() {
	HINTERNET urlHandle = InternetOpenUrl(mInternetHandle, APP_URL_CHECK_VERSION.data(), NULL, (DWORD)-1,
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_COOKIES, (DWORD_PTR)this);
	if (urlHandle == NULL) {
		ShowLastWindowsError(L"InternetOpenUrl");
		setStatus(SET_STATE, false, STATE_ERROR, RESULT_UNKNOWN);
		return (wxThread::ExitCode)1;
	}

	bool bError = false;
	bool bCanceled = false;
	std::vector<char> vcData(0);
	DWORD dwPosition = 0;

	DWORD httpStatus = 0;
	DWORD httpStatusSize = sizeof(DWORD);
	if (!HttpQueryInfo(urlHandle, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &httpStatus, &httpStatusSize, NULL)) {
		ShowLastWindowsError(L"HttpQueryInfo (1)");
		bError = true;
	} else if (httpStatus != 200) {
		bError = true;
		std::vector<wchar_t> statusMessage(1);
		DWORD httpStatusMessageSize = 1;
		HttpQueryInfo(urlHandle, HTTP_QUERY_STATUS_TEXT, statusMessage.data(), &httpStatusMessageSize, NULL);
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) { // as expected...
			statusMessage.resize(httpStatusMessageSize + 1);
			if (!HttpQueryInfo(urlHandle, HTTP_QUERY_STATUS_TEXT, statusMessage.data(), &httpStatusMessageSize, NULL))
				ShowLastWindowsError(L"HttpQueryInfo (2)");
			else {
				statusMessage[httpStatusMessageSize] = 0;
				wxLogError("Error checking for new versions\n\nHTTP %u %s", httpStatus, statusMessage.data());
			}
		} else {
			ShowLastWindowsError(L"HttpQueryInfo (3)");
		}
	} else {
		Status status;
		while (true) {
			status = getStatus();
			if (status.cancel) {
				bCanceled = true;
				break;
			}

			DWORD dwOffset = 0;
			if (vcData.size() < dwPosition + 4096)
				vcData.resize(dwPosition + 4096);
			if (!InternetReadFile(urlHandle, &vcData[dwPosition], 4096, &dwOffset)) {
				ShowLastWindowsError(L"InternetReadFile");
				bError = true;
				break;
			}
			if (dwOffset == 0) {
				vcData.resize(dwPosition);
				break;
			}
			dwPosition += dwOffset;
			GetThread()->Sleep(200);
		}
	}
	if (!InternetCloseHandle(urlHandle)) {
		ShowLastWindowsError(L"InternetCloseHandle");
		bError = true;
	}
	if (bError || bCanceled) {
		setStatus(SET_STATE, false, bCanceled ? STATE_CANCELED : STATE_ERROR, RESULT_UNKNOWN);
	} else {
		Result result = RESULT_UNKNOWN;
		std::wstring resultStr;
		resultStr.assign(vcData.begin(), vcData.end());
		if (resultStr == L"outdated") {
			result = RESULT_NEW_VERSION;
		} else if (resultStr == L"bugfix") {
			result = RESULT_NEW_BUGFIX;
		} else if (resultStr == L"current") {
			result = RESULT_UP_TO_DATE;
		}
		setStatus(SET_STATE | SET_RESULT, false, (result == RESULT_UNKNOWN) ? STATE_ERROR : STATE_FINISHED, result);
	}
	return (wxThread::ExitCode)0;
}
