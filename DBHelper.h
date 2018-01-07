#pragma once
#include "stdafx.h"
#include "SQLiteHelper.h"
#include "cstdio"
#include "sstream"
#include "EventData.h"
#include "RecordData.h"
using namespace std;
class CDBHelper
{
	SQLiteHelper db;
	CDBHelper(void)
	{
		db.openDB("myDB.db3");
	}
	virtual ~CDBHelper(void)
	{
		db.closeDB();
	}
public:
	static CDBHelper& getInstance()
	{
		static CDBHelper instance;
		return instance;
	}

	BOOL UpdateRecord(string id, string name, string duration, string end, int flag)
	{
		CString sql;
		sql.Format("update Record set _Name = '%s' , _Duration = %s ,_EndData ='%s',_flag = %d where _ID = %s;", name.c_str(), duration.c_str(), end.c_str(), flag,id.c_str());
		return db.execSQL(sql.GetBuffer());
	}

	//删除一条记录
	BOOL DeleteRecord(string id)
	{
		CString sql;
		sql.Format("delete from Record where _ID = %s;",id.c_str());
		return db.execSQL(sql.GetBuffer());
	}
	//flag标记字段，把字符串映射成int
	BOOL InsertRecord(string name,int duration,string enddata,int flag)
	{
		CString sql;
		sql.Format("insert into Record values(null,'%s',%d,'%s',%d);",name.c_str(),duration,enddata.c_str(),flag);
		return db.execSQL(sql.GetBuffer());
	}

	vector<CRecordData> GetRecord(string start, string end ,string arg)
	{
		vector<CRecordData> V;

		CString sql;
		sql.Format("select * from Record where _flag >= %s and _flag <= %s and _Name = '%s';", start.c_str(), end.c_str(),arg.c_str());
		//AfxMessageBox(sql);
		Cursor sr = db.querySQL(sql.GetBuffer());

		int __id = sr.getIndex("_ID");
		int __name = sr.getIndex("_Name");
		int __duration = sr.getIndex("_Duration");
		int __enddata = sr.getIndex("_EndData");
		//AfxMessageBox("22");
		CRecordData date;
		//date.standingtime ="...";
		for (size_t i = 0; i<sr.size(); ++i)
		{
			date.id = sr.getItem(i, __id);
			date.name = sr.getItem(i, __name);
			date.standingtime = sr.getItem(i, __duration);
			date.endtime = sr.getItem(i, __enddata);
			V.push_back(date);
		}

		return V;
	}

	vector<CRecordData> GetRecord(string start,string end)
	{
		vector<CRecordData> V;

		CString sql;
		sql.Format("select * from Record where _flag >= %s and _flag <= %s;",start.c_str(),end.c_str());
		//AfxMessageBox(sql);
		Cursor sr = db.querySQL(sql.GetBuffer());
		 
		int __id = sr.getIndex("_ID");
		int __name = sr.getIndex("_Name");
		int __duration = sr.getIndex("_Duration");
		int __enddata = sr.getIndex("_EndData");
		//AfxMessageBox("22");
		CRecordData date;
		//date.standingtime ="...";
		for(size_t i = 0; i<sr.size();++i)
		{
			date.id = sr.getItem(i,__id);
			date.name = sr.getItem(i,__name);
			date.standingtime =sr.getItem(i,__duration);
			date.endtime = sr.getItem(i,__enddata);
			V.push_back(date);
		}

		return V;
	}

	BOOL InsertEvent(string id,string name)
	{
		CString sql;
		sql.Format("insert into Event values('%s','%s');",id.c_str(),name.c_str());
		return db.execSQL(sql.GetBuffer());
	}

	BOOL DeleteEvent(string id)
	{
		CString sql;
		sql.Format("delete from Event where _id='%s';",id.c_str());
		return db.execSQL(sql.GetBuffer());
	}

	vector<CEventData> GetAllEvent()
	{
		vector<CEventData> V;

		CString sql;
		sql.Format("select * from Event;");

		Cursor sr = db.querySQL(sql.GetBuffer());
		
		int __id = sr.getIndex("_ID");
		int __name = sr.getIndex("_Name");
		CEventData data;
		for(size_t i = 0; i<sr.size();++i)
		{
			data.id = sr.getItem(i,__id);
			data.name = sr.getItem(i,__name);
			V.push_back(data);
		}
		return V;
	}

};

