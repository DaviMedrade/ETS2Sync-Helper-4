#include "precomp.hpp"
#include "ProfileSelector.hpp"

wxDEFINE_EVENT(EVT_PROFILE_SELECTED, wxCommandEvent);

wxString ProfileSelector::mLabel = "Select %ls Profile";
wxString ProfileSelector::mFormatZero = "No %ls profiles found.";
wxString ProfileSelector::mFormatOne = "1 %ls profile.";
wxString ProfileSelector::mFormatPlural = "%u %ls profiles.";

template <> const Ets2::ProfileList * ProfileSelector::getList() {
	Ets2::Info * ets2Info = getEts2Info();
	if (ets2Info->isValid()) {
		return &ets2Info->getProfiles();
	} else {
		return nullptr;
	}
}

template <> void ProfileSelector::onObjectChanged() {
	QueueEvent(new wxCommandEvent(EVT_PROFILE_SELECTED));
};
