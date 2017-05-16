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
		Save(const std::wstring directory);

		static const int DLC_SCANDINAVIA = 1 << 0;
		static const int DLC_GOINGEAST = 1 << 1;
		static const int DLC_HIGHPOWERCARGO = 1 << 2;
		static const int DLC_FRANCE = 1 << 3;
		static const int DLC_ALL = DLC_SCANDINAVIA | DLC_GOINGEAST | DLC_HIGHPOWERCARGO | DLC_FRANCE;

		struct Job {
			std::string cargo;
			int variant;
			std::string target;
			int urgency;
			int distance;
			int ferryTime;
			int ferryPrice;
			std::string companyTruck;
		};

		typedef std::map<std::string, std::vector<Job>> JobList;

		void setGame(Game game);
		int getDlcs() const;
		static void setupBlankJob(Job& job);
		bool replaceJobList(const JobList& jobs, const std::function<bool(int progress)>& callback) const;

	private:
		static const std::wstring SII_BASENAME;
		static const std::wstring SAVE_BASENAME;
		static const std::string NAME_ATTRIBUTE;
		static const std::string SAVE_TIME_ATTRIBUTE;
		static const std::string DEPEND_ATTRIBUTE;
		static const std::wstring DEPEND_SCANDINAVIA;
		static const std::wstring DEPEND_GOINGEAST;
		static const std::wstring DEPEND_HIGHPOWERCARGO;
		static const std::wstring DEPEND_FRANCE;
		static const std::string ECONOMY_UNIT;
		static const std::string GAME_TIME_ATTRIBUTE;
		static const std::string COMPANY_UNIT;
		static const std::string JOB_UNIT;
		static const std::string COMPANY_NAME_PREFIX;

		int mDlcs;

		virtual void processAttribute(Parser::Sii::Context context, const std::string& attribute, const std::string& value) override;
		virtual bool validate() override;
	};
}