#include "precomp.hpp"
#include "ShowLastWindowsError.hpp"

#include <wininet.h>

void ShowLastWindowsError(const wchar_t * task) {
	DWORD lastError = GetLastError();

	// WinInet extended error message
	if (lastError == ERROR_INTERNET_EXTENDED_ERROR) {
		lastError = 0;
		DWORD lastErrorSize = 0;
		if (!InternetGetLastResponseInfo(&lastError, NULL, &lastErrorSize) && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			wchar_t * message = new wchar_t[lastErrorSize + 1];
			if (!InternetGetLastResponseInfo(&lastError, NULL, &lastErrorSize)) {
				wxLogError("%s error 0x%08X: <could not retrieve WinInet extended error message (1)>", task, GetLastError());
				return;
			}
			DWORD idx = lastErrorSize;
			while (--idx >= 0 && (message[idx] == '\n' || message[idx] == '\r'))
				message[idx] = '\0';
			wxLogError("%s\n\n%s error 0x%08X:", message, task, lastError);
			delete[] message;
			return;
		} else {
			wxLogError("%s error 0x%08X: <could not retrieve WinInet extended error message (2)>", task, lastError);
			return;
		}
	} else if (lastError >= INTERNET_ERROR_BASE && lastError <= INTERNET_ERROR_LAST) {
		wchar_t * message = NULL;
		DWORD messageLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
			GetModuleHandle(L"wininet"), lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&message, 0, NULL);
		if (messageLength) {
			DWORD idx = messageLength;
			while (--idx >= 0 && (message[idx] == '\n' || message[idx] == '\r'))
				message[idx] = '\0';
			wxLogError("%s\n\n%s error 0x%08X", message, task, lastError);
		} else {
			wxLogError("%s error 0x%08X: <could not retrieve WinInet error message (3)>", task, lastError);
		}
		return;
	}

	wchar_t * message = NULL;
	DWORD messageLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&message, 0, NULL);
	if (messageLength) {
		DWORD idx = messageLength;
		while (--idx >= 0 && (message[idx] == '\n' || message[idx] == '\r'))
			message[idx] = '\0';
		wxLogError("%s\n\n%s error 0x%08X", message, task, lastError);
	} else {
		wxLogError("%s error 0x%08X: <could not retrieve error message (4)>", task, lastError);
	}
}