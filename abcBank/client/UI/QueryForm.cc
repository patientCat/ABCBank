//c++ program
//design by >^.^<
//@date    2019-07-21 04:48:56
#include "Constant.h"
#include "QueryForm.h"

BEGIN_EVENT_TABLE(QueryForm, wxDialog)
  EVT_BUTTON(ID_QUERY, QueryForm::onQuery)
  EVT_BUTTON(ID_CANCEL, QueryForm::onCancel)
END_EVENT_TABLE()

QueryForm::QueryForm(const wxString& title, const wxPoint &pos, const wxSize&size)
  : wxDialog(NULL, -1, wxEmptyString, pos, size)
  , totalSizer_(new wxStaticBoxSizer(new wxStaticBox(this, -1, title), wxVERTICAL))
  , itemSizer_(new wxBoxSizer(wxVERTICAL))
  , buttonSizer_(new wxBoxSizer(wxHORIZONTAL))
  , textCtrlSize_(size)
{
  totalSizer_->Add(itemSizer_, 0, wxTOP, 10);
  totalSizer_->Add(new wxPanel(this, -1), 1, wxEXPAND);
  totalSizer_->Add(buttonSizer_, 0, wxALIGN_CENTER | wxBOTTOM, kBottomBorder);
  addBottomButton();
  SetSizer(totalSizer_);
}

void 
QueryForm::addItemText(const wxString& item, const wxString& remark)
{
  wxBoxSizer *tmpSizer = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *itemStaticText = new wxStaticText(this, -1, item);
  tmpSizer->Add(itemStaticText, 0, wxALIGN_LEFT | wxLEFT, 30);

  textCtrlSize_.x /= 3;
  textCtrlSize_.y = 25;
  wxTextCtrl *textCtrl = new wxTextCtrl(this, -1, remark, wxDefaultPosition, textCtrlSize_);
  textList_[item] = textCtrl;
  tmpSizer->Add(textCtrl, 0, wxLEFT, 10);

  wxStaticText *remarkStaticText = new wxStaticText(this, -1, remark);
  tmpSizer->Add(remarkStaticText, 0, wxLEFT, 10);
  itemSizer_->Add(tmpSizer, 0, wxTOP, 20);
}

void 
QueryForm::addBottomButton()
{
  // Query
  buttonSizer_->Add(new wxButton(this, ID_QUERY, "QUERY"), 0);
  // Cancel
  buttonSizer_->Add(new wxButton(this, ID_CANCEL, "CANCEL"), 0, wxLEFT, 50);
}
