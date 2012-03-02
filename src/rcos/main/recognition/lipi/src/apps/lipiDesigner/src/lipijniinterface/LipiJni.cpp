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
 * $LastChangedDate: 2011-03-31 16:06:43 +0530 (Thu, 31 Mar 2011) $
 * $Revision: 842 $
 * $Author: mnab $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR: The ink from the digitizer is collected and processed here. 
 *		   Converts the data in java to the data acceptable by C++ code.
 *
 * CONTENTS:
 *			InitializeLipiEngine
 *			NewProjectJNI
 *			LoadProjectJNI
 *			AddStrokeJNI
 *			DeleteStrokeJNI
 *			RecognizeJNI
 *			TrainJNI
 *			Train
 *			CloseProjectJNI
 *			ExitJNI
 *			LoadLipiInterface
 *			UnloadModelData
 *			GetLipiEnginePath
 *			GetAllFunctions
 *			CopyTraceGroup
 *			CopyScreenContext
 *			CreateShapeRecognizer
 *			LoadModelData
 *			UnloadLipiInterface
 *
 * AUTHOR:     Roopa S Teradal
 *
 * DATE:       
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/


#pragma once
#include "LIPIUIController.h"
#include "LipiJni.h"
#include <exception>
#include <stdio.h>
#include <string>
#include "LTKMacros.h"

#ifndef __W32__
#include  <dlfcn.h>
#else
#include <windows.h>
#endif


/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_InitializeLipiEngine
* DESCRIPTION	: Loads lipiengine.dll into memory
* ARGUMENTS		: void
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
JNIEXPORT void JNICALL Java_LIPIUIController_InitializeLipiEngine
  (JNIEnv *env, jobject this_obj)
{
	 
	try
	{
		LoadLipiInterface();
		cout << "DLL LOADED IN MEMORY" << endl;
	}
	catch(...)
	{
			cout << "EXCEPTION INSIDE InitializeLipiEngine" << endl;

	}
}


/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_NewProjectJNI
* DESCRIPTION	: Creates the shape recognizer
* ARGUMENTS		: jstring ProjectName :Name of the to be created
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
JNIEXPORT void JNICALL Java_LIPIUIController_NewProjectJNI
  (JNIEnv *env, jobject this_obj, jstring str_projName)
{
	try
	{
		projName ="";
		projName = env->GetStringUTFChars(str_projName,0);

		cout << "PRoject Name is:" << projName << endl;
			//ListFileManager Initialization
		string temp;
		if(envstring !="")
		{
			 temp = envstring + SEPARATOR + PROJECTS + SEPARATOR + projName +
			 SEPARATOR + CONFIG + SEPARATOR + DEFAULT + SEPARATOR + TRAININGLIST_FILE;
			cout << "TRAINLIST PATH IS " << temp << endl;
		}
		else
			cout << "EXCEPTION INSIDE NEWPROJECTJNI:LIPI_ROOT IS NOT SET" << endl;
		lfm.SetDatabasePath(temp);
		lfm.ReadDatabase(string(FLDSEPERATOR)); 
		CreateShapeRecognizer();
	}
	catch(...)
	{
			cout << "EXCEPTION INSIDE NEWPROJECTJNI" << endl;

	}
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_LoadProjectJNI
* DESCRIPTION	: Loads the project in to memory.
*				  LIPIJNIInterface calls the method (LoadModelData) of LIPITK
* ARGUMENTS		: jstring str_projName :name of the project
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
JNIEXPORT void JNICALL JNICALL Java_LIPIUIController_LoadProjectJNI
  (JNIEnv *env, jobject this_obj,jstring str_projName)
{
	try
	{
		cout << "Inside LoadProjectJNI" << endl;
		projName =NULL;
		cout << "before Instr_projName : %s"<< str_projName << endl;
		projName = env->GetStringUTFChars(str_projName,0);
		cout << "after Instr_projName : %s"<< projName << endl;
		//ListFileManager Initialization
		string temp;
		if(envstring !="")
		{
			cout << "I N ENV LoadProjectJNI" << endl;
			 temp = envstring + SEPARATOR + PROJECTS + SEPARATOR + projName +
			 SEPARATOR + CONFIG + SEPARATOR + DEFAULT + SEPARATOR + TRAININGLIST_FILE;
			cout << "TRAINLIST PATH IS " << temp << endl;
		}
		else
		{
			cout << "OUT ENV LoadProjectJNI" << endl;
			cout << "EXCEPTION INSIDE LOADPROJECTJNI:LIPI_ROOT IS NOT SET" << endl;
		}
		
		cout << "after ENV LoadProjectJNI" << endl;
		lfm.SetDatabasePath(temp);
		lfm.ReadDatabase(string(FLDSEPERATOR));
		CreateShapeRecognizer();

		/// Load the mdt file if it exists
		struct stat stFileInfo;
		temp = envstring + SEPARATOR + PROJECTS + SEPARATOR + projName +
			 SEPARATOR + CONFIG + SEPARATOR + DEFAULT + SEPARATOR + MDT_FILE;
		if(stat(temp.c_str(),&stFileInfo)==0)
		{
			LoadModelData();
			cout << "Dat file Loaded" << endl;
		}
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE LOADPROJECTJNI" << endl;

	}
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_AddStrokeJNI
* DESCRIPTION	: LIPITKJNIInterface calls a method of ListFileManager(AddEntryInTrainedListFile()) to add classid/sample to the list file
*				  LIPITKJNIInterface calls a method of LIPITK (UnloadModelData)
*                 LIPITKJNIInterface calls a method(Train) of LIPITK to create the trained dat file
*                 LIPITKJNIInterface calls a method of LIPITK (LoadModelData)
* ARGUMENTS		: jstring str_ClassID :class id to which to add the stroke 
				  jstring str_sampleaction :add a sample to existing class  or add new class along with the stroke data
*				  jboolean bool_autoTrain :automatic training or manual
*
* RETURNS		: void
* NOTES			: None 
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/

JNIEXPORT void JNICALL Java_LIPIUIController_AddStrokeJNI
  (JNIEnv *env, jobject this_obj,jstring file_path,jstring str_ClassID,jboolean bool_autoTrain)
{

	 try
	 {
		 bool autotrain = (bool)bool_autoTrain;
		 cout << "VALUE OF autotrain is " << autotrain <<endl;
		 const char* strClassid = env->GetStringUTFChars(str_ClassID,0);
		 const char* strFilepath = env->GetStringUTFChars(file_path,0);
		 string strFPath;
		 string classidStr;
		 classidStr = strClassid;
		 strFPath = strFilepath;

		 int classid = atoi(strClassid);
		 cout << "STRFPATH NAME IS: " << strFPath <<endl << "CLASSID IS " << classid <<endl;

		 lfm.AddClass(classidStr,strFPath); 
		 lfm.UpdateDatabase();

		 if(autotrain && lfm.RecordSize()>0)
		 {
			 Train();
		 }
		 else
		 cout << "MDT SIZE IS NULL" << endl;
		 
	 }
	 catch(...)
	 { 

		cout << "EXCEPTION INSIDE ADDSTROKEJNI" << endl;
	 }

}


/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_DeleteStrokeJNI
* DESCRIPTION	: calls a method of ListFileManager(DeleteEntryFromListFile()) to delete classid/sample from the list file
*			      and re-arrange the class Ids
*				  LIPITKJNIInterface calls a method of LIPITK (UnloadModelData)
*				  LIPITKJNIInterface calls a method of LIPITK (Train)to create the trained dat file
*				  LIPITKJNIInterface calls a method of LIPITK (LoadModelData)
* ARGUMENTS		: jstring classId :class id to which to delete the stroke
*				  jstring sampleID :sample id to be deleted
*				  jstring sampleclassaction :delete a sample from existing class 
*				  jboolean autoTrain :automatic training or manual
*
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/

JNIEXPORT void JNICALL Java_LIPIUIController_DeleteStrokeJNI
  (JNIEnv *env, jobject this_obj,jstring str_ClassID,jstring file_Path, jstring str_SampleID, jstring str_sampleclassaction,jboolean bool_autoTrain )
{
		
	try
	{
			bool autotrain = (bool)bool_autoTrain;
			const char* strSampleaction = env->GetStringUTFChars(str_sampleclassaction,0);
			const char* strClassid = env->GetStringUTFChars(str_ClassID,0);
			const string strfile_Path = env->GetStringUTFChars(file_Path,0);
			int classid = atoi(strClassid);
			string strFPath;
			string classidStr;

			classidStr = strClassid;
			strFPath = strfile_Path;
			if(LTKSTRCMP(strSampleaction, DELETECLASS_ACTION) == 0)
			{
			lfm.PurgeClass(classidStr); 
			}

			if(LTKSTRCMP(strSampleaction, DELETESAMPLE_ACTION) == 0)
			{		
			lfm.DeleteClass(classidStr,strFPath); 
			}
			lfm.UpdateDatabase();
			if(autotrain && lfm.RecordSize()>0)
			{
			Train();
			}
			else
			cout << "MDT SIZE IS NULL" << endl;
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE DELETESTROKE" << endl;
	}
}


/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_RecognizeJNI
* DESCRIPTION	: to  recognize the stroke drawn by the user
* ARGUMENTS		: jobject inkData :stroke data to be recognized
* RETURNS		: array of class ids along with the confidence
* NOTES			: 
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/

JNIEXPORT jobjectArray JNICALL Java_LIPIUIController_RecognizeJNI
  (JNIEnv *env, jobject this_object, jobject strokeobj)
{

	try{
			LTKTraceGroup oTraceGroup;
			envlocal = env;
		//get the class
			jclass classXY = env->GetObjectClass(strokeobj);
						if(classXY==NULL)
							cout << "CLASS ID IS NULL";
				
			jmethodID  id_stroke = env->GetMethodID(classXY,"getStrokeSize","()I");
						if(id_stroke==NULL)
								cout << "getJNIStroke ID IS NULL";
			numstrokes = env->CallIntMethod(strokeobj,id_stroke);
						if(numstrokes ==0)
							cout << "numstrokes ID IS NULL";
					
			jmethodID  getstroke_id = env->GetMethodID(classXY,"getStroke","()[LStroke;");
						if(getstroke_id==NULL)
							cout << "getstroke_id ID IS NULL";

			jobjectArray StrokeArray_local = env->NewObjectArray(numstrokes,classXY,NULL);
							if(StrokeArray_local==NULL)
							cout << "StrokeArray_local ID IS NULL" << endl;
					
			jobjectArray StrokeArray = (jobjectArray) env->CallObjectMethod(strokeobj, getstroke_id, StrokeArray_local);
						if(StrokeArray==NULL)
							cout << "StrokeArray ID IS NULL" << endl;
						
			jclass StrokeClass = env->FindClass("LStroke;");
							if(StrokeClass==NULL)
							cout << "StrokeClass ID IS NULL" << endl;
					
			CoordClass = env->FindClass("LPoint;");
							if(CoordClass==NULL)
							cout << "StrokeClass ID IS NULL" << endl;
					
			for(int i=0;i<numstrokes;i++)
		 {


					jobject SingleStrokeObj = env->GetObjectArrayElement(StrokeArray,i);
					
					jmethodID  id_stroke = env->GetMethodID(StrokeClass,"getStrokeLength","()I");
							if(id_stroke==NULL)
							{
									cout << "getStrokeLength ID IS NULL " << endl;
									break;
							}
							
				ArrayStrokeSize = env->CallIntMethod(SingleStrokeObj,id_stroke);
				if(ArrayStrokeSize==0)
							cout << "ArrayStrokeSize ID IS NULL" << endl;
						else
							cout << "ArrayStrokeSize ID IS NOT NULL " << ArrayStrokeSize << endl;
				for(int k=0;k<ArrayStrokeSize;k++)
				{
					jmethodID  getCoOrdMethod_id = env->GetMethodID(StrokeClass,"getPoints","()[LPoint;");
						if(getCoOrdMethod_id==NULL)
						{
							cout << "getCoOrdMethod_id ID IS NULL";
							break;
						}

					jobjectArray CoordArray_local = env->NewObjectArray(ArrayStrokeSize,CoordClass,NULL);
						if(CoordArray_local==NULL)
						{
							cout << "CoordArray_local ID IS NULL" << endl;
							break;
						}
								
					CoordArray = (jobjectArray) env->CallObjectMethod(SingleStrokeObj, getCoOrdMethod_id, CoordArray_local);
						if(CoordArray==NULL)
						{
							cout << "CoordArray ID IS NULL" << endl;
							break;
						}
					
					jobject StrokeEle = env->GetObjectArrayElement(CoordArray,k);
						if(StrokeEle==NULL)
						{
							cout << "StrokeEle ID IS NULL" <<endl;
							break;
						}
														
					 getgetX_id = env->GetMethodID(CoordClass,"getXPoint","()I");

						if(getgetX_id==NULL)
						{
							cout << "getgetX_id ID IS NULL";
							break;
						}
					jint StrokeXele = env->CallIntMethod(StrokeEle,getgetX_id);

					getgetY_id = env->GetMethodID(CoordClass,"getYPoint","()I");

						if(getgetY_id==NULL)
						{
							cout << "getgetY_id ID IS NULL";
							break;
						}
					jint StrokeYele = env->CallIntMethod(StrokeEle,getgetY_id);
				}
				CopyTraceGroup(oTraceGroup);	

			}

		    /**
			*Vector<bool> in LIPITK V1 is changed to 
			*Vector<int> in LIPITK V2
			*/
			vector<int> oSubSetOfClasses;
			vector<LTKShapeRecoResult> oResultSet;
			//LTKTraceGroup oTraceGroup;
			LTKScreenContext oScreenContext;
			LTKCaptureDevice ltkcapturedevice;
		    /**
			*setXdpi in LIPITK V1 is changed to 
			*setXDPI in LIPITK V2
			*/
			ltkcapturedevice.setXDPI(100);
		    /**
			*setYdpi in LIPITK V1 is changed to 
			*setYDPI in LIPITK V2
			*/
			ltkcapturedevice.setYDPI(100);
			//CopyTraceGroup(oTraceGroup);	
			CopyScreenContext(oScreenContext);

			if(pShapeReco)
			{
				if(lfm.RecordSize()>0)
				{
					int dev_context = pShapeReco->setDeviceContext(ltkcapturedevice); 
		    /**
			* Confidence value is changed from 0.99f to 
			* 0.002f
			*/
					int iResult = pShapeReco->recognize(oTraceGroup, oScreenContext, oSubSetOfClasses, CONFIDENCE_THRESHOLD, NUMOFCHOICES, oResultSet);
				}
			}
			else
				cout << "Shape Reco is NULL" << endl;
			
			jclass resultclass = env->FindClass("LipitkResult");

     		jfieldID ID = env->GetFieldID(resultclass, "Id", "I");
    		jfieldID confidence = env->GetFieldID(resultclass, "Confidence", "F");
			
		    /**
			*oResultSet.size() is used instead of 
			*iNumofChoices to avoid "Access violation error" if the number of results is less than the num of choices
			*/
			ResultSetArray = env->NewObjectArray(oResultSet.size(),resultclass,NULL);
			jmethodID oneCtorID = env->GetMethodID(resultclass, "<init>", "()V");
		  
		    /**
			*oResultSet.size() is used instead of 
			*iNumofChoices to avoid "Access violation error" if the number of results is less than the num of choices
			*/
			for(int k=0;k<(oResultSet.size());k++)
		 {
			jobject obj = env->NewObject(resultclass,oneCtorID);
			env->SetIntField(obj, ID,oResultSet[k].getShapeId());
			env->SetFloatField(obj, confidence,oResultSet[k].getConfidence());
			env->SetObjectArrayElement(ResultSetArray,k,obj);
			cout << "Num:" <<k <<"ID:" <<oResultSet[k].getShapeId()<<"Confidence:"<<oResultSet[k].getConfidence() << endl;
			obj = NULL;        
		 }
		

	}
	catch(exception e)
	{
				cout << "EXCEPTION INSIDE RECOGNIZEJNI" <<endl;
	}

	return ResultSetArray;
}


/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_TrainJNI
* DESCRIPTION	: LIPIJNIInterface calls the method of LIPITK (UnLoadModelData)
				  LIPIJNIInterface calls the method of LIPITK (Train)
				  LIPIJNIInterface calls the method of LIPITK (LoadModelData)
* ARGUMENTS		: 
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/

JNIEXPORT void JNICALL Java_LIPIUIController_TrainJNI
  (JNIEnv *env, jobject this_obj)
{
	try
	{
		if(lfm.RecordSize()>0)
		Train();
		else
		cout << "MDT SIZE IS NULL" << endl;
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE TRAINJNI" << endl;
	}


}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Train
* DESCRIPTION	: calls the method of LIPITK (UnLoadModelData)
				  calls the method of LIPITK (Train)
				  calls the method of LIPITK (LoadModelData)
* ARGUMENTS		: void
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
void Train()
{

	try
	{

		string strHeader1 = "";
		string comment1 = "";
		string  dataset1 = "";
		string strTrainLstFile = envstring + SEPARATOR + PROJECTS + SEPARATOR + projName +
		SEPARATOR + CONFIG + SEPARATOR + DEFAULT + SEPARATOR + TRAININGLIST_FILE;
		string strHeader = strHeader1;
		string comment = comment1;
		string dataset = dataset1;
		if(pShapeReco)
		{
			UnloadModelData();
			int iErrorCode = pShapeReco->train(strTrainLstFile, strHeader, comment, dataset);
			if(iErrorCode != 0)
			{
				cout << "ERROR WHILE TRAINING CODE " << iErrorCode << endl;
			}
			LoadModelData();
		}
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE TRAIN";
	}

}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_CloseProjectJNI
* DESCRIPTION	: UnLoads lipiengine.dll from memory
				  Unloads the LIPIJNIInterface dll from memory
* ARGUMENTS		: project to be Closed(path of the project name will be passed)
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
JNIEXPORT void JNICALL Java_LIPIUIController_CloseProjectJNI
  (JNIEnv *env, jobject this_obj)
{
	try
	{
		UnloadModelData();
		cout << "CLOSE PROJECT DONE ****&&&" << endl;
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE CLOSEPROJECTJNI";
	}
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: Java_LIPIUIController_ExitJNI
* DESCRIPTION	: UnLoads lipiengine.dll from memory
				  Unloads the LIPIJNIInterface dll from memory
* ARGUMENTS		: 
* RETURNS		: void
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
JNIEXPORT void JNICALL Java_LIPIUIController_ExitJNI
  (JNIEnv *, jobject)
{
	try
	{
		UnloadLipiInterface();
		cout << "EXIT DONE ****&&&" <<endl;
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE EXITJNI" << endl;
	}
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: LoadLipiInterface
* DESCRIPTION	: Initializes Lipi Engine
* ARGUMENTS		: void
* RETURNS		: int
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
LIPIJNIINTERFACE_API int LoadLipiInterface()
{
	try
	{
		char AlgoDLL[MAX_PATH_LEN];
		// Get the lipiengine.dll path
		GetLipiEnginePath(AlgoDLL);
		cout << "LIPITK PATH IS "<< AlgoDLL << endl;
		if (InitializeLipiEngine(AlgoDLL) == -1)
		{
			cout << "InitializeLipiEngine failed" << endl;
			return 1;
		}
		
	}
	catch(...)
	{
		cout << "EXCEPTION INSIDE LOADLIPIINTERFACE";
	}
	return 0;
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: LoadLipiInterface
* DESCRIPTION	: Unloads Shape Recognizer
* ARGUMENTS		: void
* RETURNS		: int
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
LIPIJNIINTERFACE_API int UnloadModelData()
{
	try
	{
		if(pShapeReco != NULL)
		{	
			pShapeReco->unloadModelData();
		}
		else
		{
			cout << "PSHAPERECO IS NULL" << endl;
			return -1;
		}
	}

	catch(...) {

		cout << "EXCEPTION INSIDE UNLOADMODELDATA" << endl;
	}

	return 0;
	
}


/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: GetLipiEnginePath
* DESCRIPTION	: Returns the path of the lipiengine dll
* ARGUMENTS		: lipiEnginePath - contains the lipiengine dll path when returned
* RETURNS		: 0 for SUCCESS & -1 else 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
int GetLipiEnginePath(char *lipiEnginePath)
{
	
	try
	{
		//Get the LIPI_ROOT environment variable 
		envstring = getenv(LIPIROOT_ENV_STRING);
		if(envstring == "")
			return -1;
		
		//Compose the lipiengine module path using the environment path string
#ifdef _WIN32
		sprintf(lipiEnginePath, "%s%slib%s%s%s", envstring.c_str(), SEPARATOR, SEPARATOR, LIPIENGINE_MODULE_STR, DLL_EXT);
#else
		sprintf(lipiEnginePath, "%s%slib%slib%s%s", envstring.c_str(), SEPARATOR, SEPARATOR, LIPIENGINE_MODULE_STR, DLL_EXT);
#endif
		
		cout << "Env Path Is " << lipiEnginePath << endl;
	}

	catch(...) {

		cout << "EXCEPTION INSIDE GETLIPIENGINEPATH" << endl;
	}

	return 0;
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: InitializeLipiEngine
* DESCRIPTION	: Loads the lipiengine dll
* ARGUMENTS		: algoDllName- contains the lipiengine dll path when returned
* RETURNS		: 0 for SUCCESS & -1 else 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
int InitializeLipiEngine(char* algoDllName)
{
	try
	{
		
     	hLipiModule = LTKLoadDLL(algoDllName);
		
		if(hLipiModule == NULL)
			return -1;

		if (GetAllFunctions()  != 0)
			return -1;


		//create an instance of LipiEngine Module
		if(ptrObj == NULL)
		{	
			ptrObj = fptrLTKLipiEngine();
		    /**
			*Lipiroot is set before the initialization of lipiengine 
			*to resolve load exception
			*/
			ptrObj->setLipiRootPath(envstring.c_str());
			int iResult = ptrObj->initializeLipiEngine();	
			if(iResult == -1)
				return -1;
		}
		
	}
	catch(...) 
	{
			cout << "EXCEPTION INSIDE : INITIALIZELIPIENGINE";
	}
	return 0;
}


/**********************************************************************************
* AUTHOR		: Anuj Garg
* DATE			: 19-DEC-2005
* NAME			: GetAllFunctions
* DESCRIPTION	: Local helper function to retrieve all standard exported functions
*				  from the algorithm DLL. i.e. PCA.Dll.
* ARGUMENTS		: None
* RETURNS		: returns 0 on success and -1 on any error. 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description of change
*
* ************************************************************************************/
int GetAllFunctions()
{

	try
	{
		if(hLipiModule != NULL)
		{
			
			fptrLTKLipiEngine = (FN_PTR_LTKLIPIENGINE) LTKGetDLLFunc(hLipiModule, CREATE_LTK_LIPIENGINE);
			if(fptrLTKLipiEngine == NULL)
				return -1;

		    /**
			*getCurrentVersion() in LIPITK V1 is changed to 
			*getToolkitVersion() in LIPITK V2
			*/	
			fptrLipiGetCurrentVer = (FN_PTR_LIPIGETCURRENTVER) LTKGetDLLFunc(hLipiModule, GET_ToolKit_VERSION);
			if(fptrLipiGetCurrentVer == NULL)
				return -1;
		
		}
	}
	catch(...)
	{
			cout << "EXCEPTION INSIDE GETALLFUNCTIONS" << endl;
	}
	return 0;
	
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: CopyTraceGroup
* DESCRIPTION	: 
* ARGUMENTS		: 
* RETURNS		: 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
************************************************************************************/

int CopyTraceGroup(LTKTraceGroup &oTraceGroup)
{
	try
	{

		vector<LTKChannel> channels;				//	channels of a trace 

		LTKChannel xChannel("X", DT_INT, true);	//	x-coordinate channel of the trace 
		LTKChannel yChannel("Y", DT_INT, true);	//	y-coordinate channel of the trace

		//	initializing the channels of the trace
		channels.push_back(xChannel);	
		channels.push_back(yChannel);

		//	composing the trace format object
		LTKTraceFormat traceFormat(channels);

		vector<float> point;				//	a point of a trace
		LTKTrace trace(traceFormat); 
		for(int i = 0; i < ArrayStrokeSize; i++)
		{

			jobject StrokeEle = envlocal->GetObjectArrayElement(CoordArray,i);
			jmethodID  getgetX_id = envlocal->GetMethodID(CoordClass,"getXPoint","()I");
			jint StrokeXele = envlocal->CallIntMethod(StrokeEle,getgetX_id);
			jint StrokeYele = envlocal->CallIntMethod(StrokeEle,getgetY_id);
			point.push_back(StrokeXele);
			point.push_back(StrokeYele);
			trace.addPoint(point);
			point.clear();

		}
		oTraceGroup.addTrace(trace);
		
	}
	catch(...) 
	{
			cout << "EXCEPTION INSIDE COPYTRACEGROUP" << endl;
	}
	return 0;
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: CopyScreenContext
* DESCRIPTION	: 
* ARGUMENTS		: 
* RETURNS		: 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
************************************************************************************/
int CopyScreenContext(LTKScreenContext oScreenContext)
{
	try 
	{
		oScreenContext.setBboxLeft(150);
		oScreenContext.setBboxBottom(300);
		oScreenContext.setBboxRight(212);
		oScreenContext.setBboxTop(248);
	}
	catch(...) 
	{
		cout << "EXCEPTION INSIDE COPYSCREENCONTEXT" <<endl;
	}
	return 0;
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: CreateShapeRecognizer
* DESCRIPTION	: Creates the Shape Recognizer object
* ARGUMENTS		: None
* RETURNS		: 0 for SUCCESS & -1 else 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
*************************************************************************************/
int CreateShapeRecognizer()
{
	try
	{
		int iResult = 0;
		string sProjectName;
		sProjectName =projName;
		string strProfileName = DEFAULT;
		cout << "Create Shape rec sProjectName is " << sProjectName.c_str() << endl;
		    /**
			*CreateShape recognizer in LIPITK V2
			*takes different parameters than in LIPITK V1
			*/	
		if(ptrObj == NULL)
			cout << "ptrObj is NULL" << endl;
		iResult = ptrObj->createShapeRecognizer(sProjectName, strProfileName, &pShapeReco);
		if(iResult != SUCCESS)
		{
			
				cout << "pShapeReco == NULL";
			return -1;
		}
		
	}
	catch(...) 
	{
		cout << "EXCEPTION INSIDE LOADLIPIENGINEMODULE" << endl;
	}
	return 0;
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: LoadModelData
* DESCRIPTION	: Calls the Shape Recognizer's LoadModalData() method to load data
* ARGUMENTS		: None
* RETURNS		: 0 for SUCCESS & -1 else 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
* Balaji MNA        31st March 2011     Calling deleteShapeRecognizer method from  
*                                       LTKLipiEnigneInterface is modified in lipi toolkit
*************************************************************************************/
int LoadModelData()
{
	
	try
	{

		int iResult = 0;

		//Load the model data into memory before starting the recognition...
		{
			iResult = pShapeReco->loadModelData();
			if(iResult != 0)
			{	
				cout << "LoadModelData - failed pShapeReco";

				/**
				*deleteShapeRecognizer() in LIPITK V2  
				* takes different parameters
				*/	
				ptrObj->deleteShapeRecognizer(pShapeReco);
				return -1;
			}
		}
		
	}
	catch(...) 
	{
		cout << "EXCEPTION INSIDE LOADMODELDATA" << endl;
	}
	return 0;
}

/**********************************************************************************
* AUTHOR		: Roopa S Teradal
* DATE			: 
* NAME			: LoadModelData
* DESCRIPTION	: Calls the Shape Recognizer's deleteShapeRecogizer() method 
* ARGUMENTS		: None
* RETURNS		: 0 for SUCCESS & -1 else 
* NOTES			: None
* CHANGE HISTROY
* Author			Date				Description
* Balaji MNA        31st March 2011     Calling deleteShapeRecognizer method from  
*                                       LTKLipiEnigneInterface is modified in lipi toolkit
*************************************************************************************/
int UnloadLipiInterface()
{
	try
	{
		// This statement will crash the DLL in Debug mode due to problem of STL patch.
		// Comment next two lines when build in debug mode. 
					
		// Delete the shape recognizer object

		/**
		*deleteShapeRecognizer() in LIPITK V2  
		* takes different parameters
		*/	
		if (pShapeReco != NULL && ptrObj != NULL)
			ptrObj->deleteShapeRecognizer(pShapeReco);
			
		LTKUnloadDLL(hLipiModule);
		hLipiModule = NULL;
	}
	catch(...) {

		cout << "EXCEPTION INSIDE UNLOADLIPIINTERFACE" << endl;
	}

	return 0;
}
