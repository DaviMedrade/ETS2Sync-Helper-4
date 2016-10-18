#pragma once

class Ets2SyncHelperApp : public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();

private:
	wxLocale * mLocale;
};
