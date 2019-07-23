#pragma once

#include <wx/wx.h>
#include <map>
#include <vector>

class GridPanel
: public wxPanel
{
  using TextCtrlMap = std::map<wxString, wxTextCtrl*>;
  using StaticTextList = std::vector<wxStaticText*>;
public:
  GridPanel(wxPanel* parent, int rowNum, int colNum, int wBorder=0, int hBorder=0);


  void addRowGrid(const wxString&, const wxString&, bool isPassword = false);
  wxTextCtrl *getTextCtrl(wxString const &name);
  void resetText();
private:
  wxFlexGridSizer *sizer_;

  StaticTextList textList_;
  TextCtrlMap textCtrlMap_;
  StaticTextList textRemarkList_;
};
