#pragma once

#include <string>

#define THIS_APP_VERSION L"5.8.1"

const std::wstring APP_NAME = L"ETS2-ATS-Sync-Helper";
const std::wstring APP_DISPLAY_NAME = L"ETS2&ATS Sync Helper";
const std::wstring APP_VERSION = THIS_APP_VERSION;

const std::wstring APP_URL_WEBSITE = L"https://github.com/Inklay/";

#if _DEBUG
const std::wstring APP_URL_BASE = L"http://localhost:8080/SyncHelper/app/";
#else
const std::wstring APP_URL_BASE = L"http://webservices-env.eba-nh6hq243.us-east-1.elasticbeanstalk.com/SyncHelper/app/";
#endif

const std::wstring APP_URL_CHECK_VERSION = APP_URL_BASE + "check_version?v=" + APP_VERSION;

const std::wstring APP_URL_SYNC = APP_URL_BASE + "sync?v=" + APP_VERSION;