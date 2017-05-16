#pragma once

#include "Ets2StaticBox.hpp"
#include "StatusText.hpp"
#include "ObjectCombobox.hpp"

template <class T> class ObjectSelector : public Ets2StaticBox {
public:
	ObjectSelector<T> (wxWindow * parent, wxWindowID id)
		: Ets2StaticBox(parent, id, L"") {

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
		std::wstring gameString = getEts2Info()->getGame() == Ets2::Game::ATS ? L"ATS" : L"ETS2";
		SetLabel(wxString::Format(mLabel, gameString));

		if (!list) {
			mStatusText->SetLabel(L"", StatusText::Type::STATUS);
			mCombobox->clearList();
			return;
		}

		size_t numObjects = list->size();

		wxString& formatString = (numObjects == 0 ? mFormatZero : (numObjects == 1 ? mFormatOne : mFormatPlural));
		wxString formattedString = (numObjects > 1 ? wxString::Format(formatString, numObjects, gameString) : wxString::Format(formatString, gameString));
		mStatusText->SetLabel(formattedString, numObjects == 0 ? StatusText::Type::FAILURE : StatusText::Type::SUCCESS);

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