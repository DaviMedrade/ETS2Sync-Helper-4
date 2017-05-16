#include "precomp.hpp"
#include "Save.hpp"
#include "File.hpp"
#include "lib/file.hpp"
#include "lib/Utf8ToUtf16.hpp"

namespace Ets2 {
	const std::wstring Save::SII_BASENAME = L"info.sii";
	const std::wstring Save::SAVE_BASENAME = L"game.sii";
	const std::string Save::NAME_ATTRIBUTE = "name";
	const std::string Save::SAVE_TIME_ATTRIBUTE = "file_time";
	const std::string Save::DEPEND_ATTRIBUTE = "dependencies[";
	const std::wstring Save::DEPEND_SCANDINAVIA = L"dlc|eut2_north|";
	const std::wstring Save::DEPEND_GOINGEAST = L"dlc|eut2_east|";
	const std::wstring Save::DEPEND_HIGHPOWERCARGO = L"rdlc|eut2_trailers|";
	const std::wstring Save::DEPEND_FRANCE = L"dlc|eut2_fr|";
	const std::string Save::ECONOMY_UNIT = "economy";
	const std::string Save::GAME_TIME_ATTRIBUTE = "game_time";
	const std::string Save::COMPANY_UNIT = "company";
	const std::string Save::JOB_UNIT = "job_offer_data";
	const std::string Save::COMPANY_NAME_PREFIX = "company.volatile.";

	Save::Save(const std::wstring directory)
		: Object(SII_BASENAME) {
		//wxStopWatch initTime;
		mDlcs = 0;
		init(directory);
		//DEBUG_LOG(L"%s: Initialized in %lld µs", mName, initTime.TimeInMicro());
	};

	void Save::setGame(Game game) {
		mGame = game;
	}

	int Save::getDlcs() const {
		return mDlcs;
	}

	void Save::processAttribute(Parser::Sii::Context context, const std::string& attribute, const std::string& value) {
		if (context != Parser::Sii::Context::ATTRIBUTE) {
			return;
		}
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
			if (wideValue.find(DEPEND_SCANDINAVIA) == 0) {
				mDlcs |= DLC_SCANDINAVIA;
			} else if (wideValue.find(DEPEND_GOINGEAST) == 0) {
				mDlcs |= DLC_GOINGEAST;
			} else if (wideValue.find(DEPEND_HIGHPOWERCARGO) == 0) {
				mDlcs |= DLC_HIGHPOWERCARGO;
			} else if (wideValue.find(DEPEND_FRANCE) == 0) {
				mDlcs |= DLC_FRANCE;
			}
		}
	}

	bool Save::validate() {
		return true;
	}

	bool Save::replaceJobList(const JobList& jobs, const std::function<bool(int progress)>& callback) const {
		if (!callback(0)) {
			return false;
		}
		std::wstring fileName = mDirectory + L"\\" + SAVE_BASENAME;
		std::string data;
		long gameTime = -1;
		if (!File::read(fileName, data)) {
			DEBUG_LOG(L"Error reading save file '%s'", fileName);
			return false;
		}
		// Save the original save, unencrypted
		write_file(data, fileName + L".raw.txt");
		//exit(0);
		std::string currentCompany;
		Job blankJob;
		blankJob.cargo = "null";
		blankJob.companyTruck = "";
		blankJob.variant = -1;
		blankJob.target = "";
		blankJob.urgency = -1;
		blankJob.distance = 0;
		blankJob.ferryPrice = 0;
		blankJob.ferryTime = 0;
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
		if (data.empty()) {
			DEBUG_LOG("Save file is empty. Aborting...");
			return false;
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
				newSaveData.append(1, ' ').append(name).append(": ");
				newLineHasValue = false;
				if (inEconomy && name == GAME_TIME_ATTRIBUTE) {
					gameTime = std::stol(value);
					hasGameTime = true;
					//DEBUG_LOG(L"Game time: %ld", gameTime);
				} else if (inJob) {
					if (name == "cargo") {
						newSaveData.append(currentJob->cargo);
						newLineHasValue = true;
					} else if (name == "company_truck") {
						bool needsQuotes = (currentJob->companyTruck.empty() || currentJob->companyTruck.find_first_of('/') == std::string::npos);
						if (needsQuotes) {
							newSaveData.append("\"");
						}
						newSaveData.append(currentJob->companyTruck);
						if (needsQuotes) {
							newSaveData.append("\"");
						}
						newLineHasValue = true;
					} else if (name == "variant") {
						newSaveData.append(currentJob->variant == -1 ? "nil" : std::to_string(currentJob->variant));
						newLineHasValue = true;
					} else if (name == "target") {
						newSaveData.append("\"").append(currentJob->target).append("\"");
						newLineHasValue = true;
					} else if (name == "expiration_time") {
						if (!hasGameTime) {
							DEBUG_LOG(L"Jobs started but game time was not found yet.");
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
					}
				}
				if (!newLineHasValue) {
					newSaveData.append(sourceValue);
				}
				newSaveData.append("\r\n");
			}

			progress = offset * 100 / dataLength;
			if (progress != lastProgress) {
				if (!callback(progress)) {
					return false;
				}
				lastProgress = progress;
			}
			return true;
		});
		if (!parseSuccess) {
			DEBUG_LOG(L"Error parsing save file: '%s' (offset 0x%08x)", fileName, currentLine, lastOffset);
			return false;
		}
		newSaveData.append("\r\n}\r\n");
		//throw(std::runtime_error("Aborting..."));
		try {
			write_file(newSaveData, fileName);
			//write_file(newSaveData, fileName + L".synced.txt");
		} catch (int e) {
			DEBUG_LOG(L"Error writing save file: '%s' (errno: %d)", e);
			return false;
		}
		DEBUG_LOG(L"Replaced jobs in %ld ms", sw.Time());
		return true;
	}
}