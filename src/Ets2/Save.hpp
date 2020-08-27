#pragma once

#include "Object.hpp"
#include "ObjectList.hpp"

#include <string>
#include <map>

namespace Ets2 {
	class Save;
	typedef ObjectList<Save> SaveList;

	class Save : public Object {
	public:
		Save(Game game, const std::wstring directory);

		struct Job {
			std::string cargo;
			std::string target;
			int urgency;
			int distance;
			int ferryTime;
			int ferryPrice;
			std::string companyTruck;
			std::string trailerVariant;
			std::string trailerDefinition;
			int unitsCount;
			int fillRatio;
			int trailerPlace;
		};

		typedef std::map<std::string, std::vector<Job>> JobList;
		typedef std::vector<std::wstring> DlcList;

		const DlcList& getDlcs() const;
		static void setupBlankJob(Job& job);
		int replaceJobList(const JobList& jobs, const std::function<bool(int progress)>& callback) const;

	private:
		static const std::wstring SII_BASENAME;
		static const std::wstring SAVE_BASENAME;
		static const std::string NAME_ATTRIBUTE;
		static const std::string SAVE_TIME_ATTRIBUTE;
		static const std::string DEPEND_ATTRIBUTE;
		static const std::string ECONOMY_UNIT;
		static const std::string GAME_TIME_ATTRIBUTE;
		static const std::string COMPANY_UNIT;
		static const std::string JOB_UNIT;
		static const std::string COMPANY_NAME_PREFIX;

		DlcList mDlcs;

		virtual void processAttribute(Parser::Sii::Context context, const std::string& attribute, const std::string& value) override;
		virtual bool validate() override;
	};
}