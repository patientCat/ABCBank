//c++ program
//design by >^.^<
//@date    2019-07-09 20:37:07
#include "ButtonPanel.h"
#include "Constant.h"


ButtonPanel::ButtonPanel(wxPanel* panel, wxOrientation orient)
  : wxPanel(panel, -1)
  , sizer_(new wxBoxSizer(orient))
{
  SetSizer(sizer_);
}

void 
ButtonPanel::addButton(int id, const wxString& name)
{
  sizer_->Add(new wxButton(this, id, name), 0, 
      wxALL, 20);
}
void
ButtonPanel::addThreeButtons()
{
  addButton(ID_SUBMIT, "SUBMIT");
  addButton(ID_RESET, "RESET");
  addButton(ID_CANCEL, "CANCEL");
}
