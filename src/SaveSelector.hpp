#pragma once

#include "ObjectSelector.hpp"

wxDECLARE_EVENT(EVT_SAVE_SELECTED, wxCommandEvent);

typedef ObjectSelector<Ets2::Save> SaveSelector;

template <> const Ets2::SaveList * SaveSelector::getList();
template <> void SaveSelector::onObjectChanged();
