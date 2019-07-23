#pragma once

#include <mysql/mysql.h>

#include <vector>
#include <string>
using namespace std;

namespace DAL
{

class MysqlDB;
// 抽象出来的查询结果集
class MysqlRecordset
{
	friend class MysqlDB;
  static std::string kEmptyString;
public:
  // 根据行列来获得结果集中的某一项
	const string& getItem(unsigned int nRow, unsigned int nCol) const
	{
		return rows_[nRow][nCol];
	}

  // 根据field字段映射找到列，比较常用
	const string& getItem(unsigned int nRow, const string& name) const
	{
		unsigned int index = getFieldIndex(name);
    if(index < 0)
      return kEmptyString;
		return rows_[nRow][index];
	}

	unsigned int getRows() const
	{
		return rows_.size();
	}

	unsigned int getCols() const
	{
		return fields_.size();
	}

	int getFieldIndex(const std::string &name) const
	{
		int index = -1;
		for(unsigned int i = 0; i < fields_.size(); ++i)
		{
			if (fields_[i].name == name)
				index = fields_[i].index;
		}
		return index;
	}

	void clear()
	{
		rows_.clear();
		fields_.clear();
	}

	typedef struct Field
	{
		string name;
		unsigned int index;
	} FIELD;
	typedef vector<FIELD> FIELDS;
	typedef vector<string> ROW;
private:
	vector<ROW> rows_;
	FIELDS fields_;
};

class MysqlDB
{
public:
	MysqlDB();
	~MysqlDB();
	void open(const char* host,
		const char* user,
		const char* passwd,
		const char* db,
		unsigned int port = 0);
	void close();

	unsigned long long execSQL(const char* sql);
	MysqlRecordset querySQL(const char* sql);

  // 和主key的auto_increment有关系，获得自动增长的下一个id
	unsigned long long getInsertId() const;

  // 在执行大量的update和delete的时候，记得要开启事务
	void startTransaction();
	void commit();
	void rollback();


private:
	MYSQL* mysql_;
};

}

