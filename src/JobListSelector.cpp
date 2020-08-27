#include "precomp.hpp"
#include "JobListSelector.hpp"

JobListSelector::JobListSelector(wxWindow * parent, wxWindowID id)
	: Ets2StaticBox(parent, id, "Job List") {
	wxBoxSizer * contentSizer = getContentSizer();

	wxBoxSizer * controlsSizer = new wxBoxSizer(wxHORIZONTAL);
	contentSizer->Add(controlsSizer, wxSizerFlags().Expand());

	mJobList = new wxChoice(this, wxID_ANY);
	mJobList->Append(std::vector<wxString> {L"Automatic", L"List 1", L"List 2", L"List 3", L"List 4", L"List 5", L"List 6", L"List 7", L"List 8"});
	mJobList->SetSelection(0);
	controlsSizer->Add(mJobList);

	controlsSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(4, 4)).x);

	mAutoListInfo = new wxStaticText(this, wxID_ANY, L"Changes every day at 0:00 UTC.");
	controlsSizer->Add(mAutoListInfo, wxSizerFlags().CenterVertical());

	mJobList->Bind(wxEVT_CHOICE, [this](wxCommandEvent&) { onSetJobList(); });
}

int JobListSelector::getJobList() const {
	return mJobList->GetSelection() - 1;
}

void JobListSelector::setJobList(int jobList) {
	mJobList->SetSelection(jobList + 1);
	onSetJobList();
}

void JobListSelector::onSetJobList() {
	mAutoListInfo->Show(getJobList() == -1);
}