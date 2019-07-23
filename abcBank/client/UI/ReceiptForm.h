#pragma once

#include <wx/wx.h>

class ReceiptForm
: public wxDialog
{
public:
  enum ButtonType
  {
    ButtonOK = wxOK,
    ButtonCANCEL = wxCANCEL,
    ButtonYES = wxYES,
    ButtonNO = wxNO,
    ButtonCLOSE = wxCLOSE
  };
public:
  ReceiptForm();

  wxBoxSizer* getTotalSizer() 
  {
    return totalSizer_;
  }
  void addTitle(const wxString& title);
  void addItemText(const wxString& name, const wxString& text);
  void addBottomButton(ButtonType type = ButtonOK);
private:
  wxBoxSizer *totalSizer_;
};


