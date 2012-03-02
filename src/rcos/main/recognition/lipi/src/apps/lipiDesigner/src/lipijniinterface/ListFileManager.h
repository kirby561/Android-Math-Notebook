/*****************************************************************************************
 * Copyright (c) 2009 Hewlett-Packard Development Company, L.P.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************************/
 /************************************************************************
 * FILE DESCR: This is  ListFiles header file.
 *			   
 * CONTENTS:
 *	
 * AUTHOR:     Anuj Garg
 * DATE:       October 3, 2006
 * CHANGE HISTORY:
 * Author		Date			Description of change
 *
 ************************************************************************/
#pragma once
#include <list>
#include <fstream>
#include <string>
#include "LTKMacros.h"
#include "LTKStringUtil.h"
using namespace std;

#define MAP_FILE_NAME "MapFile.ini"
#define UNICODE_MAP_FILE_NAME "unicodeMapFile.cfg"

class ListFileManager
{
private:
	class LIST {
		int m_nClassId;
		string m_sPath;
		public:
		LIST() {
			m_nClassId = -1; 
		}
		~LIST(){}
		int GetClassId()				{	return m_nClassId;		}
		string GetPath()					{	return m_sPath;			}
		void StoreClassId(int clsId)	{	m_nClassId = clsId;		}
		void StorePath(string pPath) 	{	m_sPath = pPath;	}
	};
	class MAPPING_LIST {
		int m_nClassId;
		string m_sClassName;
		public:
		MAPPING_LIST() {
			m_nClassId = -1;
			m_sClassName = ""; 
		}
		~MAPPING_LIST(){}
		int GetMappedId()					{	return m_nClassId;				}
		string GetMappedName()				{	return m_sClassName;			}
		void MapClassId(int clsId)			{	m_nClassId = clsId;				}
		void MapClassName(string& pClsName) 	{	m_sClassName = pClsName;	}
	};

	list<LIST> *m_pList;
	list<MAPPING_LIST> *m_pMapList;
	
	string m_sDBFileName; 
	string m_sMapFileName; 
	string m_sunicodeMapFileName; 

	string m_sSeperator;
	string m_sEqual;
	void SetFieldSeperator(string& fldSeperator);
	void SortRecords();
	void ResuffleList();
	int GetClassId(string& pClsName, bool bCreateNewId);
	bool UpdateClassFile();
	bool UpdateListFile();
	bool UpdateUnicodeMapFile();
	void ReadListFile();
	void ReadClassFile();
	string GetDatabasePath();
	void AddRecord(int clsId, string& pString);
	bool PurgeClassId(int clsId);
	bool DeleteRecord(int clsId, string& pString);
public:
	ListFileManager();
	~ListFileManager();

	void ReadDatabase(string fldSeperator);
	void SetDatabasePath(string& pPath);
	void AddClass(string& pClsName, string& pString);
	bool PurgeClass(string& pClsName);
	bool DeleteClass(string& pClsName, string& pString);
	void ClearRecordset();
	bool UpdateDatabase();
	void DisplayRecords();
	int RecordSize();
};

