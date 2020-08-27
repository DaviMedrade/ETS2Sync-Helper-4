#pragma once

#include <string>

// put this here so it's easier to bump
#define THIS_APP_VERSION L"5.7.0"

const std::wstring APP_NAME = L"ETS2SyncHelper";
const std::wstring APP_DISPLAY_NAME = L"ETS2Sync Helper";
const std::wstring APP_VERSION = THIS_APP_VERSION;

const std::wstring APP_URL_WEBSITE = L"https://inklay-webapp.glitch.me/ETS2SH/app/";

const std::wstring APP_URL_CHECK_VERSION = APP_URL_WEBSITE + "check_version?v=" + APP_VERSION + "&hl=en";

const std::wstring APP_URL_SYNC = APP_URL_WEBSITE + "sync?v=" + APP_VERSION + "&hl=en";