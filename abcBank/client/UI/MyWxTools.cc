//c++ program
//design by >^.^<
//@date    2019-07-08 05:49:31
#include "MyWxTools.h"
#include "Constant.h"
#include <wx/statline.h>

void addHeadline(wxWindow* panel, wxSizer* sizer, const wxString& str)
{
  // add title
  wxStaticText *title = new wxStaticText(panel, -1, str);

  // add line
  wxStaticLine *line = new wxStaticLine(panel, -1, wxDefaultPosition, wxSize(kInterfaceLength, -1), wxLI_HORIZONTAL);


  sizer->Add(title, 0, wxTOP | wxLEFT, kTitlePos);
  sizer->Add(line, 0, wxTOP , 5);
}

