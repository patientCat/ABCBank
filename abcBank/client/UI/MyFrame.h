#pragma once

#include <wx/wx.h>

class MyFrame
: public wxFrame
{
public:
  MyFrame(const wxString& title);

  void OnExit(wxCommandEvent &event);
  void ExitConfirm(wxCloseEvent &event);
};
