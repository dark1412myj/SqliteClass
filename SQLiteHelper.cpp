#include "stdafx.h"
#define SQLITEHELPER_DLL

#include "SQLiteHelper.h"

SQLiteHelper::SQLiteHelper()
{
	db = NULL;
}

SQLiteHelper::~SQLiteHelper()
{
	if(db != NULL)
	{
		sqlite3_close(db);
	}
}

bool SQLiteHelper::openDB(char* path)
{
	int rc = 0;

	//path = "world.db";
	rc = sqlite3_open(path, &db);
	if(SQLITE_OK != rc)
	{
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return false;
	}
	else
	{
		cerr << "Open database successfully" << endl;
		return true;
	}
}

void SQLiteHelper::closeDB()
{
	sqlite3_close(db);
	db = NULL;
}

bool SQLiteHelper::execSQL(char* sql)
{
	int rc = 0;
	char *zErrMsg = NULL;

	char* arg1 = G2U(sql);

	rc = sqlite3_exec(db, arg1, 0, 0, &zErrMsg);

	delete[] arg1;

	if(SQLITE_OK != rc)
	{
		cerr << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return false;
    }
	sqlite3_free(zErrMsg);
	return true;
	
}

Cursor SQLiteHelper::querySQL(char* sql)
{
	Cursor r;
	int nCol,nRow;
	char** pResult;
	char* errmsg;
	char* arg1 = G2U(sql);
	int nResult = sqlite3_get_table(db, arg1, &pResult, &nRow, &nCol, &errmsg);
	delete[] arg1;
	arg1=NULL;
	if(nResult!=SQLITE_OK)
	{
		string s (errmsg);
		sqlite3_free(errmsg);
		throw s;
	}
	sqlite3_free(errmsg);
	int nIndex = nCol;
	for(int i =0 ; i <nCol;++i)
	{
		r.title.push_back(pResult[i]);
	}
	r.sz = nRow;
	r.colcnt = nCol;
	for(int i=0;i<nRow;i++)
    {
		vector<string> tmp;
        for(int j=0;j<nCol;j++)
        {
			char* x = U2G(pResult[nIndex]);
			tmp.push_back(x);
			delete[] x;
            ++nIndex;
        }
		r.V.push_back(tmp);
    }
	//sqlite3_free(pResult);
	return r;
}

//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr)
	{
		delete[] wstr;
	}
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr)
	{
		delete[] wstr;
	}
	return str;
}
