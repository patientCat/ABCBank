//c++ program
//design by >^.^<
//@date    2019-07-09 18:22:39
#include "MyFrame.h"
#include "Constant.h"
#include "FormManager.h"


MyFrame::MyFrame(const wxString& title)
  : wxFrame(NULL, -1, title, wxDefaultPosition, wxSize(kInterfaceLength, kInterfaceHeight), wxDEFAULT_FRAME_STYLE & (~wxRESIZE_BORDER))
{
  Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnExit));  
  Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::ExitConfirm));  
  Centre();
}

#include <iostream>
void
MyFrame::OnExit(wxCommandEvent &event)
{
  Destroy();
}

void 
MyFrame::ExitConfirm(wxCloseEvent &event)
{
  if(event.CanVeto())
  {
    if(wxMessageBox("Do you want to quit really?",
          "Please confirm",
          wxICON_QUESTION | wxYES_NO ) != wxYES)
    {
      event.Veto();
      return;
    }
  }
  Destroy();
}
