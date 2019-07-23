//c++ program
//design by >^.^<
//@date    2019-07-13 07:32:57
#include "QueryHistoryBillForm.h"

#include "Constant.h" 
#include "MyWxTools.h"
#include "MyFrame.h"
#include "FormManager.h"
#include "TitlePanel.h"
#include "GridPanel.h"
#include "ButtonPanel.h"
#include "Validator.h"
#include "DetailStatementForm.h"

#include "../../public/PackStream.h"
#include "../../public/Socket.h"

#include "../BankSession.h"

#include <wx/app.h>

using namespace PUBLIC;

BEGIN_EVENT_TABLE(QueryHistoryBillForm, wxPanel)
  EVT_BUTTON(ID_QUERY, QueryHistoryBillForm::OnQuery)
  EVT_BUTTON(ID_CANCEL, QueryHistoryBillForm::OnCancel)
END_EVENT_TABLE()

QueryHistoryBillForm::QueryHistoryBillForm(wxPanel *panel)
  : wxPanel(panel, -1, wxPoint(kChildInterfaceX, kChildInterfaceY), wxSize(kChildInterfaceLength, kChildInterfaceHeight))
  , totalSizer_(new wxBoxSizer(wxVERTICAL))
{
  // set bg color
  wxColour col;
  col.Set(wxT("#4f5049"));
  SetBackgroundColour(col);

  // set mid panel
  wxPanel *midpanel = new wxPanel(this, -1);

  // add all thing in mid panel
  wxStaticBox *staticBox = new wxStaticBox(midpanel, -1, wxT("DETAIL QUERY"));
  wxStaticBoxSizer *staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);

  gridPanel_ = new GridPanel(midpanel, 2, 3, 5, 15);
  buttonPanel_ = new ButtonPanel(midpanel);
  gridPanel_->addRowGrid(wxT("BEGIN DATE"), wxT("YYYY-MM-DD"));
  gridPanel_->addRowGrid(wxT("END DATE"), wxT("YYYY-MM-DD"));

  buttonPanel_->addButton(ID_QUERY, wxT("QUERY"));
  buttonPanel_->addButton(ID_CANCEL, wxT("CANCEL"));

  staticBoxSizer->Add(gridPanel_, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);
  staticBoxSizer->Add(new wxPanel(this, -1), 1);
  staticBoxSizer->Add(buttonPanel_, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, kBottomBorder);

  midpanel->SetSizer(staticBoxSizer);

  totalSizer_->Add(midpanel, 1, wxEXPAND | wxALL, 20);
  SetSizer(totalSizer_);

  Centre();
}

void
QueryHistoryBillForm::OnQuery(wxCommandEvent& event)
{
  // 1. 本地验证是否合法
  if(!validateLocal())
    return;
  // 2. server端验证是否有效，因此这里要有一个TcpClient来连接服务器。
  if(!validateNet())
    return;

}

void
QueryHistoryBillForm::OnCancel(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getDetailStatementForm()->Show(true);
}

bool 
QueryHistoryBillForm::validateLocal()
{
  return true;
}
bool 
QueryHistoryBillForm::validateNet()
{
  // 构造Request包，并将包发送出去
  
  return true;
}

