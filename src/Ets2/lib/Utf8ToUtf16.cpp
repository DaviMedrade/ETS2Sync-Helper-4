#include "precomp.hpp"
#include "Utf8ToUtf16.hpp"

void Utf8ToUtf16(const char * src, size_t srcLength, std::wstring& dst) {
	if (srcLength == 0) {
		dst.clear();
		return;
	}
	int len = MultiByteToWideChar(CP_UTF8, 0, src, srcLength, NULL, 0);
	if (len > 0) {
		dst.resize(len);
		if (MultiByteToWideChar(CP_UTF8, 0, src, srcLength, &dst[0], dst.length())) {
			return;
		}
	}
	// conversion failed; do a raw copy
	wxLogDebug(L"UTF8 conversion failed for string '%s'.", std::string(src, srcLength));
	dst.resize(srcLength);
	wchar_t * dstPtr = &dst[0];
	for (size_t idx = 0; idx < srcLength; idx++) {
		*(dstPtr + idx) = *(src + idx);
	}
}
