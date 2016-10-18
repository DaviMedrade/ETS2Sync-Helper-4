#pragma once

#include "Ets2StaticBox.hpp"
#include "StatusText.hpp"
#include "ObjectCombobox.hpp"

template <class T> class ObjectSelector : public Ets2StaticBox {
public:
	ObjectSelector<T> (wxWindow * parent, wxWindowID id)
		: Ets2StaticBox(parent, id, mLabel) {
		wxBoxSizer * contentSizer = getContentSizer();

		mStatusText = new StatusText(this, wxID_ANY);
		contentSizer->Add(mStatusText);

		contentSizer->AddSpacer(wxDLG_UNIT(this, wxPoint(0, 4)).y);

		mCombobox = new ObjectCombobox<T>(this, wxID_ANY);
		contentSizer->Add(mCombobox, wxSizerFlags().Expand());
		mCombobox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent&){ onObjectChanged(); });
	}

	int GetSelection() {
		return mCombobox->GetSelection();
	}

	void setList(const Ets2::ObjectList<T> * list) {
		if (!list) {
			mStatusText->SetLabel(L"", StatusText::TYPE_STATUS);
			mCombobox->clearList();
			return;
		}

		size_t numObjects = list->size();

		wxString& formatString = (numObjects == 0 ? mFormatZero : (numObjects == 1 ? mFormatOne : mFormatPlural));
		mStatusText->SetLabel(wxString::Format(formatString, numObjects), numObjects == 0 ? StatusText::TYPE_ERROR : StatusText::TYPE_SUCCESS);

		mCombobox->setList(list);
		if (numObjects == 0) {
			mCombobox->Disable();
		} else {
			mCombobox->Enable();
			mCombobox->SetSelection(0);
		}
	}

	void updateFromEts2Info() {
		setList(getList());
	};

private:
	StatusText * mStatusText;
	ObjectCombobox<T> * mCombobox;

	static wxString mLabel;
	static wxString mFormatZero;
	static wxString mFormatOne;
	static wxString mFormatPlural;

	const Ets2::ObjectList<T> * getList();
	void onObjectChanged();
};

template <class T> void ObjectSelector<T>::onObjectChanged() {};
template <class T> const Ets2::ObjectList<T> * ObjectSelector<T>::getList() { return nullptr; }