#pragma once

#include "Ets2/Info.hpp"

class Ets2StaticBox : public wxStaticBox {
public:
	Ets2StaticBox(wxWindow * parent, wxWindowID id, wxString label);

	void setEts2Info(Ets2::Info * ets2Info);
	Ets2::Info * getEts2Info();
	wxBoxSizer * getContentSizer();

protected:
	virtual void updateFromEts2Info();

private:
	Ets2::Info * mEts2Info;
	wxBoxSizer * mContentSizer;
};