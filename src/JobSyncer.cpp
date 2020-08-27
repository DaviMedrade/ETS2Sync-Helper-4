#include "precomp.hpp"
#include "JobSyncer.hpp"

#include "Ets2/Info.hpp"
#include "ShowLastWindowsError.hpp"
#include "version.hpp"

#include <map>

wxDEFINE_EVENT(EVT_JOB_SYNCER_UPDATE, wxCommandEvent);

JobSyncer::JobSyncer(wxEvtHandler * eventHandler, SyncType syncType) {
	mEventHandler = eventHandler;
	mSyncType = syncType;
	mInternetHandle = InternetOpen(APP_NAME + "/" + APP_VERSION, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (mInternetHandle == NULL) {
		ShowLastWindowsError(L"InternetOpen");
	}
	mStatus.state = State::NOT_STARTED;
	mStatus.progress = PROGRESS_UNDEFINED;
	mStatus.message = wxEmptyString;
	mSave = nullptr;
}

JobSyncer::~JobSyncer() {
	if (mInternetHandle && !InternetCloseHandle(mInternetHandle)) {
		ShowLastWindowsError(L"InternetCloseHandle");
	}
}

void JobSyncer::start(const Ets2::Save * save, const Ets2::Save::DlcList& dlcs, int jobList) {
	Status status = getStatus();
	if (status.state >= State::STARTING && status.state < State::FINISHED) {
		// Don't use SYNC_DEBUG_LOG because mSave is the save from the sync that's already running
		DEBUG_LOG(L"Can't start sync: sync already in progress.");
		// leave the status alone since another sync is running
		return;
	}
	mSave = save;
	mDlcs = dlcs;
	mJobList = jobList;
	if (mSave == nullptr) {
		SYNC_DEBUG_LOG(L"Can't start sync: save is null.");
		setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"No save selected.");
		return;
	}
	SYNC_DEBUG_LOG(L"Starting sync.");
	setStatus(SET_ALL, State::STARTING, PROGRESS_UNDEFINED, L"Starting sync…");
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
		SYNC_DEBUG_LOG(L"Could not create sync thread.");
		setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"The sync thread could not be created.");
		return;
	}
	if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
		SYNC_DEBUG_LOG(L"Could not run sync thread.");
		setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"The sync thread could not be started.");
		return;
	}
}

const JobSyncer::Status JobSyncer::getStatus() {
	Status ret;
	wxCriticalSectionLocker lock(mStatusLock);
	ret.state = mStatus.state;
	ret.progress = mStatus.progress;
	ret.message = mStatus.message;
	return ret;
}

void JobSyncer::cancel() {
	SYNC_DEBUG_LOG(L"Sync cancel requested.");
	wxCriticalSectionLocker lock(mCancelLock);
	mCancel = true;
}

void JobSyncer::setStatus(int flags, State state, int progress, wxString message) {
	wxCriticalSectionLocker lock(mStatusLock);
	if (flags & SET_STATE) {
		mStatus.state = state;
	}
	if (flags & SET_PROGRESS) {
		mStatus.progress = progress;
	}
	if (flags & SET_MESSAGE) {
		mStatus.message = message;
	}
	mEventHandler->QueueEvent(new wxCommandEvent(EVT_JOB_SYNCER_UPDATE));
}

wxThread::ExitCode JobSyncer::Entry() {
	SYNC_DEBUG_LOG(L"Sync thread started.");

	{
		wxCriticalSectionLocker locker(mCancelLock);
		mCancel = false;
	}

	Ets2::Save::JobList jobs;
	if (mSyncType == SyncType::SYNC) {
		if (!getJobs(jobs)) {
			{
				wxCriticalSectionLocker locker(mCancelLock);
				if (mCancel) {
					SYNC_DEBUG_LOG(L"Sync cancelled while getting job list.");
				} else {
					SYNC_DEBUG_LOG(L"Could not get the job list.");
				}
			}
			return (wxThread::ExitCode)1;
		}
	} else if (mSyncType == SyncType::RESET_ECONOMY) {
		Ets2::Save::Job job;
		job.cargo = "invalid";
		job.companyTruck = "invalid";
		job.trailerDefinition = "invalid";
		job.trailerVariant = "invalid";
		job.target = "invalid.invalid";
		job.urgency = 0;
		job.distance = 1000;
		job.ferryPrice = 1000;
		job.ferryTime = 100;
		job.trailerPlace = {};
		job.unitsCount = 0;
		job.fillRatio = 1;
		jobs.clear();
		jobs[mSave->getGame() == Ets2::Game::ETS2 ? "sanbuilders.hamburg" : "vm_car_dlr.las_vegas"].push_back(job);
	}

	setStatus(SET_STATE | SET_MESSAGE, State::INSERTING_JOBS, 0, L"");

	int result = mSave->replaceJobList(jobs, [&](int progress) -> bool {
		{
			wxCriticalSectionLocker locker(mCancelLock);
			if (mCancel) {
				setStatus(SET_ALL, State::CANCELED, PROGRESS_UNDEFINED, L"Sync canceled.");
				return false;
			}
		}
		setStatus(SET_ALL, State::INSERTING_JOBS, progress, wxString::Format(L"%d%% processed.", progress));
		//wxSafeYield();
		return true;
	});
	if (getStatus().state == State::INSERTING_JOBS) {
		if (result != -1) {
			setStatus(SET_ALL, State::FINISHED, result, L"Sync complete.");
		} else {
			setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"Error inserting jobs into the save.");
		}
	}

	SYNC_DEBUG_LOG(L"Sync thread finished.");
	return (wxThread::ExitCode)0;
}

bool JobSyncer::getJobs(Ets2::Save::JobList& jobs) {
	setStatus(SET_ALL, State::DOWNLOADING, PROGRESS_UNDEFINED, L"Connecting to the server…");

	std::wstring syncUrl = APP_URL_SYNC;
	std::wstring gameParam = L"";
	std::wstring dlcsParam = L"";
	if (mSave->getGame() == Ets2::Game::ATS) {
		gameParam = L"ats";
	} else {
		gameParam = L"ets2";
	}
	for (auto&& dlc : mDlcs) {
		if (!dlcsParam.empty())
			dlcsParam.push_back(L',');
		dlcsParam.append(dlc);
	}
	if (mJobList != -1) {
		syncUrl += L"&list=" + std::to_wstring(mJobList);
	}
	syncUrl += L"&game=" + gameParam + L"&dlcs=" + dlcsParam;

	SYNC_DEBUG_LOG(L"Downloading from URL: %s", syncUrl);
	HINTERNET urlHandle = InternetOpenUrl(mInternetHandle, syncUrl.data(), NULL, (DWORD)-1,
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_COOKIES, (DWORD_PTR)this);
	if (urlHandle == NULL) {
		ShowLastWindowsError(L"InternetOpenUrl");
		setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"Could not download the job list.");
		return false;
	}

	std::string data;
	do {
		DWORD httpStatus = 0;
		DWORD httpStatusSize = sizeof(DWORD);
		if (!HttpQueryInfo(urlHandle, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &httpStatus, &httpStatusSize, NULL)) {
			SYNC_DEBUG_LOG(L"Error getting the HTTP status.");
			ShowLastWindowsError(L"HttpQueryInfo (1)");
			setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"Could not download the job list.");
			break;
		}
		if (httpStatus == 426) {
			setStatus(SET_ALL, State::OUTDATED, PROGRESS_UNDEFINED, L"Outdated client. Please update to a newer version.");
			break;
		} else if (httpStatus != 200) {
			SYNC_DEBUG_LOG(L"HTTP status: %d", httpStatus);
			std::vector<wchar_t> statusMessage(1);
			DWORD httpStatusMessageSize = 1;
			HttpQueryInfo(urlHandle, HTTP_QUERY_STATUS_TEXT, statusMessage.data(), &httpStatusMessageSize, NULL);
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) { // as expected...
				statusMessage.resize(httpStatusMessageSize + 1);
				if (!HttpQueryInfo(urlHandle, HTTP_QUERY_STATUS_TEXT, statusMessage.data(), &httpStatusMessageSize, NULL)) {
					ShowLastWindowsError(L"HttpQueryInfo (2)");
					setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED,
						wxString::Format(L"Could not download the job list. HTTP %d <error getting status message>", httpStatus));
				} else {
					statusMessage[httpStatusMessageSize] = 0;
					setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED,
						wxString::Format(L"Could not download the job list. HTTP %d %s", httpStatus, statusMessage.data()));
				}
			} else {
				ShowLastWindowsError(L"HttpQueryInfo (3)");
				setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED,
					wxString::Format(L"Could not download the job list. HTTP %d <error getting status message>", httpStatus));
			}
			break;
		}

		DWORD downloadTotal = 0;
		DWORD downloadTotalSize = sizeof(DWORD);
		if (!HttpQueryInfo(urlHandle, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &downloadTotal, &downloadTotalSize, 0)) {
			SYNC_DEBUG_LOG(L"Could not retrieve content length.");
			downloadTotal = 0;
		}

		Status status;
		DWORD position = 0;
		double progress = PROGRESS_UNDEFINED;
		wxString message;
		wxDateTime chunkStartTime = wxDateTime::UNow();
		wxDateTime downloadStartTime = chunkStartTime;
		wxTimeSpan chunkTime;
		wxLongLong kbps = 0;
		wxLongLong chunkSize = 0;
		while (true) {
			SYNC_DEBUG_LOG(L"Getting chunk.");
			{
				wxCriticalSectionLocker locker(mCancelLock);
				if (mCancel) {
					SYNC_DEBUG_LOG(L"Sync canceled.");
					setStatus(SET_ALL, State::CANCELED, PROGRESS_UNDEFINED, L"Sync canceled.");
					break;
				}
			}

			DWORD offset = 0;
			if (data.size() < position + DOWNLOAD_CHUNK) {
				data.resize(position + DOWNLOAD_CHUNK);
			}
			if (!InternetReadFile(urlHandle, &data[position], DOWNLOAD_CHUNK, &offset)) {
				ShowLastWindowsError(L"InternetReadFile");
				setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"Could not download the job list.");
				break;
			}
			if (offset == 0) {
				data.resize(position);
				break;
			}
			position += offset;
			chunkSize += offset;
			chunkTime = wxDateTime::UNow() - chunkStartTime;
			if (chunkTime.GetSeconds() > 0) {
				kbps = chunkSize / chunkTime.GetSeconds() / 1024;
				chunkStartTime = wxDateTime::UNow();
				chunkSize = 0;
			}

			if (downloadTotal > 0) {
				progress = position / (double)downloadTotal * 100.0;
				message = wxString::Format(L"Downloaded %d kB of %d kB%s", position / 1024, downloadTotal / 1024, kbps == 0 ? L"" : wxString::Format(L" — %lld kB/s.", kbps));
			} else {
				message = wxString::Format(L"Downloaded %d kB%s", position / 1024, kbps == 0 ? L"" : wxString::Format(L" — %lld kB/s.", kbps));
			}
			setStatus(SET_PROGRESS | SET_MESSAGE, State::DOWNLOADING, progress, message);
			//GetThread()->Sleep(500);
		}
		SYNC_DEBUG_LOG(L"Download complete. %d byte(s) in %lld ms.", data.size(), (wxDateTime::UNow() - downloadStartTime).GetMilliseconds());
	} while (false);

	if (!InternetCloseHandle(urlHandle)) {
		ShowLastWindowsError(L"InternetCloseHandle");
		setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, L"Could not download the job list.");
		return false;
	}

	{
		Status status = getStatus();
		if (status.state != State::DOWNLOADING) {
			return false;
		}
	}

	using namespace rapidjson;
	Document json;
	wxStopWatch sws;
	json.ParseInsitu(&data[0]);
	if (json.HasParseError()) {
		setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, wxString::Format(L"Job list error at %u: %s", json.GetErrorOffset(), GetParseError_En(json.GetParseError())).ToStdWstring());
		return false;
	}
		
	std::string propName;
	std::string propValue;
	for (Value::ConstMemberIterator keyIterator = json.MemberBegin(); keyIterator != json.MemberEnd(); ++keyIterator) {
		for (Value::ConstValueIterator jobIterator = keyIterator->value.Begin(); jobIterator != keyIterator->value.End(); ++jobIterator) {
			Ets2::Save::Job job;
			Ets2::Save::setupBlankJob(job);
			for (Value::ConstMemberIterator propIterator = jobIterator->MemberBegin(); propIterator != jobIterator->MemberEnd(); ++propIterator) {
				propName.assign(propIterator->name.GetString(), propIterator->name.GetStringLength());
				if (propName == "cargo" || propName == "company_truck" || propName == "target" || propName == "trailer_variant" || propName == "trailer_definition") {
					propValue.assign(propIterator->value.GetString(), propIterator->value.GetStringLength());
					if (propName == "cargo") {
						job.cargo = propValue;
					} else if (propName == "company_truck") {
						job.companyTruck = propValue;
					} else if (propName == "target") {
						job.target = propValue;
					} else if (propName == "trailer_variant") {
						job.trailerVariant = propValue;
					} else if (propName == "trailer_definition") {
						job.trailerDefinition = propValue;
					}
				} else if (propName == "urgency" || propName == "shortest_distance_km" || propName == "ferry_time" || propName == "ferry_price" || propName == "units_count" || propName == "fill_ratio" || propName == "trailer_place") {
					int propValueInt = propIterator->value.GetInt();
					if (propName == "urgency") {
						job.urgency = propValueInt;
					} else if (propName == "shortest_distance_km") {
						job.distance = propValueInt;
					} else if (propName == "ferry_time") {
						job.ferryTime = propValueInt;
					} else if (propName == "ferry_price") {
						job.ferryPrice = propValueInt;
					} else if (propName == "units_count") {
						job.unitsCount = propValueInt;
					} else if (propName == "fill_ratio") {
						job.fillRatio = propValueInt;
					} else if (propName == "trailer_place") {
						job.trailerPlace = propValueInt;
					}
				} else {
					SYNC_DEBUG_LOG(L"unknown job property: '%s'", propName);
					setStatus(SET_ALL, State::FAILED, PROGRESS_UNDEFINED, wxString::Format(L"Unknown remote job property: %s", propName).ToStdWstring());
					return false;
				}
			}
			jobs[keyIterator->name.GetString()].push_back(job);
		}
	}
	SYNC_DEBUG_LOG(L"Parsed jobs in %d ms.", sws.Time());

	return true;
}