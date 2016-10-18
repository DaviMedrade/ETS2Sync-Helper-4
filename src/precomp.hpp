#pragma once

#include <wx/wxprec.h>
#include <wx/filename.h>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define DEBUG_CLASS_NAME [this]() { \
	wxString __s =  wxString(typeid(*this).name()); \
	if (__s.Find("class ") != wxNOT_FOUND) { \
		__s.Replace(L"class ", L""); \
	} \
	return __s; \
}()
#define DEBUG_LOG(fmt, ...) wxLogDebug(wxString(L"%s::%s: ") + wxString(fmt), DEBUG_CLASS_NAME, __func__, __VA_ARGS__)
#define DEBUG_LOG_LINE(fmt, ...) wxLogDebug(wxString(L"%s:%d at %s::%s: ") + wxString(fmt), wxString(__FILE__).AfterLast(L'\\'), __LINE__, DEBUG_CLASS_NAME, __func__, __VA_ARGS__)
