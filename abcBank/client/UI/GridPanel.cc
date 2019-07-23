//c++ program
//design by >^.^<
//@date    2019-07-09 20:13:00
#include "GridPanel.h"
#include <cassert>


GridPanel::GridPanel(wxPanel* parent, int rowNum, int colNum, int wBorder, int hBorder)
  : wxPanel(parent, -1)
  , sizer_(new wxFlexGridSizer(rowNum, colNum, wBorder, hBorder)) 
{
  SetSizer(sizer_);
}

void
GridPanel::addRowGrid(const wxString& item, const wxString& remark, bool isPassword)
{
  if(isPassword)
  {
  textCtrlMap_[item] = 
      new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
  }
  else{
  textCtrlMap_[item] = 
      new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize);
  }

  textList_.push_back(
      new wxStaticText(this, -1, item)
      );
  textRemarkList_.push_back(
      new wxStaticText(this, -1, remark)
      );
  sizer_->Add(textList_.back(), 0, wxALIGN_RIGHT);
  sizer_->Add(textCtrlMap_[item], 0, wxALIGN_CENTER);
  sizer_->Add(textRemarkList_.back(), 0);
}

wxTextCtrl * 
GridPanel::getTextCtrl(wxString const &name)
{
  assert(textCtrlMap_.count(name) > 0);
  return textCtrlMap_[name];
}

void 
GridPanel::resetText()
{
  for(auto tc : textCtrlMap_)
    tc.second->Clear();
}
