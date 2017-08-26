#pragma once

#include "Ets2StaticBox.hpp"

class JobListSelector : public Ets2StaticBox {
public:
	JobListSelector(wxWindow * parent, wxWindowID id);

	int getJobList() const;
	void setJobList(int jobList);

private:
	wxChoice * mJobList;
	wxStaticText * mAutoListInfo;
};