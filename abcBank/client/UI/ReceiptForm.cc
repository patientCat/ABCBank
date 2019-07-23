//c++ program
//design by >^.^<
//@date    2019-07-19 02:42:03
#include "ReceiptForm.h"
#include "Constant.h"
#include <wx/statline.h>


ReceiptForm::ReceiptForm()
  : wxDialog(NULL, -1, wxEmptyString, wxPoint(kChildInterfaceX, kChildInterfaceY), wxSize(kChildInterfaceLength, kChildInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
{
  Centre();
  SetSizer(totalSizer_);
}

void 
ReceiptForm::addTitle(const wxString& title)
{
  wxBoxSizer * boxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(new wxStaticText(this, -1, title), 0, wxALIGN_LEFT| wxLEFT | wxTOP , kTitlePos);
  boxSizer->Add(
    new wxStaticLine(this, -1, wxDefaultPosition, wxSize(kChildInterfaceLength, -1), wxLI_HORIZONTAL),
    0, wxTOP, 5);
  totalSizer_->Add(boxSizer, 0);
}

void 
ReceiptForm::addItemText(const wxString& name, const wxString& text)
{
  wxSizer * boxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(new wxStaticText(this, -1, name), 0, wxALIGN_CENTER_VERTICAL);
  boxSizer->Add(new wxStaticText(this, -1, text), 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  totalSizer_->Add(boxSizer, 0, wxALIGN_LEFT | wxLEFT , 2 * kTitlePos);
}

void
ReceiptForm::addBottomButton(ReceiptForm::ButtonType type)
{
  wxSizer * sizer = CreateButtonSizer(type);
  totalSizer_->Add(new wxPanel(this, -1), 1, wxEXPAND);
  totalSizer_->Add(sizer, 0, wxALIGN_CENTER | wxBOTTOM, kBottomBorder);
}
