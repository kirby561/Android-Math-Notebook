/*****************************************************************************************
* Copyright (c) 2007 Hewlett-Packard Development Company, L.P.
* Permission is hereby granted, free of charge, to any person obtaining a copy of 
* this software and associated documentation files (the "Software"), to deal in 
* the Software without restriction, including without limitation the rights to use, 
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
* Software, and to permit persons to whom the Software is furnished to do so, 
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
* OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*****************************************************************************************/

/************************************************************************
 * SVN MACROS
 *
 * $LastChangedDate: 2008-09-11 10:29:52 +0530 (Thu, 11 Sep 2008) $
 * $Revision: 694 $
 * $Author: sharmnid $
 *
 ************************************************************************/
/************************************************************************
 * FILE DESCR: Implementation of RunWordrec tool
 *
 * CONTENTS:
 *			main
 *			getIntValue
 *			getStringValue
 *			printUsage
 *			ValidateCommandLineArgs
 *
 * AUTHOR:     Thanigai Murugan K
 *
 * DATE:       August 30, 2005
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

#include "RunWordrec.h"
#include "LTKErrors.h"
#include "LTKWordRecoResult.h"
#include "LTKStringUtil.h"
#include "LTKVersionCompatibilityCheck.h"
#include "LTKException.h"
#include "LTKInc.h"
#include "LTKOSUtilFactory.h"
#include "LTKOSUtil.h"

int numChoices = 2;
char strTestLstFile[MAX_PATH] = "";
bool bComputePerformance = false;
char strProjectName[MAX_PROJECT_NAME] = "";
char strProfileName[MAX_PROFILE_NAME] = "";
char strLipiRootPath[MAX_PATH] = "";
char strOutputFileName[MAX_PATH] = "runwordrec.out";
bool bVersionRequest = false;

char **globalArg;
//char *globalArg[MAX_PATH];
int globalArgCount = 0;

int getAbsolutePath (string &pathName, const string lipiRootPath);

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: main
* DESCRIPTION	: Main function. Process the command line options and invoke the
*				  train/test methods after instantiating LipiEngine module.
* ARGUMENTS		: Command line arguments, refer to PrintUsage() function for syntax
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int main(int argc, char** argv)
//int main(int argc1, char** argv1)
{
	//char *envstring = NULL;

	//char *argv[] = {"runwordrec.exe", "-project", "boxfld_numerals", "-profile", "pointfloat_nn_boxfld", "-test", "c:\\Workspace\\lipitk_release_4.0.0\\projects\\boxfld_numerals\\data\\wordtestlist.txt"};
	
	//int argc = 7;
	char lipienginepath[MAX_PATH]="";
	int iErrorCode;
    LTKOSUtil* utilPtr = LTKOSUtilFactory::getInstance();
    void *functionHandle = NULL;
	
	globalArg = argv;
	//for( int i=0; i<argc; i++) 	globalArg[i] = _strdup(argv[i]);

	globalArgCount = argc;

	utilPtr->recordStartTime();
	
	// Assume the default log file, if user did not specify one...
	if(LTKSTRCMP(strLogFile, "") == 0)
	{
		strcpy(strLogFile, DEFAULT_LOG_FILE);
	}

	if(processCommandLineArgs() != 0)
	{
		printUsage();
		delete utilPtr;
		return -1;
	}

	if(bVersionRequest) /* Then display version and exit */
	{
		cout << "\n Version of runwordrec tool: " << CURRENT_VERSION << endl;
		delete utilPtr;
		return 0;
	}

	/* Get the LIPI_ROOT environment variable if the user has not provided in the command line */


	if(strlen(strLipiRootPath)==0)
	{
		char *tempStr=NULL;

		/* Get the LIPI_ROOT environment variable */
		tempStr=getenv(LIPIROOT_ENV_STRING);
		

		if(tempStr == NULL)
		{
			cout << "Error,LIPI_ROOT is neither provided in the command line nor set as an environment variable\n" << endl;
			delete utilPtr;
			return -1;
		}
		strcpy(strLipiRootPath,tempStr);
	}


	// Load the LipiEngine.DLL
	hLipiEngine = NULL;

    iErrorCode = utilPtr->loadSharedLib(strLipiRootPath, 
                                        LIPIENGINE_MODULE_STR, 
                                        &hLipiEngine);

	if(iErrorCode != SUCCESS)
	{
		cout << "Error loading LipiEngine module" << endl;
		delete utilPtr;
		return -1;
	}
	

	int iMajor_lipiEngine=0, iMinor_lipiEngine=0, iBugfix_lipiEngine=0;

    iErrorCode = utilPtr->getFunctionAddress(hLipiEngine, 
                                             "getToolkitVersion", 
                                             &functionHandle);

    if(iErrorCode != SUCCESS)
	{
		cout << "Error mapping the getToolkitVersion function" << endl;
		delete utilPtr;
		return -1;
	}

    LipiEngine_getCurrentVersion = (FN_PTR_GETCURRENTVERSION) functionHandle;

	LipiEngine_getCurrentVersion(&iMajor_lipiEngine, &iMinor_lipiEngine, &iBugfix_lipiEngine);

	// Version comparison START
	char toolkitVer[MAX_STRLEN];
	sprintf(toolkitVer, "%d.%d.%d",iMajor_lipiEngine,iMinor_lipiEngine,iBugfix_lipiEngine);

	LTKVersionCompatibilityCheck verTempObj;
	string supportedMinVersion(SUPPORTED_MIN_VERSION);
	string toolkitVersion(toolkitVer);

	bool compatibilityResults = verTempObj.isFirstVersionHigher(toolkitVersion, supportedMinVersion);

	if(compatibilityResults == false)
	{
		cout<< "\nIncompatible version of LipiEngine(ver: " << toolkitVersion << ") with runwordrec(ver: " << supportedMinVersion << ")" << endl;
		
		// Unload the DLL from memory
		utilPtr->unloadSharedLib(hLipiEngine);
		delete utilPtr;
		return FAILURE;
	}
	// Version comparison END

	// without reserving memory, it gives an error at the end...
	strLogFileName.reserve(MAX_PATH); 

	/* Get the function address of "createLTKLipiEngine" function from the DLL module */
    functionHandle = NULL;
    iErrorCode = utilPtr->getFunctionAddress(hLipiEngine, 
                                            "createLTKLipiEngine", 
                                            &functionHandle);

    if(iErrorCode != SUCCESS)
	{
		cout << "Error mapping the createLTKLipiEngine function" << endl;
		delete utilPtr;
		return -1;
	}

    createLTKLipiEngine = (FN_PTR_CREATELTKLIPIENGINE) functionHandle;

    functionHandle = NULL;
    
	// Create an instance of LipiEngine
	ptrObj = createLTKLipiEngine();

	// set the LIPI_ROOT path in Lipiengine module instance
	ptrObj->setLipiRootPath(strLipiRootPath);

	// set the Log File Path
	if (strlen(strLogFile) != 0 )
	{
		string tempString(strLogFile);
		ptrObj->setLipiLogFileName(tempString);
	}

	if(strlen(strLogLevel) != 0)
	{
		string tempStringLogLevel(strLogLevel);
		ptrObj->setLipiLogLevel(tempStringLogLevel);
	}


	// Initialize the LipiEngine 
	iErrorCode = ptrObj->initializeLipiEngine();
	if(iErrorCode != 0)
	{
		cout << "Error initializing lipiengine: " << getErrorMessage(iErrorCode) << endl;
		cout << "For more details, please see the log file" << endl;
		// Unload the DLL from memory
		utilPtr->unloadSharedLib(hLipiEngine);
		delete utilPtr;
		return -1;
	}

	string strProjName(strProjectName), strProfName(strProfileName);

	// Now create the word recognizer instance using the project/profile name strings
	LTKWordRecognizer *pReco;
	iErrorCode = ptrObj->createWordRecognizer(strProjName, strProfName, &pReco);

	if(iErrorCode != SUCCESS)
	{
		cout << "Error creating word recognizer: " << getErrorMessage(iErrorCode) << endl;
		cout << "For more details, please see the log file" << endl;

		// Unload the DLL from memory
		utilPtr->unloadSharedLib(hLipiEngine);
		delete utilPtr;
		return -1;
	}

	if(bComputePerformance)
    {
        utilPtr->recordStartTime();
    }
	
	iErrorCode = evaluateWordRecognizer(pReco, strTestLstFile);
	if(iErrorCode != SUCCESS)
	{

		cout << "Error during testing the word recognizer: " << getErrorMessage(iErrorCode) << endl;
		cout << "For more details, please see the log file" << endl;

		ptrObj->deleteWordRecognizer(pReco);

		// Unload the DLL from memory
		utilPtr->unloadSharedLib(hLipiEngine);
		delete utilPtr;
		return -1;
	}

	if(bComputePerformance)
	{
		utilPtr->recordEndTime();

        string timeTaken = "";
        utilPtr->diffTime(timeTaken);
        cout << "Time taken:" << timeTaken << endl;
	}

	// Delete the word recognizer which was created...
	ptrObj->deleteWordRecognizer(pReco);

	// Unload the DLL from memory
	utilPtr->unloadSharedLib(hLipiEngine);

    delete utilPtr;


	return 0;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: getIntValue
* DESCRIPTION	: To get the integer value which is passed as command line argument
*				  index specifies that the value to be fetched from the next command
*				  line argument position
* ARGUMENTS		: index - current index in the list of command line arguments
*				  iValue - Output value
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int getIntValue(int index, int* iValue)
{
	int iRes = 0;

	if(index > globalArgCount)
		return -1;

	if(index + 1 >= globalArgCount) // set to default...
		return -1;

	iRes = atoi(globalArg[index+1]);
	if(iRes <= 0)
		return -1;

	*iValue = iRes;
	return 0;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: getStringValue
* DESCRIPTION	: To get the string value which is passed as command line argument
*				  index specifies that the value to be fetched from the next command
*				  line argument position
* ARGUMENTS		: index - current index in the list of command line arguments
*				  strOption - Output value
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int getStringValue(int index, char* strOption)
{
	if(index > globalArgCount)
		return -1;

	if(index + 1 >= globalArgCount) // set to default...
		return -1;

	if(strlen(globalArg[index+1]) >= MAX_PATH)
		strncpy(strOption, globalArg[index], MAX_PATH);
	else
	{
		if(strlen(globalArg[index+1]) <= 1)
			return -1;

		// Checking the value 
		if(CheckForOption(globalArg[index+1]) != 0) 
			return -1;

		strcpy(strOption, globalArg[index+1]);
	}

	return 0;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: PrintUsage
* DESCRIPTION	: Prints the usage on an error
* ARGUMENTS		: None
* RETURNS		: void (none)
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
void printUsage()
{
	printf("\n\nUsage : runwordrec\n");
	printf("\nrunwordrec\n");
	printf("\n	-project <projectname>\n");
	printf("\n	-test <list filename>\n");
	printf("\n	[-profile <profilename>]\n");
	printf("\n	[-lipiroot <root name of the lipitk>]\n");
	printf("\n	[-loglevel <DEBUG|ERR|ALL|OFF|INFO>]\n");
	printf("\n	[-logfile <logfilename>]\n");
	printf("\n	[-output <output filename>]\n");
	printf("\n	[-numchoices <numchoices>]\n");
	printf("\n	[-perf]\n");
	printf("\n	[-ver]\n\n");
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: ValidateCommandLineArgs
* DESCRIPTION	: Validate the command line arguments. Report an error if value is
*				  missing for any command line option
* ARGUMENTS		: None
* RETURNS		: 0 on Success and -1 on Error
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int ValidateCommandLineArgs()
{
	if((LTKSTRCMP(strProjectName, "") == 0))
	{
		LOG(LTKLogger::LTK_LOGLEVEL_ERR)<< "Project Name not Specified ... " << endl;
		return -1; // No projectname specified
	}

	if(LTKSTRCMP(strTestLstFile, "") == 0)
	{
		LOG(LTKLogger::LTK_LOGLEVEL_ERR)<< "Option (train/test) not Specified ... " << endl;
		return -1; // No option specified -test
	}

	return 0;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: processCommandLineArgs
* DESCRIPTION	: Processes all the command line arguments. Report an error if value is
*				  missing for any command line option
* ARGUMENTS		: None
* RETURNS		: 0 on Success and -1 on Error
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int processCommandLineArgs()
{
	bool bProfileAssumed = true;

	// Minimum MIN_ARGS arguments must...
	if(globalArgCount >= MIN_ARGS)
	{
		if(LTKSTRCMP(globalArg[1], OPTION_VERSION) == 0)
		{
			bVersionRequest = true;
			return 0;
		}
	}

	for(int i = 1; i < globalArgCount; i++)
	{
		if((LTKSTRCMP(globalArg[i], OPTION_HELP1) == 0) || (LTKSTRCMP(globalArg[i], OPTION_HELP2) == 0))
		{
			return -1;
		}

		if(LTKSTRCMP(globalArg[i], OPTION_VERSION) == 0)
		{
			bVersionRequest = true;
			return 0;
		}

		if(LTKSTRCMP(globalArg[i], OPTION_NUMCHOICES) == 0)
		{
			if(getIntValue(i, &numChoices) != 0)
			{
				printf("\nInvalid or no value specified for -numchoices option.\n");
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_TEST) == 0)
		{
			char tempVal[MAX_PATH], tempMode[124];

			strcpy(tempMode, globalArg[i]);

			// Now get the list file name
			if(getStringValue(i, tempVal) != 0)
			{
				printf("\nMissing list file name for %s option.\n", globalArg[i]);
				return -1;
			}
			i++;

			if(LTKSTRCMP(tempMode, OPTION_TEST)==0)
			{
				strcpy(strTestLstFile, tempVal);
			}
			continue;
		}

		if(LTKSTRCMP(globalArg[i], OPTION_PERF) == 0)
		{
			if(globalArgCount <= MIN_ARGS)
			{		
				printf("\n Option %s can only be combined with other options.\n", globalArg[i]);
				return -1;
			}
			else { bComputePerformance = true; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_PROJECT) == 0)
		{
			if(getStringValue(i, strProjectName) != 0)
			{
				printf("\nInvalid or no value specified for -project option.\n");
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_PROFILE) == 0)
		{
			if(getStringValue(i, strProfileName) != 0)
			{
				printf("\nInvalid or no value specified for -profile option.\n");
				return -1;
			}
			else { i++; bProfileAssumed = false; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_OUTPUT) == 0)
		{
			if(getStringValue(i, strOutputFileName) != 0)
			{
				printf("\nInvalid or no value specified for -output option.\n");
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_LOGFILE) == 0)
		{
			if(getStringValue(i, strLogFile) != 0)
			{
				printf("\nInvalid or no value specified for -logfile option.\n");
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], LIPI_ROOT) == 0)
		{
			if(getStringValue(i, strLipiRootPath) != 0)
			{
				printf("\nInvalid or no value specified for -lipiroot option.\n");
				return -1;
			}
			else { i++;  continue;}
		}
		
		if(LTKSTRCMP(globalArg[i], OPTION_LOGLEVEL) == 0)
		{
			if(getStringValue(i, strLogLevel) != 0)
			{
				printf("\nInvalid or no value specified for -loglevel option.\n");
				return -1;
			}
			else 
			{
				i++; 
				continue;
			}
		}

		printf("\nInvalid or Unknown option.\n");
		return -1;
	}

	if(LTKSTRCMP(strProjectName, "") == 0)
	{
		//printf("\nNo project name specified using -project.\n");
		return -1;
	}

	if(ValidateCommandLineArgs() != 0)
		return -1;
	
	if(LTKSTRCMP(strProfileName, "") == 0)
		strcpy(strProfileName, DEFAULT_PROFILE);

	if(bProfileAssumed)
		printf("\n Profile not specified, assuming %s profile.\n", DEFAULT_PROFILE);

	return 0;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: evaluateWordRecognizer
* DESCRIPTION	: Load the model data and call recognize function and display the
*				  results. 
* ARGUMENTS		: pReco - handle to LTKWordRecognizer object
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int evaluateWordRecognizer(LTKWordRecognizer *pReco, const string& infilelist)
{
	int iErrorCode = 0;
	string tempStr(REC_UNIT_INFO), tempStr1(REC_MODE);
	string path;
	string strShapeId;

	vector<LTKTraceGroup> fieldInk;
	int charIndex;
	wstring eolstr(L"\r\n");
	int i;
    string strWordId;

	LTKRecognitionContext *recoContext = new LTKRecognitionContext();

	LTKCaptureDevice deviceContext;
	LTKScreenContext screenContext;

	recoContext->setWordRecoEngine(pReco);
	recoContext->setFlag(tempStr,REC_UNIT_CHAR);
	recoContext->setFlag(tempStr1,REC_MODE_STREAMING);

	recoContext->setNumResults(numChoices);

	ifstream in(infilelist.c_str());
	if(in == NULL)
	{
		LOG(LTKLogger::LTK_LOGLEVEL_ERR)<< "Test list file open error " << infilelist <<endl;
		cout << "Test list file open error : " << infilelist.c_str() << endl;

		//delete recognition context object
		if(recoContext)
		{
			//ptrObj->deleteRecognitionContext(recoContext);

			delete recoContext;
			recoContext = NULL;
		}
		return FAILURE;
	}

	ofstream resultfile(strOutputFileName,ios::out|ios::binary);

	//a Header of 0xFEFF is required to identify this is a
	//16 bit unicode file
	const unsigned short fHeader = 0xfeff;
	resultfile.write((char*)&fHeader,sizeof(unsigned short));

	while(in)
	{
		//Get the file name
		if(!getline(in,path,' ')) 
		{
			break;
		}
		
		//Get the word ID
		getline(in,strWordId);
		//iWordID = atoi(strShapeId.c_str());

		if(path.length() > 0 && path[0] == COMMENTCHAR )
		{
			continue;
		}

		if(path.length() == 0)
		{
			LOG(LTKLogger::LTK_LOGLEVEL_ERR)<< "Empty File name" <<endl;
			continue;
		}

		try
		{
			fieldInk.clear();

			getAbsolutePath(path,strLipiRootPath);

			cout << path << endl;

			//read the word file
			if(readWordFile(path, fieldInk, deviceContext, screenContext) != SUCCESS)
			{
				LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error reading ink file:" << path << endl;
				cout<<"Error reading ink file:" << path << endl;
				cout<<"Aborted"<<endl;

				//delete recognition context object
				if(recoContext)
				{
					//ptrObj->deleteRecognitionContext(recoContext);

					delete recoContext;
					recoContext = NULL;
				}
				return FAILURE;

			}
			recoContext->setDeviceContext(deviceContext);
			recoContext->setScreenContext(screenContext);

			if(fieldInk.size()==0)
			{

				LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Empty trace group read from:" << path << endl;
				continue;
			}
			
			for(charIndex = 0; charIndex < fieldInk.size(); ++charIndex)
			{
				recoContext->beginRecoUnit();
				recoContext->addTraceGroups(LTKTraceGroupVector(1,fieldInk.at(charIndex)));
				recoContext->endRecoUnit();
				recoContext->beginRecoUnit();
				recoContext->addTraceGroups(LTKTraceGroupVector(1,LTKTraceGroup()));
				recoContext->endRecoUnit();

			}
		}
		catch(LTKException e)
		{
			LOG(LTKLogger::LTK_LOGLEVEL_ERR) << e.getExceptionMessage() <<endl;

			//delete recognition context object
			if(recoContext)
			{
				//ptrObj->deleteRecognitionContext(recoContext);

				delete recoContext;
				recoContext = NULL;
			}

			return FAILURE;
		}

		//Calling recognize and retrieving the top result
		{
			LTKWordRecoResult result;
			vector<LTKWordRecoResult> r2;
			recoContext->recognize();
			recoContext->getTopResult(result);
			recoContext->getNextBestResults(numChoices-1, r2);

			vector<unsigned short> resultVec = result.getResultWord();
			if(!resultVec.empty())
			{
				resultfile.write((char *)&(resultVec.at(0)), resultVec.size()*sizeof(unsigned short));
				resultfile.write((char*)eolstr.c_str(),eolstr.length()*sizeof(unsigned short));

				for(i =0; i<r2.size(); ++i)
				{
					resultVec = r2.at(i).getResultWord();
					resultfile.write((char *)&(resultVec.at(0)), resultVec.size()*sizeof(unsigned short));
					resultfile.write((char*)eolstr.c_str(),eolstr.length()*sizeof(unsigned short));
				}
			}

			recoContext->clearRecognitionResult();
		}

		resultfile.write((char*)eolstr.c_str(),eolstr.length()*sizeof(unsigned short));
	}

	resultfile.close();

	//delete recognition context object
	if(recoContext)
	{
		//ptrObj->deleteRecognitionContext(recoContext);

		delete recoContext;
		recoContext = NULL;
	}

	return SUCCESS;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: CheckForOption
* DESCRIPTION	: Check if the argument is matching with any of the option strings
* ARGUMENTS		: strVal - Value to match
* RETURNS		: 1 if matching and 0 if not matching
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int CheckForOption(char* strVal)
{
	if( (LTKSTRCMP(strVal, OPTION_PERF) == 0) ||
		(LTKSTRCMP(strVal, OPTION_TEST) == 0) ||
		(LTKSTRCMP(strVal, OPTION_PROJECT) == 0) ||
		(LTKSTRCMP(strVal, OPTION_PROFILE) == 0) ||
		(LTKSTRCMP(strVal, OPTION_OUTPUT) == 0) ||
		(LTKSTRCMP(strVal, OPTION_LOGFILE) == 0) ||
		(LTKSTRCMP(strVal, OPTION_LOGLEVEL) == 0) ||
		(LTKSTRCMP(strVal, OPTION_NUMCHOICES) == 0))
		return 1;

	return 0;
}

//This currently assume the anotation
//is sequential
int readWordFile(string fileName, vector<LTKTraceGroup>& traceGroupVec, LTKCaptureDevice& dc, LTKScreenContext& sc)
{

	//anotation Info
	map<string, string>anotInfo;   //annotation info
	LTKTraceGroup traceGroup;      //trace group
	vector<string> strIdscs;       //comma separated strings
	vector<string> strIdshs;       //hyphen separated strings
	vector<int> intIds;            //intIds
	int firstId, secondId;         //first and last IDs when there are hyphenated IDs
	int loopIndex;                 //index of Ids
	
	vector<string>::iterator strIter;

	//Reading the anotation file
	if(LTKInkFileReader::readUnipenInkFileWithAnnotation(fileName,"CHARACTER","ALL",traceGroup,anotInfo, dc, sc) != SUCCESS)
	{
		cout<<"Error reading ink file:"<<fileName<<endl;
		LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error reading ink file:"<<fileName<<endl;
		return FAILURE;
	}

	for(map<string,string>::iterator annotationIter=anotInfo.begin();annotationIter!=anotInfo.end();++annotationIter)
	{
		LTKTraceGroup charTraceGroup;  //TraceGroup corresponding to this char

		string strokeIndices=annotationIter->first;
		string comment=annotationIter->second;

		//Finding out the stroke IDs
		LTKStringUtil::tokenizeString(strokeIndices,  " ,\t",  strIdscs);
		for(strIter = strIdscs.begin(); strIter != strIdscs.end(); ++strIter)
		{
			if((*strIter).find('-')!=-1)
			{
				//tokenize string again if there are hyphens
				LTKStringUtil::tokenizeString(*strIter, "-", strIdshs);
				firstId  = atoi((strIdshs.front()).c_str()); //first ID
				secondId = atoi((strIdshs.back()).c_str());  //second ID

				for(loopIndex = firstId; loopIndex <=secondId; ++loopIndex )
				{
					intIds.push_back(loopIndex);
				}
			}
			else
			{
				intIds.push_back(atoi((*strIter).c_str() ));
			}
			strIdshs.clear();
		}

		strIdscs.clear();

		//sort the IDs in the sequence
		sort(intIds.begin(), intIds.end(),less<int>());

		//constructing a temporary trace group
		for(loopIndex = 0; loopIndex < intIds.size(); ++loopIndex)
		{

			if(intIds.at(loopIndex) < 0 || intIds.at(loopIndex) >= traceGroup.getNumTraces())
			{
				cout<<"Annotation and trace group read from ink file:"<<fileName<<" do not match"<<endl;
				LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Annotation and trace group read from ink file:"<<fileName<<" do not match"<<endl;
				return FAILURE;

			}

			LTKTrace tempTrace;
			traceGroup.getTraceAt(intIds.at(loopIndex), tempTrace);
			
			charTraceGroup.addTrace(tempTrace);
		}

		
		//push back this trace vector.
		traceGroupVec.push_back(charTraceGroup);


		intIds.clear();
	}

	return SUCCESS;
}


int getAbsolutePath (string &pathName, const string lipiRootPath)
{

	vector<string> tokens;

	//Split the path name into number of tokens based on the delimter
	LTKStringUtil::tokenizeString(pathName,  "\\/",  tokens);

	//The first token must be the $LIPI_ROOT. Otherwise return from the function
	if (tokens[0] != LIPIROOT)
	{
		return SUCCESS;
	}
	
	//Store the Environment variable into the tokens
	tokens[0] = lipiRootPath;

	//Reinitialize the pathName
	pathName = "";
	for(int i=0 ; i < tokens.size() ; i++)
	{
		pathName += tokens[i] + SEPARATOR; 
	}

	// Erase the last character '\'
	pathName.erase(pathName.size()-1,1);

	return SUCCESS;
}
