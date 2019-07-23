#pragma once

#include <wx/wx.h>
#include <map>

class QueryForm
: public wxDialog
{
public:
  QueryForm(const wxString &title, const wxPoint&pos=wxDefaultPosition, const wxSize&size=wxDefaultSize);

  virtual void addItemText(const wxString& item, const wxString& remark);
  virtual void onQuery(wxCommandEvent&) = 0;
  virtual void onCancel(wxCommandEvent&) = 0;
  void resetText()
  {
    for(auto &elem : textList_)
    {
      elem.second->Clear();
    }
  }
  wxTextCtrl *getTextCtrl(const wxString& item)
  {
    return textList_[item];
  }
private:
  void addBottomButton();
private:
  wxStaticBoxSizer * totalSizer_;
  wxBoxSizer * itemSizer_;
  wxBoxSizer * buttonSizer_;
  std::map<wxString, wxTextCtrl*> textList_;
  wxSize textCtrlSize_;

  DECLARE_EVENT_TABLE()
};

