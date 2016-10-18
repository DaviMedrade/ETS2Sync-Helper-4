#pragma once

#include "ObjectSelector.hpp"

typedef ObjectSelector<Ets2::Profile> ProfileSelector;

wxDECLARE_EVENT(EVT_PROFILE_SELECTED, wxCommandEvent);

template <> const Ets2::ProfileList * ProfileSelector::getList();
template <> void ProfileSelector::onObjectChanged();
