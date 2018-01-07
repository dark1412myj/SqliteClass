#pragma once  
#ifdef SQLITEHELPER_DLL  
#define SQLITEHELPER_API _declspec(dllexport)  
#else  
#define SQLITEHELPER_API  _declspec(dllimport)  
#endif  

#include <stdio.h>
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
using namespace std;

class Cursor;

class SQLITEHELPER_API SQLiteHelper
{
public:
	SQLiteHelper();
	~SQLiteHelper();

	bool openDB(char* path);
	void closeDB();
	bool execSQL(char* sql);
	Cursor querySQL(char* sql);


private:
	sqlite3* db;
};


//UTF-8到GB2312的转换
SQLITEHELPER_API char* U2G(const char* utf8);

//GB2312到UTF-8的转换
SQLITEHELPER_API char* G2U(const char* gb2312);

class Cursor{
	int colcnt;
	int sz;
	vector< string > title;
	vector< vector<string> > V;
public :
	size_t titleSize()
	{
		return colcnt;
	}

	size_t size()
	{
		return sz;
	}

	string getTitle(int nCol)
	{
		return title[nCol];
	}

	int getIndex(string s)
	{
		for(int i = 0 ; i< colcnt ;++i)
		{
			if(title[i]==s)
			{
				return i;
			}
		}
		return -1;
	}

	string getItem(int nRow,int nCol)
	{
		return V[nRow][nCol];
	}

	void show()
	{
		for(size_t i = 0; i<V.size();++i)
		{
			for(size_t j = 0 ; j<V[i].size();++j)
			{
				cout<<V[i][j]<<"  ";
			}
			cout<<endl;
		}
		cout<<endl;
	}

	friend class SQLiteHelper;
};