#pragma  once

#include <wx/wx.h>
#include <wx/statline.h>


class TitlePanel
: public wxPanel
{
public:
  TitlePanel(wxPanel *parent, const wxString& title);

  // 标题的级别，是几级标题
  void setLevel1();
  void setLevel2();
  void setLevel3();
  void setLine();
private:
  wxBoxSizer *sizer_;
  wxString headline_;
  wxStaticText *title_;
  wxStaticLine *line_;
};
