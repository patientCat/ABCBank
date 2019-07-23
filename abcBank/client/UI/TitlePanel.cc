//c++ program
//design by >^.^<
//@date    2019-07-10 00:40:27
#include "TitlePanel.h"

#include "Constant.h"

TitlePanel::TitlePanel(wxPanel *parent, const wxString& headline)
  : wxPanel(parent, -1)
  , sizer_(new wxBoxSizer(wxVERTICAL))
  , headline_(headline)
{
  SetSizer(sizer_);
}

void 
TitlePanel::setLevel1()
{
  title_ = new wxStaticText(this, -1, headline_);
  sizer_->Add(title_, 0, wxTOP | wxALIGN_CENTER_HORIZONTAL, kTitlePos);
  setLine();
}

void 
TitlePanel::setLevel2()
{
  title_ = new wxStaticText(this, -1, headline_);
  sizer_->Add(title_, 0, wxTOP | wxLEFT, kTitlePos);
  setLine();
}

void
TitlePanel::setLevel3()
{
  title_ = new wxStaticText(this, -1, headline_);
  sizer_->Add(title_, 0, wxTOP | wxLEFT, 15);
}

void 
TitlePanel::setLine()
{
  line_ = new wxStaticLine(this, -1, wxDefaultPosition, wxSize(kInterfaceLength, -1), wxLI_HORIZONTAL);
  sizer_->Add(line_, 0, wxTOP , 5);
}
