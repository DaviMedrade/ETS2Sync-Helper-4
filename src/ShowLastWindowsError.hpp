#pragma once

// Gets the message for the error returned by GetLastError(), formats it, and shows it with wxLogError
// task: Indicator of what function call or task failed (usually the name of the Windows API function that was called)
void ShowLastWindowsError(const wchar_t * task);
