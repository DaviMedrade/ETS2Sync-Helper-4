#include "precomp.hpp"
#include "Ets2StaticBox.hpp"

Ets2StaticBox::Ets2StaticBox(wxWindow * parent, wxWindowID id, wxString label)
	: wxStaticBox(parent, id, label) {
	mEts2Info = nullptr;

	wxBoxSizer * canvas = new wxBoxSizer(wxVERTICAL);

	wxPoint border = wxDLG_UNIT(this, wxPoint(5, 7));
	wxBoxSizer * borderSizer = new wxBoxSizer(wxVERTICAL);
	borderSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);
	canvas->Add(borderSizer, wxSizerFlags().Border(wxTOP | wxBOTTOM, border.y).Expand());

	mContentSizer = new wxBoxSizer(wxVERTICAL);
	borderSizer->Add(mContentSizer, wxSizerFlags().Border(wxLEFT | wxRIGHT, border.x).Expand());

	SetSizer(canvas);
}

Ets2::Info * Ets2StaticBox::getEts2Info() {
	return mEts2Info;
}

void Ets2StaticBox::setEts2Info(Ets2::Info * ets2Info) {
	mEts2Info = ets2Info;
	updateFromEts2Info();
}

wxBoxSizer * Ets2StaticBox::getContentSizer() {
	return mContentSizer;
}

// Subclasses should reimplement this; it's called after mEts2Info changes.
void Ets2StaticBox::updateFromEts2Info() {
}