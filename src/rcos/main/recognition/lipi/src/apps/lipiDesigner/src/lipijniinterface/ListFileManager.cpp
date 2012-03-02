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
/******************************************************************************
 * SVN MACROS
 *
 * $LastChangedDate: 2011-08-23 13:42:01 +0530 (Tue, 23 Aug 2011) $
 * $Revision: 880 $
 * $Author: jitender $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR: This is  ListFiles implementation file.
 *			   
 * CONTENTS:
 *			SetDatabasePath
 *			GetDatabasePath
 *			SetFieldSeperator
 *			ReadListFile
 *			ReadClassFile
 *			ReadDatabase
 *			UpdateListFile
 *			UpdateClassFile
 *			UpdateDatabase
 *			AddRecord
 *			PurgeClassId
 *			DeleteRecord
 *			ResuffleList
 *			SortRecords
 *			DisplayRecords
 *			ClearRecordset
 *			GetClassId
 *			AddClass
 *			PurgeClass
 *			DeleteClass
 *			RecordSize
 *			
 * AUTHOR:     Anuj Garg
 * DATE:       October 3, 2006
 * CHANGE HISTORY:
 * Author		Date			Description of change
 *
 ************************************************************************/


//#include "LTKInc.h"
#include "LTKMacros.h"

#include "ListFileManager.h"

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ListFileManager
* DESCRIPTION	: constructor
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
ListFileManager::ListFileManager()
{
	m_pList		= new list<LIST>;
	m_pMapList	=  new list<MAPPING_LIST>;
	m_sEqual = "=";
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ~ListFileManager
* DESCRIPTION	: destructor
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
ListFileManager::~ListFileManager()
{
	m_pList->clear();
	delete m_pList;
	m_pMapList->clear();
	delete m_pMapList;
}
/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: SetDatabasePath
* DESCRIPTION	: Sets the path of the database 
* ARGUMENTS		: path - path of the training list file
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::SetDatabasePath(string& pPath)
{
	m_sDBFileName = pPath;
	m_sMapFileName = pPath;
	m_sunicodeMapFileName = pPath;
	int last_end = m_sMapFileName.find_last_of(SEPARATOR);
	int len = m_sMapFileName.length() - last_end;
	m_sMapFileName = pPath.substr(0, last_end);
	m_sMapFileName.append(SEPARATOR);
	m_sMapFileName.append(MAP_FILE_NAME);
    last_end = m_sunicodeMapFileName.find_last_of(SEPARATOR);
	m_sunicodeMapFileName = pPath.substr(0, last_end);
	last_end = m_sunicodeMapFileName.find_last_of(SEPARATOR);
	m_sunicodeMapFileName = pPath.substr(0, last_end);
	m_sunicodeMapFileName.append(SEPARATOR);
	m_sunicodeMapFileName.append(UNICODE_MAP_FILE_NAME);
	m_pList->clear();
	m_pMapList->clear();
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: GetDatabasePath
* DESCRIPTION	: Gets the path of the database 
* ARGUMENTS		: None
* RETURNS		: m_sDBFileName - path of the training list file
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
string ListFileManager::GetDatabasePath()
{
	return m_sDBFileName;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: SetFieldSeperator
* DESCRIPTION	: sets the field seperator to be used in the map file 
* ARGUMENTS		: fldSeperator - fileld seperator to be used in map file
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::SetFieldSeperator(string& fldSeperator)
{
	m_sSeperator = fldSeperator;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ReadListFile
* DESCRIPTION	: Reads the training list file
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None 
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::ReadListFile()
{
	fstream ifile(m_sDBFileName.c_str(), ios::in);
	if (ifile.fail()) 
		return;
	string output = "";
	vector<string> token;
	do{
		getline(ifile, output,'\n');
		if(output.length() > 0)	{
			LTKStringUtil::tokenizeString(output,m_sSeperator,token);
			if(token.size() > 0){
				LIST *pRec = new LIST;
				pRec->StorePath(token[0]);
					pRec->StoreClassId(atoi(token[1].c_str()));	
 					m_pList->insert(m_pList->begin(), *pRec);			
			}
		}
	}while(!ifile.eof());
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ReadClassFile
* DESCRIPTION	: Reads the Map file
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::ReadClassFile()
{
	fstream ifile(m_sMapFileName.c_str(), ios::in);
	if (ifile.fail()) 
		return;
	string output;
	vector<string> token;
	do{
		getline(ifile, output,'\n');
		if(output.length() > 0)	{
			LTKStringUtil::tokenizeString( output, m_sSeperator, token );
			if(token.size() != 0){
				MAPPING_LIST *pRec = new MAPPING_LIST;
				pRec->MapClassName(token[0]);	
					pRec->MapClassId(atoi(token[1].c_str()));					
 					m_pMapList->insert(m_pMapList->end(), *pRec);			
			}
		}
	}while(!ifile.eof());
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ReadDatabase
* DESCRIPTION	: calls the ReadListFile() & ReadClassFile() methods
* ARGUMENTS		: fldSeperator - fileld seperator to be used in map file
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::ReadDatabase(string fldSeperator)
{	
	SetFieldSeperator(fldSeperator);
	ReadListFile();
	ReadClassFile();	
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: UpdateListFile
* DESCRIPTION	: Updates the Training List file
* ARGUMENTS		: None
* RETURNS		: true on SUCCES, else false
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::UpdateListFile()
{
	list <LIST>::iterator c1_Iter;
	char buffer[512];
	fstream ofile(m_sDBFileName.c_str(), ios::out);
	if (ofile.fail()) return false;
	if(m_pList->size()==0)
	{
		memset(buffer, 0, sizeof(char));
		sprintf(buffer, "");
		ofile.write(buffer, strlen(buffer));
		ofile.close();
		return true;

	}
	if(m_pList->size() > 0) {
		
		c1_Iter = m_pList->begin();
		for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end(); c1_Iter++) {
			memset(buffer, 0, sizeof(char));
			sprintf(buffer, "%s%s%d",(*c1_Iter).GetPath().c_str(), m_sSeperator.c_str(),  (*c1_Iter).GetClassId());
			ofile.write(buffer, strlen(buffer));
			ofile.write("\n",1);
		}
		ofile.close();
		return true;
	}
	return false;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: UpdateClassFile
* DESCRIPTION	: Updates the Map file
* ARGUMENTS		: None
* RETURNS		: true on SUCCES, else false
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::UpdateClassFile()
{
	fstream ofile(m_sMapFileName.c_str(), ios::out);
	if (ofile.fail()) return false;

	list <MAPPING_LIST>::iterator c1_MapIter;
	char buffer[256];
	int prev_id = -1;
	int next_id = -1;
	
	if(m_pMapList->size()==0)
	{
		memset(buffer, 0, sizeof(char));
		sprintf(buffer, "");
		ofile.write(buffer, strlen(buffer));
		ofile.write("\n",1);
		ofile.close();
		return true;

	}

	memset(buffer, 0, sizeof(char));			
	c1_MapIter = m_pMapList->begin();
	prev_id = (*c1_MapIter).GetMappedId();
	
	sprintf(buffer, "%s%s%d", (*c1_MapIter).GetMappedName().c_str(), m_sSeperator.c_str(), (*c1_MapIter).GetMappedId());
	ofile.write(buffer, strlen(buffer));
	ofile.write("\n",1);
	for(; c1_MapIter != m_pMapList->end(); c1_MapIter++) {
		if((*c1_MapIter).GetMappedId() !=  prev_id){
			memset(buffer, 0, sizeof(char));			
			sprintf(buffer, "%s%s%d", (*c1_MapIter).GetMappedName().c_str(), m_sSeperator.c_str(), (*c1_MapIter).GetMappedId());
			ofile.write(buffer, strlen(buffer));
			ofile.write("\n",1);
			prev_id = (*c1_MapIter).GetMappedId();
		}
	}
	return true;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: UpdateDatabase
* DESCRIPTION	: calls the UpdateListFile() & UpdateClassFile() methods
* ARGUMENTS		: None
* RETURNS		: true on SUCCES, else false
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::UpdateDatabase()
{
	SortRecords();
	if (!UpdateListFile())
		return false;
    
	if(!UpdateClassFile())
		return false;

	if(!UpdateUnicodeMapFile())
		return false;

	return true;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: AddRecord
* DESCRIPTION	: Adds a new sample or a new class into the training list
* ARGUMENTS		: clsId - Id of the class to which sample has to be added
*			  pString - path of the sample's ink data file
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::AddRecord(int clsId, string& pString)
{
	LIST *pRec = new LIST;
	pRec->StoreClassId(clsId);
	pRec->StorePath(pString);
 	m_pList->insert(m_pList->begin(), *pRec);
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: PurgeClassId
* DESCRIPTION	: Deletes a class from the training list
* ARGUMENTS		: clsId - Id of the class which sample has to be deleted
* RETURNS		: true on SUCCES, else false
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::PurgeClassId(int clsId)
{
	SortRecords();
	list <LIST>::iterator c1_Iter, iter1;
	list <MAPPING_LIST>::iterator c1_MapIter;
	if(m_pList->size() > 0) {
		c1_Iter = m_pList->begin();
		for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end(); )	
		{
			if ( (*c1_Iter).GetClassId() == clsId ) 
				c1_Iter =  m_pList->erase(c1_Iter);
			else 
				c1_Iter++;
		}
	}
	if(m_pMapList->size() > 0) {
		c1_MapIter = m_pMapList->begin();
		for(c1_MapIter = m_pMapList->begin(); c1_MapIter != m_pMapList->end(); )	
		{
			if ( (*c1_MapIter).GetMappedId() == clsId ) 
				c1_MapIter =  m_pMapList->erase(c1_MapIter);
			else 
				c1_MapIter++;
		}
	}

	ResuffleList();

	return true;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: PurgeClassId
* DESCRIPTION	: Deletes a sample from the training list
* ARGUMENTS		: clsId - Id of the class who's sample has to be deleted
*				  pString - path of the sample's ink file
* RETURNS		: true on SUCCES, else false
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::DeleteRecord(int clsId, string& pString)
{
	SortRecords();
	list <LIST>::iterator c1_Iter;
	list <MAPPING_LIST>::iterator c1_MapIter;
	int count_rec = 0;
	if(m_pList->size() > 0) {
		c1_Iter = m_pList->begin();
		for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end(); ){
			if ( (*c1_Iter).GetClassId() == clsId ) {
				count_rec++;

				if(((*c1_Iter).GetPath()) == pString )
				{
					c1_Iter =  m_pList->erase(c1_Iter);
				}
				else
						c1_Iter++;
			}else 	c1_Iter++;
		}
	}

	if(count_rec == 1){
		if(m_pMapList->size() > 0) {
			c1_MapIter = m_pMapList->begin();
			for(c1_MapIter = m_pMapList->begin(); c1_MapIter != m_pMapList->end(); ){
				if ( (*c1_MapIter).GetMappedId() == clsId ) 
					c1_MapIter =  m_pMapList->erase(c1_MapIter);
				else
					c1_MapIter++;
			}
		}
	}

	ResuffleList();
	return true;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ResuffleList
* DESCRIPTION	: Resuffles the training list an map files
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::ResuffleList()
{
	list <LIST>::iterator c1_Iter;
	list <MAPPING_LIST>::iterator c1_MapIter;

	if(m_pList->size() > 0)
	{
		int prevId = -1;
		c1_Iter = m_pList->begin();
		prevId = (*c1_Iter).GetClassId();
		if(prevId > 0){
			for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end(); c1_Iter++)	
				(*c1_Iter).StoreClassId((*c1_Iter).GetClassId() - 1);			
		}
		else{
			for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end();)	{
				if ( (*c1_Iter).GetClassId() == prevId ){ c1_Iter++ ; 	continue; }
				if( ((*c1_Iter).GetClassId() - prevId) > 1 ){
					for(; c1_Iter != m_pList->end(); c1_Iter++)	
						(*c1_Iter).StoreClassId((*c1_Iter).GetClassId() - 1);
					break;
				}
				else
					prevId = (*c1_Iter).GetClassId();
			}
		}
	}

	if(m_pMapList->size() > 0)
	{
		int prevId = -1;
		c1_MapIter = m_pMapList->begin();
		prevId = (*c1_MapIter).GetMappedId();
		if(prevId > 0){
			for(c1_MapIter = m_pMapList->begin(); c1_MapIter != m_pMapList->end(); c1_MapIter++)	
				(*c1_MapIter).MapClassId((*c1_MapIter).GetMappedId() - 1);			
		}
		else{
			for(c1_MapIter = m_pMapList->begin(); c1_MapIter != m_pMapList->end();)	{
				if ( (*c1_MapIter).GetMappedId() == prevId ){ c1_MapIter++ ; 	continue; }
				if( ((*c1_MapIter).GetMappedId() - prevId) > 1 ){
					for(; c1_MapIter != m_pMapList->end(); c1_MapIter++)	
						(*c1_MapIter).MapClassId((*c1_MapIter).GetMappedId() - 1);
					break;
				}
				else
					prevId = (*c1_MapIter).GetMappedId();
			}
		}
	}
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: SortRecords
* DESCRIPTION	: Sorts the training list file
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::SortRecords()
{
	list <LIST>::iterator c1_Iter, iter1;
	if(m_pList->size() > 0) {
		c1_Iter = m_pList->begin();
		iter1 = m_pList->begin();
		string tString = ""; 
		for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end(); c1_Iter++){
			iter1 ++;
			while(iter1 != m_pList->end()) {
				if( (*c1_Iter).GetClassId() > (*iter1).GetClassId() )	{
					int id = (*c1_Iter).GetClassId();
					tString = (*c1_Iter).GetPath();
					(*c1_Iter).StoreClassId((*iter1).GetClassId()) ;	
					(*c1_Iter).StorePath((*iter1).GetPath());	
					(*iter1).StoreClassId(id);
				    (*iter1).StorePath(tString);
				}
				iter1++;
			}
			iter1 = c1_Iter;
		}		
	}	
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: DisplayRecords
* DESCRIPTION	: displays all the records in the training list.
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::DisplayRecords()
{
	list <MAPPING_LIST>::iterator c1_MapIter;
	cout << endl <<" Total Map Size " << m_pMapList->size() << endl;
	c1_MapIter = m_pMapList->begin();
	for(c1_MapIter = m_pMapList->begin(); c1_MapIter != m_pMapList->end(); c1_MapIter++)	{
		cout << endl << " ClassID " << c1_MapIter->GetMappedId() << "ClassName " << c1_MapIter->GetMappedName() << endl;
	}
	cout << endl <<" ********************************************************";

	list <LIST>::iterator c1_Iter;
	cout << endl <<" Total Size " << m_pList->size() << endl;
	c1_Iter = m_pList->begin();
	for(c1_Iter = m_pList->begin(); c1_Iter != m_pList->end(); c1_Iter++)	{
		cout << endl << "ID " <<c1_Iter->GetClassId() <<"PATH "<< c1_Iter->GetPath() << endl;
	}
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: ClearRecordset
* DESCRIPTION	: clears the Training list and map files.
* ARGUMENTS		: None
* RETURNS		: None
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::ClearRecordset()
{
	m_pList->clear();
	m_pMapList->clear();
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: GetClassId
* DESCRIPTION	: returns the class id
* ARGUMENTS		: pClsName - name of the class who's id is to be returned
*				  bCreateNewId - creates new id if true, else false
* RETURNS		: int - class id 
* NOTES			: 
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
int ListFileManager::GetClassId(string& pClsName, bool bCreateNewId = true)
{
	list <MAPPING_LIST>::iterator c1_MapIter;
	c1_MapIter = m_pMapList->begin();
	for(c1_MapIter = m_pMapList->begin(); c1_MapIter != m_pMapList->end(); c1_MapIter++)	{

		if(c1_MapIter->GetMappedName() == pClsName )
		{
			return c1_MapIter->GetMappedId();
		}
	}

	if(bCreateNewId){
		MAPPING_LIST *pRec = new MAPPING_LIST;
		if (m_pMapList->size() == 0)
			pRec->MapClassId(0);
		else{
			c1_MapIter = m_pMapList->end();
			c1_MapIter--;
			pRec->MapClassId(c1_MapIter->GetMappedId() + 1);
		}
		pRec->MapClassName(pClsName);
 		m_pMapList->insert(m_pMapList->end(), *pRec);
		if(m_pMapList->size() == 1)
			return 0;
		return c1_MapIter->GetMappedId()+1;
	}
	return -1;
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: AddClass
* DESCRIPTION	: Adds new class or sample into the project
* ARGUMENTS		: pClsName - name of the class to which sample has to be added
*				  pString - path of the sample's ink file
* RETURNS		: None 
* NOTES			: 
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
void ListFileManager::AddClass(string& pClsName, string& pString)
{
	int clsId = GetClassId(pClsName) ;
	AddRecord(clsId, pString);
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: PurgeClass
* DESCRIPTION	: deletes class from the project
* ARGUMENTS		: pClsName - name of the class which has to be deleted
* RETURNS		: true on SUCCES, else false 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::PurgeClass(string& pClsName)
{
	int clsId = GetClassId(pClsName, false) ;
	return PurgeClassId(clsId);
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: DeleteClass
* DESCRIPTION	: deletes sample from the project
* ARGUMENTS		: pClsName - name of the class who's sample has to be deleted
*			      pString - path of the sample's ink data file
* RETURNS		: true on SUCCES, else false 
* NOTES			: None 
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::DeleteClass(string& pClsName, string& pString)
{
	int clsId = GetClassId(pClsName, false);
	return DeleteRecord(clsId, pString);
}

/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: October 3, 2006
* NAME			: RecordSize
* DESCRIPTION	: Returns the number of classes in the project
* ARGUMENTS		: None
* RETURNS		: int - number of classes in the project 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
int ListFileManager::RecordSize()
{
	return (m_pMapList->size());
}

/**********************************************************************************
* AUTHOR		: Balaji MNA
* DATE			: April 1, 2011
* NAME			: UpdateUnicodeMapFile
* DESCRIPTION	: Updates the Map file
* ARGUMENTS		: None
* RETURNS		: true on SUCCES, else false
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description 
*************************************************************************************/
bool ListFileManager::UpdateUnicodeMapFile()
{
	fstream ofile(m_sunicodeMapFileName.c_str(), ios::out);
	if (ofile.fail()) return false;

	list <MAPPING_LIST>::iterator c1_MapIter;
	char buffer[256];
	int prev_id = -1;
	int next_id = -1;
	
	if(m_pMapList->size()==0)
	{
		memset(buffer, 0, sizeof(char));
		sprintf(buffer, "");
		ofile.write(buffer, strlen(buffer));
		ofile.write("\n",1);
		ofile.close();
		return true;

	}

	memset(buffer, 0, sizeof(char));			
	c1_MapIter = m_pMapList->begin();
	prev_id = (*c1_MapIter).GetMappedId();
	
	sprintf(buffer, "%d%s%s", (*c1_MapIter).GetMappedId(), m_sEqual.c_str(), (*c1_MapIter).GetMappedName().c_str());
	ofile.write(buffer, strlen(buffer));
	ofile.write("\n",1);
	for(; c1_MapIter != m_pMapList->end(); c1_MapIter++) {
		if((*c1_MapIter).GetMappedId() !=  prev_id){
			memset(buffer, 0, sizeof(char));			
			sprintf(buffer, "%d%s%s", (*c1_MapIter).GetMappedId(), m_sEqual.c_str(), (*c1_MapIter).GetMappedName().c_str());
			ofile.write(buffer, strlen(buffer));
			ofile.write("\n",1);
			prev_id = (*c1_MapIter).GetMappedId();
		}
	}
	return true;
}