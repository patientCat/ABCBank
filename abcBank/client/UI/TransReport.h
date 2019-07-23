//c++ program
//design by >^.^<
//@date    2019-07-13 06:40:44
#include <wx/wx.h>
#include <wx/listctrl.h>

// 思考下它的功能
// 当从CMD层收到TransDetail数据后，要根据TransDetail插入数据

namespace PUBLIC{
struct TransDetail;
}

class TransReport
: public wxDialog
{
public:
  TransReport(const wxString& title);
  void insertRow(const PUBLIC::TransDetail&);
  void onCancel(wxCommandEvent&);
private:
  wxListCtrl * listCtrl_;
  int insertIndex_;
};
