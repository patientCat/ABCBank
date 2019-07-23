#pragma once

#include <wx/wx.h>

class ButtonPanel
: public wxPanel
{
public:
  ButtonPanel(wxPanel *parent, wxOrientation orient= wxHORIZONTAL);
  void addButton(int id, const wxString& name);
  void addThreeButtons();

private:
  wxBoxSizer *sizer_;
};
