#pragma once

#include "QueryForm.h"


class BankSession;

class QueryDayBillForm
: public QueryForm
{
public:
  QueryDayBillForm();

  void onQuery(wxCommandEvent&) override;
  void onCancel(wxCommandEvent&) override;

private:
  void showReceiptForm(BankSession&);
  bool validateLocal(BankSession&);
  bool validateNet(BankSession&);

private:
};
