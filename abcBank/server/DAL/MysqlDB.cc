#include "MysqlDB.h"

#include "muduo/base/Exception.h"

using namespace DAL;
using namespace muduo;

MysqlDB::MysqlDB() : mysql_(NULL)
{
}

MysqlDB::~MysqlDB()
{
	if (mysql_)
	{
		close();
	}
}
void MysqlDB::open(const char* host,
				   const char* user,
				   const char* passwd,
				   const char* db,
				   unsigned int port)
{

	mysql_ = mysql_init(NULL);
	if (mysql_ == NULL)
	{
		string errmsg = mysql_error(mysql_);
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}

	my_bool reconnect = true;
	mysql_options(mysql_, MYSQL_OPT_RECONNECT, &reconnect);
	mysql_options(mysql_, MYSQL_SET_CHARSET_NAME, "gbk");

	if (!mysql_real_connect(mysql_, host, user,
		passwd, db, 0, NULL, 0))
	{
		string errmsg = mysql_error(mysql_);
		close();
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}
}

void MysqlDB::close()
{
	if (NULL != mysql_)
	{
		mysql_close(mysql_);
		mysql_ = NULL;
	}
}

MysqlRecordset MysqlDB::querySQL(const char* sql)
{
	if (mysql_query(mysql_, sql) != 0)
	{
		//int errno = mysql_errno(mysql_);
		string errmsg = mysql_error(mysql_);
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}

	MYSQL_RES*  mysql_res;
	mysql_res = mysql_store_result(mysql_);

	//得到查询返回的行数
	//unsigned long n = mysql_affected_rows(mysql_);	

	//指向  mysql 的查询字段集
	MYSQL_FIELD* mysql_field = NULL;
	
	MysqlRecordset rs;
	unsigned int i = 0;
	unsigned int nCols = mysql_num_fields(mysql_res);
	while ((mysql_field = mysql_fetch_field(mysql_res)) != NULL)
	{
		MysqlRecordset::FIELD field;
    // 将name和index信息设置好，用来供MysqlRecordset查找
		field.name = mysql_field->name;
		field.index = i;
		++i;
		rs.fields_.push_back(field);
	}
 
	MYSQL_ROW mysql_row;
	while ((mysql_row = mysql_fetch_row(mysql_res)))
	{
		MysqlRecordset::ROW row(nCols);
		for (i = 0; i< nCols; ++i)
		{
			row[i] = mysql_row[i] ? mysql_row[i] : "";
		}
		rs.rows_.push_back(row);
		
	}
	

	mysql_free_result(mysql_res);

	return rs;

}

unsigned long long MysqlDB::execSQL(const char* sql)
{
	if (mysql_query(mysql_, sql) != 0)
	{
		//int errno = mysql_errno(mysql_);
		string errmsg = mysql_error(mysql_);
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}

	return mysql_affected_rows(mysql_);

}

void MysqlDB::startTransaction()
{
	if (mysql_query(mysql_, "START TRANSACTION") != 0)
	{
		//int errno = mysql_errno(mysql_);
		string errmsg = mysql_error(mysql_);
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}
}

void MysqlDB::commit()
{
	if (mysql_query( mysql_, "COMMIT") != 0)
	{
		//int errno = mysql_errno(mysql_);
		string errmsg = mysql_error(mysql_);
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}
}

void MysqlDB::rollback()
{
	if (mysql_query(mysql_, "ROLLBACK") == 0)
	{
		//int errno = mysql_errno(mysql_);
		string errmsg = mysql_error(mysql_);
    string tmp = string("DB ERROR:") + errmsg;
		throw Exception(tmp);
	}
}


unsigned long long MysqlDB::getInsertId() const
{
	return mysql_insert_id(mysql_);
}
