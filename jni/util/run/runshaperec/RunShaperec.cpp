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
 * $LastChangedDate: 2011-02-08 16:57:52 +0530 (Tue, 08 Feb 2011) $
 * $Revision: 834 $
 * $Author: mnab $
 *
 ************************************************************************/
/************************************************************************
 * FILE DESCR: Implementation of RunShaperec tool
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
#include "RunShaperec.h"
#include "LTKTrace.h"
#include "LTKLipiEngineInterface.h"
#include "LTKErrors.h"
#include "LTKStringUtil.h"
#include "LTKVersionCompatibilityCheck.h"
#include "LTKException.h"
#include "LTKErrorsList.h"
#include "LTKOSUtilFactory.h"
#include "LTKOSUtil.h"
#include "LTKConfigFileReader.h"
#include "LTKClassifierDefaults.h"
#include "LTKShapeRecoUtil.h"
#include "LTKShapeFeature.h"
#include "LTKShapeFeatureExtractorFactory.h"
#include "LTKShapeFeatureExtractor.h"
#include "LTKInkFileReader.h"

#define ADAPTCHAR "#"
#define NEW_SHAPEID -2

enum TRAIN_TEST_ADAPT {NONE, TEST, TRAIN, ADAPT};
TRAIN_TEST_ADAPT  trainTestAdapt=NONE;

/* function pointer declaration to get the function address of "createLTKLipiEngine" */
typedef LTKLipiEngineInterface* (*FN_PTR_CREATELTKLIPIENGINE) (void);
FN_PTR_CREATELTKLIPIENGINE createLTKLipiEngine;

typedef void (*FN_PTR_DELETELTKLIPIENGINE) (void);
FN_PTR_DELETELTKLIPIENGINE deleteLTKLipiEngine;

typedef int (*FN_PTR_GETCURRENTVERSION) (int *iMajor, int *iMinor, int *iBugfix);
FN_PTR_GETCURRENTVERSION LipiEngine_getCurrentVersion;

typedef int (*FN_PTR_CREATELTKLIPIPREPROCESSOR)(const LTKControlInfo& , LTKPreprocessorInterface** );
FN_PTR_CREATELTKLIPIPREPROCESSOR createLTKLipiPreProcessor = NULL;

typedef int (*FN_PTR_DELETELTKLIPIPREPROCESSOR)(LTKPreprocessorInterface* );
FN_PTR_DELETELTKLIPIPREPROCESSOR deleteLTKLipiPreProcessor =  NULL;

typedef int (*FN_PTR_SETLTKLOGFILENAME)(const string& );
FN_PTR_SETLTKLOGFILENAME setLTKLogFileName =  NULL;


char strTrainLstFile[MAX_PATH] = "";
char strTestLstFile[MAX_PATH] = "";
char strProjectName[MAX_PROJECT_NAME] = "";
char strProfileName[MAX_PROFILE_NAME] = "";
char strHeaderInfoFileName[MAX_PATH] = "";
char strComment[MAX_STRLEN] = "";
char strDataset[MAX_STRLEN] = "";
char strLogFile[MAX_PATH] = "";
char strLogLevel[MAX_PATH] = "";
string strLogFileName;
char strSampleSet[MAX_PATH] = "";
vector<int> shapeSubSet; //subset of classes to be compared for recognition. If empty, all classes are compared

char strAdaptListFile[MAX_PATH];
char strLipiRootPath[MAX_PATH];
char strOutputFileName[MAX_PATH];
char strInFileType[MAX_PATH]={0};
float confThreshold;
int numChoices;

bool bVersionRequest = false;
bool bComputePerformance = false;

/* Pointer to the LipiEngine interface */
LTKLipiEngineInterface *ptrObj = NULL;
void *hLipiEngine;
char **globalArg;
int globalArgCount = 0;

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

//int main(int argc1, char* argv1[])
int main(int argc, char** argv)
{
	//char *argv[] = {"runshaperec.exe", "-project", "alphanumeric", "-profile", "pointfloat_activedtw", "-test", "c:\\Workspace\\lipitk_release_4.0.0\\projects\\alphanumeric\\data\\testPunctuation.txt", "-sampleset", "62-71"};
	//char *argv[] = {"runshaperec.exe", "-project", "alphanumeric", "-train", "c:\\Workspace\\lipitk_release_4.0.0\\projects\\alphanumeric\\data\\trainfile_00.txt"};
	//int argc = 9;
	char *envstring = NULL;
	int iErrorCode;
    void * functionHandle = NULL;
    LTKOSUtil* utilPtr = LTKOSUtilFactory::getInstance();
	LTKShapeRecognizer *pReco = NULL;

	numChoices = NUM_CHOICES_FILTER_OFF;
	confThreshold = CONF_THRESHOLD_FILTER_OFF;
	strcpy(strLipiRootPath,"");
	strcpy(strOutputFileName,"runshaperec.out");
	strcpy (strAdaptListFile,"");
	strcpy(strInFileType,INK_FILE);

	globalArg = argv;

	// for( int i=0; i<argc; i++) 	globalArg[i] = _strdup(argv[i]);

	globalArgCount = argc;

    utilPtr->recordStartTime();

	//freopen ("runshapeCOut.txt","w",stdout);

	if(processCommandLineArgs() != 0)
	{
		printUsage();
        delete utilPtr;
		return -1;
	}

	if(bVersionRequest) /* Then display version and exit */
	{
		cout << "\n Version of runshaperec Tool: " << CURRENT_VERSION << endl;
        delete utilPtr;
		return 0;
	}

	/* Get the LIPI_ROOT environment variable */
	envstring = strLipiRootPath;

	if(strlen(envstring) == 0)
	{
		envstring = getenv(LIPIROOT_ENV_STRING);

		if(envstring == NULL )
		{
			cout << "Error, Environment variable is not set LIPI_ROOT" << endl;
            delete utilPtr;
			return -1;
		}
		strcpy(strLipiRootPath, envstring );

	}

	// Load the LipiEngine.DLL
	hLipiEngine = NULL;
	
    iErrorCode = utilPtr->loadSharedLib(envstring, LIPIENGINE_MODULE_STR, &hLipiEngine);

	if(iErrorCode != SUCCESS)
	{
		cout << "Error loading LipiEngine module" << endl;
        delete utilPtr;
		return -1;
	}

	//Set logfile to lipiengine
	if (strlen(strLogFile) != 0 )
	{
		iErrorCode = utilPtr->getFunctionAddress(hLipiEngine,
												 "setLTKLogFileName",
												 &functionHandle);

		if(iErrorCode != SUCCESS)
		{
			cout << "Error mapping the setLTKLogFileName function" << endl;
			delete utilPtr;
			return -1;
		}

		setLTKLogFileName = (FN_PTR_SETLTKLOGFILENAME) functionHandle;

		functionHandle = NULL;

		string tempString(strLogFile);
		setLTKLogFileName(tempString);
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

    functionHandle = NULL;

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
		cout << "\nIncompatible version of LipiEngine(ver: " <<
                toolkitVersion << ") with runshaperec(ver: " <<
                supportedMinVersion << ")" << endl;

        cleanUp(pReco, utilPtr);

		return FAILURE;
	}
	// Version comparison END


	/* Get the function address of "createLTKLipiEngine" function from the DLL module */
    functionHandle = NULL;
    utilPtr->getFunctionAddress(hLipiEngine, "createLTKLipiEngine", &functionHandle);

    createLTKLipiEngine = (FN_PTR_CREATELTKLIPIENGINE) functionHandle;

    functionHandle = NULL;

    /* Get the function address of "deleteLTKLipiEngine" function from the DLL module */
    functionHandle = NULL;
    utilPtr->getFunctionAddress(hLipiEngine, "deleteLTKLipiEngine", &functionHandle);

    deleteLTKLipiEngine = (FN_PTR_DELETELTKLIPIENGINE) functionHandle;

    functionHandle = NULL;

	// Create an instance of LipiEngine


	ptrObj = createLTKLipiEngine();

	// set the LIPI_ROOT path in Lipiengine module instance
	ptrObj->setLipiRootPath(envstring);

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
		cout << " Error : " << getErrorMessage(iErrorCode) << endl;

		cleanUp(pReco, utilPtr);

		return FAILURE;
	}

	cout << " lipiengine initialized " << endl;

	string strProjName(strProjectName), strProfName(strProfileName);



	iErrorCode = ptrObj->createShapeRecognizer(strProjName, strProfName, &pReco);

	if(iErrorCode != SUCCESS)
	{
		cout << "Error : "
			 << getErrorMessage(iErrorCode) << endl;

		cleanUp(pReco,utilPtr);

		return FAILURE;
	}

	// Train or Test option?
	if(trainTestAdapt == TRAIN)
	{
		if(bComputePerformance)
		{
			utilPtr->recordStartTime();
		}

		string strHeader = strHeaderInfoFileName;
		string comment = strComment;
		string dataset = strDataset;


		if(strcmp(FEATURE_FILE,strInFileType) == 0 )
		{
			string featureExtractorName;
			if(validateFeatureFile(strTrainLstFile,featureExtractorName) == SUCCESS)
			{
				/*if(featureExtractorName.empty() == true)
				{
					cout<<"Feature extractor name in config file and feature file do not match."<<endl;
					return FAILURE;
				}*/

				iErrorCode = pReco->train(strTrainLstFile, strHeader, comment, dataset,strInFileType);
			}
		}
		else

			iErrorCode = pReco->train(strTrainLstFile, strHeader, comment, dataset);

		if(iErrorCode != 0)
		{
			cout << "Error : " <<  getErrorMessage(iErrorCode) << endl;

			cleanUp(pReco,utilPtr);

			return FAILURE;
		}

		if(bComputePerformance)
		{
		    //Time at the end of Train Clustering
            utilPtr->recordEndTime();

            string timeTaken = "";
            utilPtr->diffTime(timeTaken);

			cout << "Time taken (in seconds) for train function: "
				 << timeTaken << endl;
		}
	}
	else if(trainTestAdapt == TEST )
	{
		if(bComputePerformance)
        {
            utilPtr->recordStartTime();
        }


		if(LTKSTRCMP(strInFileType, INK_FILE) == 0)
		{
			iErrorCode = evaluateShapeRecognizer(&pReco);

		}
		else if(LTKSTRCMP(strInFileType, FEATURE_FILE) == 0)
		{
			iErrorCode = ShapeRecognizerFromFeatureFile(&pReco);
		}


		if(iErrorCode != SUCCESS)
		{

            cleanUp(pReco,utilPtr);
			return FAILURE;
		}

		if(bComputePerformance)
		{
			utilPtr->recordEndTime();

            string timeTaken = "";
            utilPtr->diffTime(timeTaken);

			cout << endl;
			cout << "Time Taken  = " << timeTaken << endl;
		}
	}
	else if(trainTestAdapt == ADAPT )
	{
		if(bComputePerformance)
        {
            utilPtr->recordStartTime();
        }

		iErrorCode = evaluateAdapt(&pReco, strProjName);
		if(iErrorCode !=SUCCESS)
		{
			cout << "Error loading Model data." << endl;
			LOG( LTKLogger::LTK_LOGLEVEL_ERR) << "Error : " <<
				iErrorCode << getErrorMessage(iErrorCode) << "Error in evaluateAdapt()"
				<<endl;

			cleanUp(pReco, utilPtr);
			LTKReturnError(iErrorCode);
		}

		if(bComputePerformance)
		{
			utilPtr->recordEndTime();

            string timeTaken = "";
            utilPtr->diffTime(timeTaken);

			cout << endl;
			cout << "Time Taken  = " << timeTaken << endl;
		}
	}
    else
    {
        cout << "Invalid option, Exiting " << endl;
    }


	cleanUp(pReco, utilPtr);

	cout << "Exiting runShapeRec\n";

	//fclose(stdout);
	return SUCCESS;
}

/***********************************************************************************
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

    string tempString = globalArg[index+1];

    if (LTKStringUtil::isInteger(tempString) != true )
    {
        return -1;
    }

	iRes = atoi(globalArg[index+1]);

	if(iRes < 0)
		return -1;

	*iValue = iRes;
	return 0;
}
/***********************************************************************************
* AUTHOR		: Vandana Roy
* DATE			: 13-AUG-2007
* NAME			: getFloatValue
* DESCRIPTION	: To get the float value which is passed as command line argument
*				  index specifies that the value to be fetched from the next command
*				  line argument position
* ARGUMENTS		: index - current index in the list of command line arguments
*				  iValue - Output value
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/

int getFloatValue(int index, float* fValue)
{
//	float fRes = 0.0f;
	float fRes = -1.0f;

	if(index > globalArgCount)
		return -1;

	if(index + 1 >= globalArgCount) // set to default...
		return -1;

    string tempString = globalArg[index+1];

    if (LTKStringUtil::isFloat(tempString) != true )
    {
        return -1;
    }

	fRes = atof(globalArg[index+1]);

	if(fRes < 0)
		return -1;

	*fValue = fRes;
	return 0;
}

/***********************************************************************************
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
	printf("\n\nUsage : runshaperec\n");
	printf("\nrunshaperec\n");
	printf("\n	-project <projectname>\n");
	printf("\n	-train <filename> | -test <list filename> | -adapt <filename>\n");
	printf("\n	[-h <header info filename>]\n");
	printf("\n	[-lipiroot <root name of the lipitk>]\n");
	printf("\n	[-profile <profilename>]\n");
	printf("\n	[-loglevel <DEBUG|ERR|ALL|OFF|INFO>]\n");
	printf("\n	[-logfile <logfilename>]\n");
	printf("\n	[-output <output filename>]\n");
	printf("\n	[-numchoices <numchoices>]\n");
	printf("\n	[-confthreshold <confthreshold>]\n");
	printf("\n	[-infiletype <feature|default ink>]\n");
	printf("\n	[-sampleset <range of classes to train or test from like 0-25 >]\n");
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
		cout << "Project name is not specified." << endl;
		return -1; // No projectname specified
	}

	if((LTKSTRCMP(strTrainLstFile, "") == 0) &&
       (LTKSTRCMP(strTestLstFile, "") == 0) &&
	   (LTKSTRCMP(strAdaptListFile, "") == 0)
	){
		cout << "Either -train/-test/-adapt or -writefeatureFile option is required to run this." << endl;
		return -1; // No option specified -train or -test
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
		if((LTKSTRCMP(globalArg[i], OPTION_HELP1) == 0) ||
            (LTKSTRCMP(globalArg[i], OPTION_HELP2) == 0))
		{
			return -1;
		}

		if(LTKSTRCMP(globalArg[i], OPTION_VERSION) == 0)
		{
			bVersionRequest = true;
			return 0;
		}

		if(LTKSTRCMP(globalArg[i], SAMPLE_SET) == 0)
		{
			char szTemp[MAX_PATH];

			if( 0 != getStringValue(i,strSampleSet))
			{
				cout << "Invalid or no value specified for -sampleset option." << endl;
				return -1;
			}
			else
			{
				fillSampleSetVec(strSampleSet);
				i++;
				continue;
			}			
		}

		if(LTKSTRCMP(globalArg[i], OPTION_NUMCHOICES) == 0)
		{
			if ( i+1 >= globalArgCount )
			{
				cout << "Invalid or no value specified for -numchoices option." << endl;
				return -1;
			}

			if ( LTKSTRCMP(globalArg[i+1], "all" ) == 0 )
			{
				numChoices = NUM_CHOICES_FILTER_OFF;
                i++ ;
                continue;
			}
			else if(getIntValue(i, &numChoices) != 0)
			{
				cout << "Invalid or no value specified for -numchoices option." << endl;
				return -1;
			}
			else
            {
                i++;
                continue;
            }
		}
		if(LTKSTRCMP(globalArg[i], OPTION_CONFTHRESHOLD) == 0)
		{
			if(getFloatValue(i, &confThreshold) != 0)
			{
				cout << "Invalid or no value specified for -confthreshold option." << endl;
				return -1;
			}
			else { i++; continue;}
		}


		if((LTKSTRCMP(globalArg[i], OPTION_TRAIN) == 0) ||
			(LTKSTRCMP(globalArg[i], OPTION_TEST) == 0) ||
			(LTKSTRCMP(globalArg[i], OPTION_ADAPT) == 0))
		{
			char tempVal[MAX_PATH], tempMode[124];

			strcpy(tempMode, globalArg[i]);

			// Now get the list file name
			if(getStringValue(i, tempVal) != 0)
			{
				cout << "Missing list file name for" << globalArg[i] << " option."  << endl;
				return -1;
			}
			i++;

			if(LTKSTRCMP(tempMode, OPTION_TRAIN)==0)
			{
				trainTestAdapt = TRAIN;
				strcpy(strTrainLstFile, tempVal);
			}
			if(LTKSTRCMP(tempMode, OPTION_TEST)==0)
			{
				trainTestAdapt = TEST;
				strcpy(strTestLstFile, tempVal);
			}
			if(LTKSTRCMP(tempMode, OPTION_ADAPT)==0)
			{
				trainTestAdapt = ADAPT;
				strcpy(strAdaptListFile, tempVal);
			}
			continue;
		}

		if(LTKSTRCMP(globalArg[i], OPTION_PERF) == 0)
		{
			if(globalArgCount <= MIN_ARGS)
			{
				cout << "Option " << globalArg[i] <<
					    " can only be combined with other options." << endl;
				return -1;
			}
			else { bComputePerformance = true; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_PROJECT) == 0)
		{
			if(getStringValue(i, strProjectName) != 0)
			{
				cout << "Invalid or no value specified for -project option." << endl;
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_COMMENT) == 0)
		{
			if(getStringValue(i, strComment) != 0)
			{
				cout << "Invalid or no value specified for -comment option." << endl;
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_DATASET) == 0)
		{
			if(getStringValue(i, strDataset) != 0)
			{
				cout << "Invalid or no value specified for -dataset option." << endl;
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_PROFILE) == 0)
		{
			if(getStringValue(i, strProfileName) != 0)
			{
				cout << "Invalid or no value specified for -profile option." << endl;
				return -1;
			}
			else { i++; bProfileAssumed = false; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_OUTPUT) == 0)
		{
			if(getStringValue(i, strOutputFileName) != 0)
			{
				cout << "Invalid or no value specified for -output option." << endl;
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_HEADERINFO) == 0)
		{
			if(getStringValue(i, strHeaderInfoFileName) != 0)
			{
				cout << "Invalid or no value specified for -h option." << endl;
				return -1;
			}
			else { i++; continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_LOGFILE) == 0)
		{
			if(getStringValue(i, strLogFile) != 0)
			{
				cout << "Invalid or no value specified for -logfile option." << endl;
				return -1;
			}
			else { i++; continue;}
		}
		if(LTKSTRCMP(globalArg[i], OPTION_LOGLEVEL) == 0)
		{
			if(getStringValue(i, strLogLevel) != 0)
			{
				cout << "Invalid or no value specified for -loglevel option." << endl;
				return -1;
			}
			else
			{
				i++;
				continue;
			}
		}

		if(LTKSTRCMP(globalArg[i], LIPI_ROOT) == 0)
		{
			if(getStringValue(i, strLipiRootPath) != 0)
			{
				cout << "Invalid or no value specified for -lipiroot option." << endl;
				return -1;
			}
			else { i++;  continue;}
		}

		if(LTKSTRCMP(globalArg[i], OPTION_INFILETYPE) == 0)
		{
			if(getStringValue(i, strInFileType) != 0)
			{
				cout << "Invalid or no value specified for -infiletype option." << endl;
				return -1;
			}
			else { 
				if(LTKSTRCMP(strInFileType, INK_FILE) != 0 && LTKSTRCMP(strInFileType, FEATURE_FILE) != 0)
				{
					cout << "File Type was incorrect (Either feature or ink to be specified)." << endl;
					return -1;
				}
				
				i++; continue;
			}
		}

		cout << globalArg[i] << " : Invalid or Unknown option." << endl;
		return -1;
	}

	if(trainTestAdapt == NONE)
		return -1; // specify either training or testing

	if(LTKSTRCMP(strProjectName, "") == 0)
	{
		cout << "No project name specified using -project." << endl;
		return -1;
	}

	if(ValidateCommandLineArgs() != 0)
		return -1;

	if(LTKSTRCMP(strProfileName, "") == 0)
		strcpy(strProfileName, DEFAULT_PROFILE);

	if(trainTestAdapt== TEST && (LTKSTRCMP(strHeaderInfoFileName, "") != 0))
	{
		cout << "Ignoring -h for option -test." << endl;
	}

	if(bProfileAssumed)
		cout << "Profile not specified, assuming "
		     <<  DEFAULT_PROFILE << " profile." << endl;

	return 0;
}

/**********************************************************************************
* AUTHOR		: Thanigai Murugan K
* DATE			: 30-AUG-2005
* NAME			: evaluateShapeRecognizer
* DESCRIPTION	: Load the model data and call recognize function and display the
*				  results.
* ARGUMENTS		: pReco - handle to LTKShapeRecognizer object
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
* Saravanan .R		17-01-07			Call a getAbsolutePath() function to convert
*										the Relative path to absolute path for testing
*************************************************************************************/
int evaluateShapeRecognizer(LTKShapeRecognizer **pReco)
{


	//LTKCaptureDevice captureDevice(0, true, 0, 0, 0);
    LTKCaptureDevice captureDevice;
	LTKScreenContext screenContext;
	int iErrorCode;

	//vector<int> shapeSubSet; //subset of classes to be compared for recognition. If empty, all classes are compared



//	float confThreshold = 0.0f;

	vector<LTKShapeRecoResult> results;

	string path;

	string numSamples;

	string strShapeId;

	int intShapeId;

	int errorCount = 0;

	int total = 0;

	results.reserve(numChoices+1);

	try
	{
		int iResult = (*pReco)->loadModelData();
		if(iResult != 0)
		{
			cout << "Error loading Model data." << endl;
			LOG( LTKLogger::LTK_LOGLEVEL_ERR) << "Error loading Model data." << endl;

			cout << "Error : " <<  getErrorMessage(iResult) << endl;
			return -1;
		}
	}

	catch(LTKException e)
	{
		LOG( LTKLogger::LTK_LOGLEVEL_ERR) << e.getExceptionMessage() << endl;

		return FAILURE;
	}

	ifstream in(strTestLstFile);
	if(in == NULL)
	{
		LOG( LTKLogger::LTK_LOGLEVEL_ERR) << "Test list file open error" + string(strTestLstFile) << endl;
		cout << "Test list file open error : " << strTestLstFile << endl;
		return -1;
	}

	ofstream resultfile(strOutputFileName);

	while(in)
	{
		if(!getline(in,path,' '))
		{
			break;
		}

		getline(in,strShapeId);

		++total;

		intShapeId = atoi(strShapeId.c_str());

		LTKTraceGroup traceGroup;


		if(path.length() > 0 && path[0] == COMMENTCHAR )
		{
			continue;
		}

		if(path.length() == 0)
		{
			LOG( LTKLogger::LTK_LOGLEVEL_DEBUG) << "Empty File name"  << endl;
			continue;
		}

		/* To get the Absolute Path from the ink file	*/

		int error = getAbsolutePath(path, strLipiRootPath);

		/* Print the path name	*/

		cout << path << endl;

		try
		{
			iErrorCode = LTKInkFileReader::readUnipenInkFile(path, traceGroup, captureDevice, screenContext);
		}
		catch(LTKException e)
		{
			cout << "Error while reading the input file" << endl;
			cout << getErrorMessage(iErrorCode) << endl;
			LOG( LTKLogger::LTK_LOGLEVEL_ERR) << getErrorMessage(iErrorCode) << endl;
			return FAILURE;
		}

		(*pReco)->setDeviceContext(captureDevice);

		iErrorCode = (*pReco)->recognize(traceGroup, screenContext, shapeSubSet, confThreshold, numChoices, results);

		if(iErrorCode != 0)
		{
			cout << "Error : " <<  getErrorMessage(iErrorCode) << endl;
			return -1;
		}

	//	pReco->deleteClass(0);
		resultfile << path << '\t' << intShapeId << ' ';

        if (results.size() == 0)
        {
            resultfile << -1 << ' ' << -1 << ' ';
        }

		for(int index =0; index < results.size(); ++index)
		{
			resultfile << results[index].getShapeId() << ' ' << results[index].getConfidence() << ' ';
		}

		resultfile << endl;

		results.clear();

	}

	if(total > 0)
	{
		LOG( LTKLogger::LTK_LOGLEVEL_DEBUG) << "total" + total <<
					"Error Count = " + errorCount << "  " +(total-errorCount)/total << endl;
		cout << total << ' ' << errorCount << ' ' << (total-errorCount)/total;
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
		(LTKSTRCMP(strVal, OPTION_TRAIN) == 0) ||
		(LTKSTRCMP(strVal, OPTION_TEST) == 0) ||
		(LTKSTRCMP(strVal, OPTION_PROJECT) == 0) ||
		(LTKSTRCMP(strVal, OPTION_PROFILE) == 0) ||
		(LTKSTRCMP(strVal, OPTION_OUTPUT) == 0) ||
		(LTKSTRCMP(strVal, OPTION_HEADERINFO) == 0) ||
		(LTKSTRCMP(strVal, OPTION_LOGFILE) == 0) ||
		(LTKSTRCMP(strVal, OPTION_LOGLEVEL) == 0) ||
		(LTKSTRCMP(strVal, OPTION_NUMCHOICES) == 0) ||
		(LTKSTRCMP(strVal, OPTION_INFILETYPE) == 0))
		return 1;

	return 0;
}


/*****************************************************************************
* AUTHOR		: Nidhi Sharma
* DATE			: 30-AUG-2005
* NAME			: getAbsolutePath
* DESCRIPTION	:
* ARGUMENTS		:
* RETURNS		:
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*****************************************************************************/
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

/*****************************************************************************
* AUTHOR		: Nidhi Sharma
* DATE			: 04-Jul-2008
* NAME			: cleanUp
* DESCRIPTION	:
* ARGUMENTS		:
* RETURNS		:
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*****************************************************************************/
int cleanUp(LTKShapeRecognizer* pReco, LTKOSUtil* utilPtr)
{
    int iErrorCode = SUCCESS;

    if (pReco != NULL)
    {
        iErrorCode = ptrObj->deleteShapeRecognizer(pReco);

        if (iErrorCode != SUCCESS )
        {
            cout << "Error deleting shape recognizer " << iErrorCode <<
                getErrorMessage(iErrorCode) << endl;

    		return iErrorCode;
        }
    }

    if(NULL != deleteLTKLipiEngine)
	{
		// delete lipiengine instance
		deleteLTKLipiEngine();
	}

	// Unload the DLL from memory
	iErrorCode = utilPtr->unloadSharedLib(hLipiEngine);

    if (iErrorCode != SUCCESS )
    {
        cout << "Error unloading lipiengine library " << endl;

		return FAILURE;
    }

    if (utilPtr != NULL )
    {
        delete utilPtr;
        utilPtr = NULL;
    }

	return SUCCESS;
}


int validateFeatureFile(const string& listFile, string& outFeatureExtractorName)
{
	ifstream tempInput(listFile.c_str());
	try
	{
		string firstLine;

		if(!tempInput)
		{
			cout << "Error: Fail to open " << listFile << " file.";
			return -1;
		}

		getline(tempInput,firstLine,'\n');
		LTKStringUtil::trimString(firstLine);

		vector<string> tokens;

		if(firstLine.size() <=2) //ensuring size before stripping the first and last angle brackets
		{
			cout<<"No header information in the feature file"<<endl;
			tempInput.close();
			return FAILURE;
		}

		firstLine = firstLine.substr(1,firstLine.size()-2); //removing the first and last angle brackets

		LTKStringUtil::tokenizeString(firstLine,"= ",tokens);

		if(tokens.size()!=2 && tokens[0]!=FE_NAME)
		{
			cout<<"Feature extractor name is missing in the feature file."<<endl;
			tempInput.close();
			return FAILURE;
		}

		outFeatureExtractorName = tokens[1];


		tempInput.close();
	}
	catch(...)
	{
		tempInput.close();
	}

	return SUCCESS;
}

/******************************************************************************
* AUTHOR		: Balaji MNA
* DATE			: 02-DEC-2008
* NAME			: unloadFeatureExtractor
* DESCRIPTION	: This method unloads the Feature extractor instance
* ARGUMENTS		: none
* RETURNS		: none
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description
******************************************************************************/
int unloadFeatureExtractor(LTKShapeFeatureExtractor* ptrFeatureExtractor,void *libHandlerFE)
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "ShapeFeature::deleteFeatureExtractorInstance()" << endl;

    if (ptrFeatureExtractor != NULL)
    {
        typedef int (*FN_PTR_DELETE_SHAPE_FEATURE_EXTRACTOR)(LTKShapeFeatureExtractor *obj);
        FN_PTR_DELETE_SHAPE_FEATURE_EXTRACTOR deleteFeatureExtractor;
        void * functionHandle = NULL;

        // Map createpreprocessor and deletePreprocessor functions
		LTKOSUtil*  osUtilPtr = LTKOSUtilFactory::getInstance();
        int returnVal = osUtilPtr->getFunctionAddress(libHandlerFE,
                                                DELETE_SHAPE_FEATURE_EXTRACTOR,
                                                &functionHandle);
        // Could not map the createLipiPreprocessor function from the DLL
    	if(returnVal != SUCCESS)
    	{
    	    LOG(LTKLogger::LTK_LOGLEVEL_ERR) <<
                "Error: "<< EDLL_FUNC_ADDRESS_DELETE_FEATEXT << " " <<
                getErrorMessage(EDLL_FUNC_ADDRESS_DELETE_FEATEXT) <<
                " unloadFeatureExtractor" << endl;

            LTKReturnError(EDLL_FUNC_ADDRESS_DELETE_FEATEXT);
    	}

        deleteFeatureExtractor = (FN_PTR_DELETE_SHAPE_FEATURE_EXTRACTOR)functionHandle;

        deleteFeatureExtractor(ptrFeatureExtractor);

        ptrFeatureExtractor = NULL;

        // unload feature extractor dll
        if(libHandlerFE != NULL)
    	{
            //Unload the DLL
    		osUtilPtr->unloadSharedLib(libHandlerFE);
    		libHandlerFE = NULL;

    	}

		if(NULL != osUtilPtr)
		{
			delete osUtilPtr;
			osUtilPtr = NULL;
		}
    }

    return SUCCESS;
}

int loadFeatureExtractor(string featureExtractorName,
						 LTKShapeFeatureExtractor** ptrFeatureExtractor,
						 void **libHandlerFE)
{
	int errorCode;
	try
	{
		LTKControlInfo controlInfo;
		controlInfo.projectName = strProjectName;
		controlInfo.profileName = strProfileName;
		controlInfo.lipiRoot = strLipiRootPath;
		controlInfo.toolkitVersion = SUPPORTED_MIN_VERSION;

		string recognizerType = SHAPE_RECOGNIZER_STRING;
		string outRecognizerString = "";

		string lipiRoot(strLipiRootPath);
		string projectName(strProjectName);
		string profileName(strProfileName);

		string profileCfgPath = lipiRoot + PROJECTS_PATH_STRING +
						projectName + PROFILE_PATH_STRING + profileName
						+ SEPARATOR + PROFILE_CFG_STRING;

		LTKConfigFileReader* profileConfigReader = NULL;

		try
		{
			profileConfigReader = new LTKConfigFileReader(profileCfgPath);
		}
		catch(LTKException e)
		{
			LOG( LTKLogger::LTK_LOGLEVEL_ERR) <<
				"Error: "<< getErrorMessage(e.getErrorCode()) << profileCfgPath <<
    			" ShapeFeature::initalize()"<<endl;

			LTKReturnError(e.getErrorCode());
		}

		errorCode = profileConfigReader->getConfigValue(recognizerType,
													outRecognizerString);
		controlInfo.cfgFileName = outRecognizerString;
		if(errorCode != SUCCESS)
		{
			/* No recognizer specified. */
			if (recognizerType == SHAPE_RECOGNIZER_STRING )
			{
				errorCode = ENO_SHAPE_RECOGNIZER;
			}
			else
			{
				errorCode = ENO_WORD_RECOGNIZER;
			}

			LOG( LTKLogger::LTK_LOGLEVEL_ERR) <<
				"Error: "<< getErrorMessage(errorCode) << profileCfgPath <<
    			" ShapeFeature::Initalize()"<<endl;

			delete profileConfigReader;

			LTKReturnError(errorCode);
		}

		delete profileConfigReader;

		controlInfo.cfgFileName = outRecognizerString;

		LTKShapeFeatureExtractorFactory factory;
		int errorCode = factory.createFeatureExtractor(featureExtractorName,
				strLipiRootPath,
				libHandlerFE,
				controlInfo,
				ptrFeatureExtractor);

		if(errorCode != SUCCESS)
		{
			LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<< EFTR_EXTR_NOT_EXIST << " " <<
				" ShapeFeature::initializeFeatureExtractorInstance()" << endl;
			LTKReturnError(errorCode);
		}

	}
	catch(LTKException e)
	{
		//Unloading the feature Extractor instance
	    unloadFeatureExtractor(*ptrFeatureExtractor,(*libHandlerFE));
		throw e;
	}
	return SUCCESS;
}



/**********************************************************************************
* AUTHOR		: Balaji MNA
* DATE			: 01-DEC-2008
* NAME			: ShapeRecognizerFromFeatureFile
* DESCRIPTION	: Load the model data and call recognize function and display the
*				  results.
* ARGUMENTS		: pReco - handle to LTKShapeRecognizer object
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int ShapeRecognizerFromFeatureFile(LTKShapeRecognizer **pReco)
{
	int iErrorCode;

	vector<int> shapeSubSet; //subset of classes to be compared for recognition. If empty, all classes are compared
	vector<LTKShapeRecoResult> results;

	string strShapeFeature;
	string numSamples;
	string strShapeId;

	int intShapeId;
	int errorCount = 0;
	int total = 0;

    stringVector tokens;
    stringVector classToken;

	string strFeatureVector;
	results.reserve(numChoices+1);


	try
	{
		int iResult = (*pReco)->loadModelData();
		if(iResult != 0)
		{
			cout << "Error loading Model data." << endl;
			LOG( LTKLogger::LTK_LOGLEVEL_ERR) << "Error loading Model data." << endl;

			cout << "Error : " <<  getErrorMessage(iResult) << endl;
			return -1;
		}
	}

	catch(LTKException e)
	{
		LOG( LTKLogger::LTK_LOGLEVEL_ERR) << e.getExceptionMessage() << endl;

		return FAILURE;
	}

	string featureExtractorName;
	int errorCode = validateFeatureFile(strTestLstFile,featureExtractorName);
	if(errorCode != SUCCESS || featureExtractorName.empty() )
	{
		return FAILURE;
	}

	ifstream in(strTestLstFile);
	if(in == NULL)
	{
		LOG( LTKLogger::LTK_LOGLEVEL_ERR) << "Test list file open error" + string(strTestLstFile) << endl;
		cout << "Test list file open error : " << strTestLstFile << endl;
		return -1;
	}

	LTKShapeFeatureExtractor* ptrFeatureExtractor = NULL;
	void *libHandlerFE = NULL;

	loadFeatureExtractor(featureExtractorName, &ptrFeatureExtractor,&libHandlerFE);

	if(NULL == ptrFeatureExtractor || NULL == libHandlerFE)
	{
		in.close();
		return FAILURE;
	}
	ofstream resultfile(strOutputFileName);

	while(getline(in, strFeatureVector, NEW_LINE_DELIMITER ))
	{
		LTKStringUtil::tokenizeString(strFeatureVector,
				CLASSID_FEATURES_DELIMITER,  classToken);

		if(classToken.size() != 2)
			continue;

		intShapeId = atoi((classToken[0]).c_str());

		if(intShapeId == -1)
			continue;

		LTKStringUtil::tokenizeString(classToken[1],  FEATURE_EXTRACTOR_DELIMITER,  tokens);

		vector<LTKShapeFeaturePtr> shapeFeatureVector;

		for(int i = 0; i < tokens.size(); ++i)
		{
			LTKShapeFeaturePtr shapeFeature = ptrFeatureExtractor->getShapeFeatureInstance();

			if (shapeFeature->initialize(tokens[i]) != SUCCESS)
			{
				LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< EINVALID_INPUT_FORMAT << " " <<
					"Number of features extracted from a trace is not correct"<<
					" ShapeRecognizerFromFeatureFile()" << endl;

				unloadFeatureExtractor(ptrFeatureExtractor,libHandlerFE);

				LTKReturnError(EINVALID_INPUT_FORMAT);
			}

			shapeFeatureVector.push_back(shapeFeature);
		}

		iErrorCode = (*pReco)->recognize(shapeFeatureVector, shapeSubSet, confThreshold, numChoices, results);

		if(iErrorCode != SUCCESS)
		{
			//Use LOGGER
			cout << "Error : " <<  getErrorMessage(iErrorCode) << endl;
			unloadFeatureExtractor(ptrFeatureExtractor,libHandlerFE);

			return -1; //return ierrorcode
		}

		resultfile << "INPUT_FROM_FEATURE_FILE" << '\t' << intShapeId << ' ';
        if (results.size() == SUCCESS)
        {
            resultfile << -1 << ' ' << -1 << ' ';
        }

		for(int index =0; index < results.size(); ++index)
		{
			resultfile << results[index].getShapeId() << ' ' << results[index].getConfidence() << ' ';
		}

		resultfile << endl;

		results.clear();

	}

	unloadFeatureExtractor(ptrFeatureExtractor,libHandlerFE);
	if(total > 0)
	{
		LOG( LTKLogger::LTK_LOGLEVEL_DEBUG) << "total" + total <<
					"Error Count = " + errorCount << "  " +(total-errorCount)/total << endl;
		cout << total << ' ' << errorCount << ' ' << (total-errorCount)/total;
	}


	return SUCCESS;

}
/**********************************************************************************
* AUTHOR		: Anish Kumar
* DATE			: 30-AUG-2005
* NAME			: evaluateAdapt
* DESCRIPTION	: 
* ARGUMENTS		: 
* RETURNS		: -1 on error 0 on success
* NOTES			:
* CHANGE HISTROY
* Author			Date				
* 					
*************************************************************************************/
int evaluateAdapt(LTKShapeRecognizer **addReco, string projectName)
{
        int iErrorCode;
		int errorCode;
	string recognizerName;

	errorCode = getRecognizerName(strProjectName,strProfileName,recognizerName);
	if(errorCode != SUCCESS)
	{
		LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<
			" " << getErrorMessage(errorCode) << "RunShaperec::evaluateAdapt()" 
			<<endl;
		LTKReturnError(errorCode);
	}

	//if recognizer name is ACTIVEDTW call evaluateAdaptActiveDTW 
	if(recognizerName == ACTIVEDTW)
	{
		errorCode = evaluateAdaptActiveDTW(addReco,projectName);
		LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<
			" " << getErrorMessage(errorCode) << "RunShaperec::evaluateAdapt()" 
			<<endl;
		LTKReturnError(errorCode);
	}
	else
	{

		LTKCaptureDevice captureDevice;
		LTKScreenContext screenContext;

		vector<int> shapeSubSet; //subset of classes to be compared for recognition. If empty, all classes are compared
		vector<LTKShapeRecoResult> results;
		string path;
		int trueShapeId;

		string adaptCharCheck;
		string lineBuffer;
		ifstream adaptFileHandle;
		string line = "";
		//Flag is set when EOF is reached
		bool eofFlag = false;
		stringVector tokens;
		//ID for each shapes
		int shapeId = -1;

		LTKTraceGroup traceGroup;
		LTKShapeRecoUtil shapeRecoUtil;
		bool projectTypeDynamic = false;
		unsigned short numShapes = 0;
		string strNumShapes = "";
		string strProfileDirectory;
		string tempLipiRootPath = strLipiRootPath;
		strProfileDirectory = tempLipiRootPath + PROJECTS_PATH_STRING +projectName + PROFILE_PATH_STRING;

		string projectCFGPath;
		projectCFGPath = strProfileDirectory + PROJECT_CFG_STRING;

		
		int errorCode = shapeRecoUtil.isProjectDynamic(projectCFGPath,
			numShapes, strNumShapes, projectTypeDynamic);

		if( errorCode != SUCCESS)
		{
			LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<
			" " << getErrorMessage(errorCode) << "RunShaperec::evaluateAdapt()" 
			<<endl;
			LTKReturnError(errorCode);
		}
		

		if(!projectTypeDynamic)
		{	 
			LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " << errorCode << " " <<
			"RunShaperec::evaluateAdapt()" <<endl;
			LTKReturnError(EPROJ_NOT_DYNAMIC);
		}

		
	
		adaptFileHandle.open(strAdaptListFile, ios::in);

		ofstream resultfile(strOutputFileName);
	
		(*addReco)->setDeviceContext(captureDevice);

		
		int sampleCount = 1;
	
		while(!adaptFileHandle.eof())
		{		
			getline(adaptFileHandle, line, NEW_LINE_DELIMITER);
			path  = "";
			if( adaptFileHandle.eof() )
			{
				eofFlag = true;
			}
			if ( line[0] == COMMENTCHAR )
			{
				continue;
			}

			if (eofFlag == false)
			{
				/* Print the line to be parsed	*/
				cout << line <<" "<<endl;	
				sampleCount++;

				//Tokenize the string
				errorCode = LTKStringUtil::tokenizeString(line,  LIST_FILE_DELIMITER,  tokens);

				if( errorCode != SUCCESS )
				{
					LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< errorCode << " " <<
					"RunShaperec::evaluateAdapt()" << endl;
					LTKReturnError(errorCode);
				}

				//Tokens must be of size 2, one is pathname and other is shapeId
				//If the end of file not reached then continue the looping
				if( tokens.size() > 3 )
				{
					LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<<
					"Number of tokens identified in the line is > 3 " <<
					line << endl;
					cout<< "Error: Number of tokens identified in the line is > 3 " <<
					line << endl;
				
					continue;
				}
				//Tokens must be of size 3, one is pathname ,second is shapeId and last is Adapt Character
				if( tokens.size() == 3)
				{
					LTKTraceGroup traceGroup;
					if(tokens[2] != ADAPTCHAR)
					{
						LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<<
						"Invalid token " <<
						tokens[2] << endl;
						cout<< "Error: Invalid token " << tokens[2] << endl;
					
						continue;
					}

					path = tokens[0];//Tokens[0] indicates the path name

					/* To get the Absolute Path from the ink file	*/					
					int error = getAbsolutePath(path, strLipiRootPath);

					iErrorCode = LTKInkFileReader::readUnipenInkFile(path, traceGroup, captureDevice, screenContext);
					if(iErrorCode != SUCCESS)
					{
						LOG(LTKLogger::LTK_LOGLEVEL_ERR) << getErrorMessage(iErrorCode) << endl;
						cout << "Error while reading the input file " << endl;
						cout << getErrorMessage(iErrorCode) << endl;

						LTKReturnError(iErrorCode);
					}
					//Tokens[1] indicates the shapeId
					shapeId = atoi( tokens[1].c_str() );
					if(shapeId < 0)
					{
						LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<<
						"NNShapeRecognizer requires training file class Ids to be positive integers "<<
						"RunShaperec::evaluateAdapt()" << endl;
						LTKReturnError(iErrorCode);
					}
					(*addReco)->addSample(traceGroup, shapeId);	
					
				}
				if( tokens.size() == 2)
				{				
					//cout<<"inside adapt character"<<endl;
					trueShapeId = atoi( tokens[1].c_str() );

					LTKTraceGroup traceGroup;
					path = tokens[0];
					/* To get the Absolute Path from the ink file	*/
					int error = getAbsolutePath(path, strLipiRootPath);

					iErrorCode = LTKInkFileReader::readUnipenInkFile(path, traceGroup, captureDevice, screenContext);
					if(iErrorCode != SUCCESS)
					{
						cout << "Error while reading the input file " << endl;
						cout << getErrorMessage(iErrorCode) << endl;
						LOG(LTKLogger::LTK_LOGLEVEL_ERR) << getErrorMessage(iErrorCode) << endl;
						LTKReturnError(iErrorCode);
					}
				
					
					iErrorCode = (*addReco)->recognize(traceGroup, screenContext, shapeSubSet, confThreshold, numChoices, results);
					if(iErrorCode != SUCCESS)
					{
						cout << "Error : " <<  getErrorMessage(iErrorCode) << endl;
						LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<iErrorCode <<getErrorMessage(iErrorCode)
						<<" Recognize()"<<endl;
						LTKReturnError(iErrorCode);
					}

					resultfile << path << '\t' << trueShapeId << ' ';

					if (results.size() == 0)
					{
						resultfile << -1 << ' ' << -1 << ' ';
					}

					for(int index =0; index < results.size(); ++index)
					{
						resultfile << results[index].getShapeId() << ' ' << results[index].getConfidence() << ' ';
					}
					resultfile << endl;

					

					//Call Adapt after recognize
					(*addReco)->adapt(trueShapeId);
				
					

					results.clear();
				}
			}
		}
		printf("\n");
        }
	return SUCCESS;
}

int getRecognizerName(const string strProjectName,const string strProfileName, string& outRecognizerString)
{
 
    int errorCode;
    string tempLipiRootPath = strLipiRootPath;
    string profileCfgPath = tempLipiRootPath + PROJECTS_PATH_STRING + strProjectName + PROFILE_PATH_STRING + strProfileName + SEPARATOR + PROFILE_CFG_STRING;
 
    LTKConfigFileReader* profileConfigReader = NULL;
 
    try
    {
        profileConfigReader = new LTKConfigFileReader(profileCfgPath);
	
    }
    catch(LTKException e)
    {
	
        LOG( LTKLogger::LTK_LOGLEVEL_ERR) <<
            "Error: "<< getErrorMessage(e.getErrorCode()) << profileCfgPath <<
            " LTKLipiEngineModule::validateProfile()"<<endl;
 
        LTKReturnError(e.getErrorCode());
    }
 
    errorCode = profileConfigReader->getConfigValue(SHAPE_RECOGNIZER_STRING,
                                                    outRecognizerString);
   
 
    if(errorCode != SUCCESS)
    {
        
        errorCode = ENO_SHAPE_RECOGNIZER;
        
        
        LOG( LTKLogger::LTK_LOGLEVEL_ERR) <<
            "Error: "<< getErrorMessage(errorCode) << profileCfgPath <<
            " LTKLipiEngineModule::validateProfile()"<<endl;
 
        delete profileConfigReader;
 
        LTKReturnError(errorCode);
    }
 
    delete profileConfigReader;

    return SUCCESS;
 
}

int evaluateAdaptActiveDTW(LTKShapeRecognizer **addReco, const string strProjectName)
{
	int errorCode;	
	LTKCaptureDevice captureDevice;
	LTKScreenContext screenContext;

	vector<int> shapeSubSet; //subset of classes to be compared for recognition. If empty, all classes are compared
	vector<LTKShapeRecoResult> results;
	string path;

	//incase the shapeid of the sample is not specified
        //we assign it a new shape id based on the exiting shape ids

	int trueShapeId = NEW_SHAPEID;

	string adaptCharCheck;
	string lineBuffer;
	ifstream adaptFileHandle;
	string line = "";
	//Flag is set when EOF is reached
	bool eofFlag = false;

	//a flag used to detect if a new shape has been added
	bool addFlag = false;
	stringVector tokens;
	//ID for each shapes
	int shapeId = -1;

	LTKTraceGroup traceGroup;
	LTKShapeRecoUtil shapeRecoUtil;
	bool projectTypeDynamic = false;
	unsigned short numShapes = 0;
	string strNumShapes = "";
	string strProfileDirectory;
	string tempLipiRootPath = strLipiRootPath;
	strProfileDirectory = tempLipiRootPath + PROJECTS_PATH_STRING + strProjectName + PROFILE_PATH_STRING;

	string projectCFGPath;
	projectCFGPath = strProfileDirectory + PROJECT_CFG_STRING;

	errorCode = shapeRecoUtil.isProjectDynamic(projectCFGPath,
			numShapes, strNumShapes, projectTypeDynamic);

	if( errorCode != SUCCESS)
	{
		LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<
		" " << getErrorMessage(errorCode) << "RunShaperec::evaluateAdaptActiveDTW()" 
		<<endl;
		LTKReturnError(errorCode);
	}
		

	if(!projectTypeDynamic)
	{	 
		LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " << errorCode << " " <<
		"RunShaperec::evaluateAdaptActiveDTW()" <<endl;
		LTKReturnError(EPROJ_NOT_DYNAMIC);
	}

	adaptFileHandle.open(strAdaptListFile, ios::in);
	ofstream resultfile(strOutputFileName);
	(*addReco)->setDeviceContext(captureDevice);

	//loading model data
	errorCode = (*addReco)->loadModelData();
	

	if(errorCode != SUCCESS)
	{
		if(errorCode == EMODEL_DATA_FILE_OPEN)
		{
			LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Model Data File Does Not Exist" << endl;
		
			addFlag = true;
		}
		else
		{
			LOG( LTKLogger::LTK_LOGLEVEL_ERR) << "Error : " <<
			errorCode << getErrorMessage(errorCode) << "Error in evaluateAdaptActiveDTW()"
			<<endl;
			LTKReturnError(errorCode);
		}
	}

	int sampleCount = 1;
	while(!adaptFileHandle.eof())
	{
		getline(adaptFileHandle, line, NEW_LINE_DELIMITER);
		path  = "";
		
		if( adaptFileHandle.eof() )
		{
			eofFlag = true;
		}
		if ( line[0] == COMMENTCHAR )
		{
			continue;
		}

		if (eofFlag == false)
		{

			/* Print the line to be parsed	*/
			cout << line <<" "<<sampleCount<< endl;	
			sampleCount++;

			//Tokenize the string
			errorCode = LTKStringUtil::tokenizeString(line,  LIST_FILE_DELIMITER,  tokens);

			if( errorCode != SUCCESS )
			{
				LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< errorCode << " " <<
				"RunShaperec::evaluateAdapt()" << endl;
				LTKReturnError(errorCode);
			}

			//Tokens must be of size 2, one is pathname and other is shapeId
			//If the end of file not reached then continue the looping
			//if( tokens.size() > 2 )
			if( tokens.size() > 3 )
			{
				LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<<
				"Number of tokens identified in the line is > 2 " <<
				line << endl;
				cout<< "Error: Number of tokens identified in the line is > 2 " <<
				line << endl;
				
				continue;
			}
			if( tokens.size() == 3)
			{

				if(tokens[2] != ADAPTCHAR)
				{
					LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<<
						"Invalid token " <<
						tokens[2] << endl;
					cout<< "Error: Invalid token " << tokens[2] << endl;

					continue;
				}
			}

			trueShapeId = atoi( tokens[1].c_str() );
			
			LTKTraceGroup traceGroup;
			path = tokens[0];
			/* To get the Absolute Path from the ink file	*/
			int error = getAbsolutePath(path, strLipiRootPath);

			errorCode = LTKInkFileReader::readUnipenInkFile(path, traceGroup, captureDevice, screenContext);
			if(errorCode != SUCCESS)
			{
				cout << "Error while reading the input file " << endl;
				cout << getErrorMessage(errorCode) << endl;
				LOG(LTKLogger::LTK_LOGLEVEL_ERR) << getErrorMessage(errorCode) << endl;
				LTKReturnError(errorCode);
			}

			//if addFlag set to true add class 
			if(addFlag)
			{
				errorCode = (*addReco)->addClass(traceGroup,trueShapeId);
				if(errorCode != SUCCESS)
				{
					LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<getErrorMessage(errorCode)
					<<"Runshaperec:addClass()"<<endl;
					LTKReturnError(errorCode);				
				}
			}
			else
			{
				//call recognize
				errorCode = (*addReco)->recognize(traceGroup, screenContext, shapeSubSet, confThreshold, numChoices, results);
				if(errorCode != SUCCESS)
				{
					LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<getErrorMessage(errorCode)
					<<"Runshaperec:recognize()"<<endl;
					LTKReturnError(errorCode);				
				}

				resultfile << path << '\t' << trueShapeId << ' ';

				if (results.size() == 0)
				{
					resultfile << -1 << ' ' << -1 << ' ';
				}

				for(int index =0; index < results.size(); ++index)
				{
					resultfile << results[index].getShapeId() << ' ' << results[index].getConfidence() << ' ';
				}
				resultfile << endl;

				errorCode = (*addReco)->adapt(trueShapeId);

				if(errorCode != SUCCESS)
				{
					if(errorCode == EINVALID_SHAPEID)
					{
						errorCode = (*addReco)->addClass(traceGroup,trueShapeId);

						if(errorCode != SUCCESS)
						{
							LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<getErrorMessage(errorCode)
							<<" addClass()"<<endl;
							LTKReturnError(errorCode);
						}
					}
					else
					{
						LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<errorCode <<getErrorMessage(errorCode)
						<<" adapt()"<<endl;
						LTKReturnError(errorCode);
					}
				}
			}
			
			results.clear();
			
		}
		//resetting adaptFlag
		addFlag = false;
	}

	return SUCCESS;
	
}


void fillSampleSetVec(char *szSampleSet)
{
	int nStart=0, nEnd=0, nIndex =0;
	char *szToken = NULL;
	char szStart[50], szEnd[50];

	for( int i =0; szSampleSet[i] != '\0'; i++)
	{
		if('-' == szSampleSet[i])
		{
			nIndex = i;
			szToken = &szSampleSet[i];
			break;			 
		}
	}
	szToken+=1;

	strncpy(szStart, szSampleSet, nIndex);
	strcpy(szEnd, szToken);

	nStart = atoi(szStart);
	nEnd = atoi(szEnd);

	for( int i=nStart; i<=nEnd; i++)
		shapeSubSet.push_back(i);

}