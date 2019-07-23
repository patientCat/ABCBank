//c++ program
//design by >^.^<
//@date    2019-07-13 07:32:57
#include "QueryDayBillForm.h"

#include "Constant.h" 
#include "MyWxTools.h"
#include "MyFrame.h"
#include "FormManager.h"
#include "TitlePanel.h"
#include "GridPanel.h"
#include "ButtonPanel.h"
#include "Validator.h"
#include "DetailStatementForm.h"
#include "TransReport.h"

#include "../../public/PackStream.h"
#include "../../public/Socket.h"
#include "../../public/Exception.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include <wx/app.h>

using namespace PUBLIC;

QueryDayBillForm::QueryDayBillForm()
  : QueryForm("DETAIL QUERY", wxPoint(kChildInterfaceX, kChildInterfaceY), wxSize(kChildInterfaceLength, kChildInterfaceHeight))
{
  addItemText("Date", "YYYY-MM-DD");
  Centre();
}

void
QueryDayBillForm::onQuery(wxCommandEvent& event)
{
  try{
    BankSession bs;
    bs.setCmd(CMD_DAY_BILL);
    // 一定是从第0页开始查询
    bs.setRequestItem("PAGE", "0");
    // 1. 本地验证是否合法
    if(!validateLocal(bs))
      return;
    // 2. 网络验证
    if(!validateNet(bs))
      return;

    assert(bs.getErrorCode() == 0);
    showReceiptForm(bs);
  }
  catch(Exception &e){
    wxLogError(wxT("BankSession %s"), e.what());
  }
}

void
QueryDayBillForm::showReceiptForm(BankSession& bs)
{
  TransReport report("DetailQuery");
  for(auto &td : *bs.getTransDetailVecPtr())
  {
    report.insertRow(td);
  }
  report.ShowModal();
}

void
QueryDayBillForm::onCancel(wxCommandEvent& event)
{
  Show(false);
  GetFormManager().getDetailStatementForm()->Show(true);
}

bool 
QueryDayBillForm::validateLocal(BankSession& bs)
{
  wxTextCtrl* dateTc = getTextCtrl("Date");
  auto date = dateTc->GetLineText(0);
  if(!Validator::validateDateAndTip(date))
    return false;

  // 设置构造Request包的元素
  bs.setRequestItem("DATE", date);
  return true;
}
bool 
QueryDayBillForm::validateNet(BankSession& bs)
{
  GetTransactionManager().doAction(bs);
  // 根据返回的错误码和错误msg反馈
  if(bs.getErrorCode() != 0)
  {
    wxLogError(wxT("error: %s"), wxString::FromUTF8(bs.getErrorMsg()));
    return false;
  }
  return true;
}

