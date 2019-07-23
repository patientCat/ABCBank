//c++ program
//design by >^.^<
//@date    2019-07-22 06:15:41
#include "Constant.h"
#include "TransReport.h"
#include "../../public/TransDetail.h"
#include "../../public/StringUtil.h"
#include <string>

using namespace std;
using namespace PUBLIC;

TransReport::TransReport(const wxString& title)
  : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(2*kInterfaceLength, kInterfaceHeight))
  , listCtrl_(new wxListCtrl(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT))
  , insertIndex_(0)
{
  wxListItem col0;
  col0.SetId(0);
  col0.SetText(wxT("序号"));
  col0.SetWidth(20);
  listCtrl_->InsertColumn(0, col0);

  wxListItem col1;
  col1.SetId(1);
  col1.SetText(wxT("交易日期"));
  col1.SetWidth(160);
  listCtrl_->InsertColumn(1, col1);

  wxListItem col2;
  col2.SetId(2);
  col2.SetText(wxT("账号"));
  col2.SetWidth(40);
  listCtrl_->InsertColumn(2, col2);

  wxListItem col3;
  col3.SetId(3);
  col3.SetText(wxT("对方账号"));
  col3.SetWidth(80);
  listCtrl_->InsertColumn(3, col3);

  wxListItem col4;
  col4.SetId(4);
  col4.SetText(wxT("摘要"));
  col4.SetWidth(40);
  listCtrl_->InsertColumn(4, col4);

  wxListItem col5;
  col5.SetId(5);
  col5.SetText(wxT("交易金额"));
  col5.SetWidth(80);
  listCtrl_->InsertColumn(5, col5);

  wxListItem col6;
  col6.SetId(6);
  col6.SetText(wxT("余额"));
  col6.SetWidth(40);
  listCtrl_->InsertColumn(6, col6);

  Centre();
}

void
TransReport::insertRow(const TransDetail& td)
{
  listCtrl_->InsertItem(insertIndex_, "");
  listCtrl_->SetItem(insertIndex_, 0, td.trans_id.c_str());
  listCtrl_->SetItem(insertIndex_, 1, td.trans_date.c_str());
  if(td.account_id == "000000")
  {
    listCtrl_->SetItem(insertIndex_, 2, td.account_id.c_str());
  }
  else{
    listCtrl_->SetItem(insertIndex_, 2, "NULL");
  }
  if(td.other_account_id == "000000")
  {
    listCtrl_->SetItem(insertIndex_, 3, td.other_account_id.c_str());
  }
  else{
    listCtrl_->SetItem(insertIndex_, 3, "NULL");
  }
  listCtrl_->SetItem(insertIndex_, 4, wxString::FromAscii(td.abstract_name.c_str(), td.abstract_name.size()));
  listCtrl_->SetItem(insertIndex_, 5, formatDoubleString(to_string(td.money), 2).c_str());
  listCtrl_->SetItem(insertIndex_, 6, formatDoubleString(to_string(td.balance), 2).c_str());
  ++insertIndex_;
}
