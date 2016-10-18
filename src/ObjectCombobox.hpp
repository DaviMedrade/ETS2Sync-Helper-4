#pragma once

#include "Ets2/Object.hpp"
#include <wx/odcombo.h>

template <class T> class ObjectCombobox : public wxOwnerDrawnComboBox {
public:
	ObjectCombobox<T>(wxWindow * parent, wxWindowID id)
		: wxOwnerDrawnComboBox(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY) {
		mList = nullptr;
		mTimer.SetOwner(this);
		Bind(wxEVT_TIMER, [this](wxTimerEvent&) { onTimerEvent(); }, mTimer.GetId());
		mTimer.Start(300);
	}

	struct OBJECT_SIZES {
		wxSize size;
		wxCoord maxNameWidth;
		wxCoord separatorWidth;
		wxCoord maxDateWidth;
		wxCoord maxItemHeight;
	};

	void onTimerEvent() {
		Refresh();
		if (this->IsPopupShown()) {
			this->GetPopupControl()->GetControl()->Refresh();
		}
	}

	void clearList() {
		Clear();
		Disable();
	}

	void setList(const Ets2::ObjectList<T> * collection) {
		mList = collection;
		if (mList->empty()) {
			clearList();
			return;
		}

		std::vector<wxString> names;
		for (T * item : *mList) {
			names.push_back(item->getName());
		}
		Set(names);
	}

	// All items are supposed to be the same size, which is the size of the longest item,
	// so there's no point in passing the index of the item we want.
	OBJECT_SIZES getItemSizes() const {
		wxString name;
		wxSize extName, extDate;
		wxClientDC dc(GetParent());
		OBJECT_SIZES ret;

		ret.maxNameWidth = 0;
		ret.maxDateWidth = 0;
		ret.maxItemHeight = 0;
		ret.separatorWidth = dc.GetTextExtent(L" — ").x;
		for (T * item : *mList) {
			name = item->getName();

			extName = dc.GetTextExtent(name);
			ret.maxNameWidth = std::max(extName.x, ret.maxNameWidth);
			ret.maxItemHeight = std::max(extName.y, ret.maxItemHeight);

			extDate = dc.GetTextExtent(item->getSaveTimeCaption());
			ret.maxDateWidth = std::max(extDate.x, ret.maxDateWidth);
			ret.maxItemHeight = std::max(extDate.y, ret.maxItemHeight);
		}
		ret.size = wxSize(ret.maxNameWidth + ret.separatorWidth + ret.maxDateWidth + ITEM_PAD_H * 2, ret.maxItemHeight + ITEM_PAD_V * 2);
		return ret;
	}

	static const int ITEM_PAD_H = 4;
	static const int ITEM_PAD_V = 2;
	virtual wxCoord OnMeasureItemWidth(size_t /* index */) const override {
		return getItemSizes().size.x;
	}

	virtual wxCoord OnMeasureItem(size_t /* index */) const override {
		return getItemSizes().size.y;
	}

	virtual void OnDrawItem(wxDC& dc, const wxRect& rect, int n, int flags) const override {
		if (n == wxNOT_FOUND || ((unsigned int)n >= (*mList).size())) {
			return;
		}
		T * item = (*mList)[n];
		wxString name = item->getName();
		wxString date = item->getSaveTimeCaption();
		if (flags & wxODCB_PAINTING_CONTROL) {
			wxString s = name + L" — " + date;
			wxSize ext = GetTextExtent(s);
			dc.DrawText(s, rect.x + ITEM_PAD_H, rect.y + (rect.height - ext.y) / 2);
			return;
		}

		OBJECT_SIZES sizes = getItemSizes();
		const wxSize& sz = sizes.size;
		wxSize dateExt = dc.GetTextExtent(date);
		dc.DrawText(name, rect.x + ITEM_PAD_H, rect.y + ITEM_PAD_V);
		if (sz.x > GetSize().x) {
			dc.DrawText(date, rect.x + rect.width - dateExt.x - ITEM_PAD_H, rect.y + ITEM_PAD_V);
		} else {
			dc.DrawText(date, rect.x + sz.x - sizes.maxDateWidth - ITEM_PAD_H, rect.y + ITEM_PAD_V);
		}
	}

private:
	const Ets2::ObjectList<T> * mList;
	wxTimer mTimer;
};
