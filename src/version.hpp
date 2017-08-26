#pragma once

#include <string>

// put this here so it's easier to bump
#define THIS_APP_VERSION L"5.3.0"

const std::wstring APP_NAME = L"ETS2SyncHelper";
const std::wstring APP_DISPLAY_NAME = L"ETS2Sync Helper";
const std::wstring APP_VERSION = THIS_APP_VERSION;

const std::wstring APP_URL_WEBSITE = L"http://sync.dsantosdev.com/";
#if _DEBUG
const std::wstring APP_URL_API_BASE = APP_URL_WEBSITE + "app-test/";
#else
const std::wstring APP_URL_API_BASE = APP_URL_WEBSITE + "app/";
#endif
const std::wstring APP_URL_CHECK_VERSION = APP_URL_API_BASE + "check_version?v=" + APP_VERSION + "&hl=en";

const std::wstring APP_URL_SYNC = APP_URL_API_BASE + "sync?v=" + APP_VERSION + "&hl=en";