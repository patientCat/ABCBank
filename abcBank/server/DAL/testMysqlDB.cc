//c++ program
//design by >^.^<
//@date    2019-07-18 19:39:53
#include <iostream>
#include "MysqlDB.h"

int main()
{
    MysqlDB db;
    db.open("localhost", "root", "123", "bank");
    string user("admin");
    string pass("123456");

    char buff[1024] = {0};
    snprintf(buff, sizeof(buff), 
        "select teller_name from TELLER where teller_name='%s' and teller_pass='%s';",
        user.c_str(), pass.c_str());
    MysqlRecordset res;
    res = db.querySQL(buff);
    cout << res.getRows() << endl;
}
