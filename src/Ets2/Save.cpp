#include "precomp.hpp"
#include "Save.hpp"
#include "File.hpp"
#include "Info.hpp"
#include "lib/file.hpp"
#include "lib/Utf8ToUtf16.hpp"

namespace Ets2 {
	const std::wstring Save::SII_BASENAME = L"info.sii";
	const std::wstring Save::SAVE_BASENAME = L"game.sii";
	const std::string Save::NAME_ATTRIBUTE = "name";
	const std::string Save::SAVE_TIME_ATTRIBUTE = "file_time";
	const std::string Save::DEPEND_ATTRIBUTE = "dependencies[";
	const std::string Save::ECONOMY_UNIT = "economy";
	const std::string Save::GAME_TIME_ATTRIBUTE = "game_time";
	const std::string Save::COMPANY_UNIT = "company";
	const std::string Save::JOB_UNIT = "job_offer_data";
	const std::string Save::COMPANY_NAME_PREFIX = "company.volatile.";

	Save::Save(Game game, const std::wstring directory)
		: Object(SII_BASENAME) {
		//wxStopWatch initTime;
		mDlcs.clear();
		mGame = game;
		//DEBUG_LOG(L"%s: Initializing", directory);
		init(directory);
		//DEBUG_LOG(L"%s: Initialized in %lld µs", mName, initTime.TimeInMicro());
	};

	const Save::DlcList& Save::getDlcs() const {
		return mDlcs;
	}

	void Save::processAttribute(Parser::Sii::Context context, const std::string& attribute, const std::string& value) {
		if (context != Parser::Sii::Context::ATTRIBUTE) {
			return;
		}
		//DEBUG_LOG(L"%s: %s", attribute, value);
		if (mName.empty() && attribute == NAME_ATTRIBUTE) {
			Utf8ToUtf16(value.data(), value.length(), mName);
		} else if (mSaveTime == 0 && attribute == SAVE_TIME_ATTRIBUTE) {
			try {
				mSaveTime = std::stoll(value);
			} catch (std::invalid_argument) {
				mSaveTime = 0;
			}
		} else if (attribute.find(DEPEND_ATTRIBUTE) != std::string::npos) {
			std::wstring wideValue;
			Utf8ToUtf16(value.data(), value.length(), wideValue);
			//DEBUG_LOG(L"Dependency: %ls", wideValue);
			size_t dlcNameStart = std::wstring::npos;
			size_t dlcNameEnd = std::wstring::npos;
			if (wideValue.find(L"dlc|") == 0) {
				dlcNameStart = 4;
			} else if (wideValue.find(L"rdlc|") == 0) {
				dlcNameStart = 5;
			}
			if (dlcNameStart != std::wstring::npos) {
				if (wideValue.find(L"eut2_", dlcNameStart) == dlcNameStart) {
					dlcNameStart += 5;
				} else if (wideValue.find(L"ats_", dlcNameStart) == dlcNameStart) {
					dlcNameStart += 4;
				}
				dlcNameEnd = wideValue.find(L'|', dlcNameStart);
				mDlcs.push_back(wideValue.substr(dlcNameStart, dlcNameEnd == std::wstring::npos ? std::wstring::npos : dlcNameEnd - dlcNameStart));
				DEBUG_LOG(L"%ls: DLC: %ls", mName, mDlcs.at(mDlcs.size() - 1));
			}
		}
	}

	bool Save::validate() {
		return true;
	}

	void Save::setupBlankJob(Job& job) {
		job.target = "";
		job.urgency = -1;
		job.distance = 0;
		job.ferryTime = 0;
		job.ferryPrice = 0;
		job.cargo = "null";
		job.companyTruck = "";
		job.trailerVariant = "null";
		job.trailerDefinition = "null";
		job.unitsCount = 0;
		job.fillRatio = 1;
		job.trailerPlace = 0;
	}

	// Returns the number of jobs inserted in the save, or -1 in case of error.
	int Save::replaceJobList(const JobList& jobs, const std::function<bool(int progress)>& callback) const {
		if (!callback(0)) {
			return -1;
		}
		std::wstring fileName = mDirectory + L"\\" + SAVE_BASENAME;
		std::string data;
		long gameTime = -1;
		if (!File::read(fileName, data)) {
			DEBUG_LOG(L"Error reading save file '%s'", fileName);
			return -1;
		}
		// Save the original save, unencrypted
		write_file(data, fileName + L".raw.txt");
		//exit(0);
		std::string currentCompany;
		Job blankJob;
		setupBlankJob(blankJob);
		size_t companyJobIndex;
		std::vector<Job> emptyJobList;
		const std::vector<Job> * companyJobs = nullptr;
		const Job * currentJob = nullptr;

		long currentLine = 0;
		std::string newSaveData = "SiiNunit\r\n{";
		newSaveData.reserve(20 * 1024 * 1024);
		bool inEconomy = false;
		bool inJob = false;
		bool hasGameTime = false;
		bool newLineHasValue = false;
		unsigned long lastOffset = 0;
		int lastProgress = -1;
		int progress = 0;
		int jobsAdded = 0;
		if (data.empty()) {
			DEBUG_LOG("Save file is empty. Aborting...");
			return -1;
		}
		size_t dataLength = data.length();
		wxStopWatch sw;
		sw.Start(0);
		bool parseSuccess = File::parse(data, [&](const Parser::Sii::Context context, const std::string& name, const std::string& value, const std::string& sourceValue, unsigned long offset) -> bool {
			//DEBUG_LOG("context: %d, name: \"%s\", value: \"%s\", sourceValue: \"%s\", offset: %d", context, name, value, sourceValue, offset);
			lastOffset = offset;
			if (context == Parser::Sii::Context::UNIT_START) {
				newSaveData.append("\r\n").append(name).append(" : ").append(value).append(" {\r\n");
				if (!hasGameTime && name == ECONOMY_UNIT) {
					inEconomy = true;
				} else if (name == COMPANY_UNIT) {
					if (value.find(COMPANY_NAME_PREFIX) == 0) {
						currentCompany.assign(value, COMPANY_NAME_PREFIX.length(), value.length() - COMPANY_NAME_PREFIX.length());
						//DEBUG_LOG(L"Current company: %s", currentCompany);
						auto r = jobs.find(currentCompany);
						if (r == jobs.end()) {
							companyJobs = &emptyJobList;
							//DEBUG_LOG("No jobs...");
						} else {
							companyJobs = &r->second;
							//DEBUG_LOG("Got %d jobs!", companyJobs->size());
						}
						companyJobIndex = 0;
					} else {
						companyJobs = &emptyJobList;
						//DEBUG_LOG("Company doesn't start with the right prefix: '%s'", value);
					}
				} else if (name == JOB_UNIT) {
					inJob = true;
					if (companyJobIndex < companyJobs->size()) {
						currentJob = &(*companyJobs)[companyJobIndex];
						//DEBUG_LOG("Existing job.");
					} else {
						currentJob = &blankJob;
						//DEBUG_LOG("Blank job.");
					}
					//DEBUG_LOG("Current job: %d (%p) - %s to %s", companyJobIndex, currentJob, currentJob->cargo, currentJob->target);
				}
			} else if (context == Parser::Sii::Context::UNIT_END) {
				inEconomy = false;
				if (inJob) {
					++companyJobIndex;
					inJob = false;
				}
				newSaveData.append("}\r\n");
			} else {
				if (!inJob || name.find("trailer_place[") != 0) {
					newSaveData.append(1, ' ').append(name).append(": ");
				}
				newLineHasValue = false;
				if (inEconomy && name == GAME_TIME_ATTRIBUTE) {
					gameTime = std::stol(value);
					hasGameTime = true;
					//DEBUG_LOG(L"Game time: %ld", gameTime);
				} else if (inJob) {
					if (name == "cargo") {
						newSaveData.append(currentJob->cargo);
						newLineHasValue = true;
						if (currentJob != &blankJob) {
							jobsAdded++;
						}
					} else if (name == "company_truck") {
						bool needsQuotes = (currentJob->companyTruck.empty() || currentJob->companyTruck.find_first_of('/') != std::string::npos);
						if (needsQuotes) {
							newSaveData.append("\"");
						}
						newSaveData.append(currentJob->companyTruck);
						if (needsQuotes) {
							newSaveData.append("\"");
						}
						newLineHasValue = true;
					} else if (name == "trailer_variant") {
						newSaveData.append(currentJob->trailerVariant);
						newLineHasValue = true;
					} else if (name == "trailer_definition") {
						newSaveData.append(currentJob->trailerDefinition);
						newLineHasValue = true;
					} else if (name == "target") {
						newSaveData.append("\"").append(currentJob->target).append("\"");
						newLineHasValue = true;
					} else if (name == "expiration_time") {
						if (!hasGameTime) {
							DEBUG_LOG(L"Jobs started but game time was not found yet.");
							return false;
						}
						newSaveData.append(currentJob == &blankJob ? "nil" : std::to_string(gameTime + 30000));
						newLineHasValue = true;
					} else if (name == "urgency") {
						newSaveData.append(currentJob->urgency == -1 ? "nil" : std::to_string(currentJob->urgency));
						newLineHasValue = true;
					} else if (name == "shortest_distance_km") {
						newSaveData.append(std::to_string(currentJob->distance));
						newLineHasValue = true;
					} else if (name == "ferry_time") {
						newSaveData.append(std::to_string(currentJob->ferryTime));
						newLineHasValue = true;
					} else if (name == "ferry_price") {
						newSaveData.append(std::to_string(currentJob->ferryPrice));
						newLineHasValue = true;
					} else if (name == "trailer_place") {
						newSaveData.append(std::to_string(currentJob->trailerPlace));
						newLineHasValue = true;
					} else if (name == "units_count") {
						newSaveData.append(std::to_string(currentJob->unitsCount));
						newLineHasValue = true;
					} else if (name == "fill_ratio") {
						newSaveData.append(std::to_string(currentJob->fillRatio));
						newLineHasValue = true;
					}
				}
				if (name.find("trailer_place[") != 0) {
					if (!newLineHasValue) {
						newSaveData.append(sourceValue);
					}
					newSaveData.append("\r\n");
				}
			}

			progress = offset / (dataLength / 100);
			if (progress != lastProgress) {
				//DEBUG_LOG("offset: %8lu dataLength: %8lu", offset * 100, dataLength);
				if (!callback(progress)) {
					return false;
				}
				lastProgress = progress;
			}
			return true;
		});
		if (!parseSuccess) {
			DEBUG_LOG(L"Error parsing save file: '%s' (offset 0x%08x)", fileName, currentLine, lastOffset);
			return -1;
		}
		newSaveData.append("\r\n}\r\n");
		//throw(std::runtime_error("Aborting..."));
		try {
			write_file(newSaveData, fileName);
			write_file(newSaveData, fileName + L".synced.txt");
		} catch (int e) {
			DEBUG_LOG(L"Error writing save file: '%s' (errno: %d)", e);
			return -1;
		}
		DEBUG_LOG(L"Replaced jobs in %ld ms", sw.Time());
		return jobsAdded;
	}
}