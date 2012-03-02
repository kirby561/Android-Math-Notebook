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

/***************************************************************************
 * FILE DESCR: Definitions of Model Data Viewer main function.
 *
 * CONTENTS:   
 *		main
 *		helpDisplay
 *		mapOptions
 *	
 *		
 *		 
 * AUTHOR:  Vijayakumara M
 *
 * DATE:    Aug 23, 2005
 * CHANGE HISTORY:
 * Author		Date			Description of change
 ****************************************************************************/
#pragma warning (disable : 4786)

#include "mdv.h"
#include "LTKCheckSumGenerate.h"
#include "LTKStringUtil.h"


/**********************************************************************************
* AUTHOR		: Vijayakumara M
* DATE			: 23 Aug 2005
* NAME			: main
* DESCRIPTION	: Model Data viewer main function - Takes file name as its argument
*				  and checks the integrity of the file and displays the options user
*				  entered in the command prompt.
* ARGUMENTS		: int argc, char *argv[]  ( file name and user desired options)
* RETURNS		: 0 on success and -1 on failure.
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
************************************************************************************/
int main(int argc, char* argv[])
{

	string m_strLipiRootPath    = "";
    string m_logFileName        = DEFAULT_LOG_FILE;
    string m_logLevelStr        = "ERROR";
    LTKLogger::EDebugLevel m_logLevel = DEFAULT_LOG_LEVEL;

	int index = 1;
	int tmp = 0;
	int str = 0;
    int filteredArgc = argc;
	
	bool filFlag = false;		// Flag to indicate the file name specified in the comman prompt.
	bool allFlag = false;		// Flag to indicate all options to be displayed or not.
	bool helpFlag = false;
    bool preprocFlag = false;   // Flag to indicate preproc fields to be displayed.
	
	stringStringMap headerSequence;	// Get the maped header tokenized stings.

	stringStructMap optMap;		// Gets the options and corresponding values.

    stringStructMap optPreProcMap; // Gets the Preprocessing options and corresponding values.
	
	string fileName;			// Model Data file Name.

	string description;			// Option description

	LTKCheckSumGenerate chFile;			// instance of checkSum class for checking file integrity.

	//Display order
	char optIndex[][20]={"-PROJNAME","-NUMSHAPES","-RECNAME","-RECVER","-CHECKSUM","-CREATETIME","-MODTIME","-HEADERLEN","-DATAOFFSET","-HEADERVER","-BYTEORDER", "-FEATEXTR"};
    
    if (argc == 1)
    {
        helpDisplay();
        return SUCCESS;
    }
	
	// Get the argument passed through command prompt.
	while(index < argc)
	{
		// If argument is equal to "-a" or "-all" set allFlag for to display all the options.
		if(LTKSTRCMP(argv[index], OPTION_ALL) == 0)
		{
			allFlag = true;
			index++;
			continue;
		}

        //Preproc
        if(LTKSTRCMP(argv[index], OPTION_PREPROC) == 0)
		{
			preprocFlag = true;
			index++;
			continue;
		}

		if(LTKSTRCMP(argv[index], OPTION_HELP) == 0)
		{
			// Call helpDisplay function for displaying the help notice.
			helpDisplay();
			return SUCCESS;
		}

        if(LTKSTRCMP(argv[index], OPTION_LIPI_ROOT) == 0)
		{
			if (index+1 >= argc)
            {
                cout << "Please specify LIPI_ROOT " << endl;
                return FAILURE;
            }

            m_strLipiRootPath = argv[index+1];
            index++;
		}

		if(LTKSTRCMP(argv[index], OPTION_INPUT) == 0)
		{
			//set filFlag if the file Name is spcified
			if(index+1 < argc)
			{
				filFlag = true;
				fileName = argv[index+1];
				index++;
			}
		}

		if(LTKSTRCMP(argv[index], OPTION_VER) == 0)
		{
			cout << CURRENT_VERSION << endl;
			return SUCCESS;
		}

        if(LTKSTRCMP(argv[index], OPTION_LOGFILE) == 0)
		{
			if (index+1 >= argc)
            {
                cout << "Please specify log file name " << endl;
                return FAILURE;
            }

            m_logFileName = argv[index+1];
            filteredArgc = argc-2;
            index++;
		}
		if(LTKSTRCMP(argv[index], OPTION_LOGLEVEL) == 0)
		{
			if (index+1 >= argc)
            {
                cout << "Please specify log level " << endl;
                return FAILURE;
            }

            m_logLevelStr = argv[index+1];

            int errorCode = mapLogLevel(m_logLevelStr, m_logLevel);
            
            if (errorCode != SUCCESS)
            {
                LTKReturnError(errorCode);
            }

            filteredArgc = argc-2;
            index++;
		}

		index++;
	}

    // If nothing specified except the file, display all
	if( (filteredArgc == 5 || filteredArgc == 3) && filFlag) 
    {   
		allFlag = true;
    }

	index = 1;

    if (m_strLipiRootPath.empty())
    {
		char* envstring = NULL;
		envstring = getenv(LIPIROOT_ENV_STRING);

		if(envstring == NULL )
		{
			cout << "Error, Environment variable is not set LIPI_ROOT" << endl;
			return FAILURE;
		}
		m_strLipiRootPath = envstring;
		envstring = NULL;
    }
    
    // Configure logger
    LTKLoggerUtil::createLogger(m_strLipiRootPath);
    LTKLoggerUtil::configureLogger(m_logFileName, m_logLevel);

	//if filFlag is set, display File name is not specified and display help
	if(filFlag == false)
	{
		cout <<endl<<endl<<"Model data file Name is not given " <<endl;
		cout <<"Please specify the file Name with \"-input\" option" << endl;

		cout << endl;
		//Display Help 
		helpDisplay();
		cout << endl;

		LTKLoggerUtil::destroyLogger();
		return FAILURE;
	}

	// Check file integrity. If file has been altered then set the chflag.
	if((tmp = chFile.readMDTHeader(fileName, headerSequence)) != SUCCESS)
	{
		if( tmp == EMODEL_DATA_FILE_OPEN)
		{
			cout << "Unable to open model data file" << endl;
			LTKLoggerUtil::destroyLogger();
			return FAILURE;
		}
		else if( tmp == EMODEL_DATA_FILE_FORMAT)
		{
			cout << "Incompatible model data file. The header is not in the desired format." << endl;
			LTKLoggerUtil::destroyLogger();
			return FAILURE;
		}
		else if( tmp == EINVALID_INPUT_FORMAT)
		{
			cout << endl << "Model data file is corrupted" << endl;
			LTKLoggerUtil::destroyLogger();
			return FAILURE;
		}
	}
	else
	{
			cout << endl << "Checksum successfully verified"<<endl<<endl;
	}

    if(preprocFlag)
	{
       	// Map preprocessing options with their values.
        mapPreprocOptions(optPreProcMap, headerSequence);
	    DisplayPreProc(optPreProcMap);
		LTKLoggerUtil::destroyLogger();
		return 0;
	}

   	// Map options with their values.
	mapOptions(optMap, headerSequence);

	// Iterator pointing at the begining of the option mapped address.
	map<string,  option>::iterator the_begiter = optMap.begin();


	if(allFlag)
	{
        str = 0;
		while (optIndex[str][0] != '\0')
		{
			if(!(optMap[optIndex[str]].description.empty()))
					cout << optMap[optIndex[str]].description << optMap[optIndex[str]].value << endl;
			str++;
		}

        // Map preprocessing options with their values.
        mapPreprocOptions(optPreProcMap, headerSequence);
	    DisplayPreProc(optPreProcMap);


		if( !(headerSequence[PLATFORM].empty()))
		{
			cout << "Platform		   - " << headerSequence[PLATFORM] << endl;			
			cout << "			     " << headerSequence[OSVERSION] << endl;
			cout << "			     " << headerSequence[PROCESSOR_ARCHITEC] << endl;
		}

		if( !(headerSequence[COMMENT].empty()))
		{
			cout << "Comment			   - " << headerSequence[COMMENT] << endl;
			cout << "Comment length		   - " << headerSequence[COMMENTLEN] << endl;	
		}

		if( !(headerSequence[DATASET].empty()))
		{
			cout << "Dataset name		   - " << headerSequence[DATASET] << endl;			
		}	

		LTKLoggerUtil::destroyLogger();
		return 0;
	}

    
	// Display the values if file is present and in the desired format
	while((index < argc))
	{
		if( (LTKSTRCMP(argv[index], "-INPUT") == 0) || 						
			(LTKSTRCMP(fileName.c_str(), argv[index]) == 0) ||
			(LTKSTRCMP(argv[index], "-LIPIROOT") == 0) || 						
			(LTKSTRCMP(m_strLipiRootPath.c_str(), argv[index]) == 0))
		{
			++index;	
			continue;
		}

		if( (LTKSTRCMP(argv[index], "-DATASET") == 0) || 						
			(LTKSTRCMP(argv[index], "-COMMENT") == 0) ||
			(LTKSTRCMP(argv[index], "-COMMENTLEN") == 0))
		{
			++index;
			continue;
		}

		if(LTKSTRCMP(argv[index], "-PLATFORM") == 0)
		{
			cout << "Platform		   - " << headerSequence[PLATFORM] << endl;
			cout << "			     " << headerSequence[OSVERSION] << endl;
			cout << "			     " << headerSequence[PROCESSOR_ARCHITEC] << endl;

			++index;
			continue;
		}

		description = optMap[stringToUpper(argv[index])].description;

		if(description.empty())
			cout << "UnKnown Option - "<< argv[index] << endl;
		else 
			cout << optMap[argv[index]].description << optMap[argv[index]].value << endl;

		++index;	
	}
	
	LTKLoggerUtil::destroyLogger();
	return SUCCESS;
}

/**********************************************************************************
* AUTHOR		: Vijayakumara M
* DATE			: 23 Aug 2005
* NAME			: helpDisplay
* DESCRIPTION	: This function reades the contents of help.txt file and Displays it
*				  on the screen.
* ARGUMENTS		: - No -
* RETURNS		: 0 on success and -1 on failure.
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
************************************************************************************/
int helpDisplay()
{
	cout <<	"mdv -input <md file name> -lipiroot <lipi root> \n "
         << " <option1> <option2> ...\n"<< endl;
	
	cout << "options :" << endl;
	
	cout << "[-help]		-	Displays the usage of this tool" << endl;
	cout << "[-projname]	-	Displays the name of the project" << endl;
	cout << "[-numshapes]	-	Displays the number of shapes" << endl;
	cout << "[-recname]	-	Displays the name of the shape recognizer" << endl;
	cout << "[-recver]	-	Displays the version the shape recognizer" << endl;
	cout << "[-checksum]	-	Displays the checksum of the file" << endl;
	cout << "[-createtime]	-	Displays the date on which the modle data file created" << endl;
	cout << "[-modtime]	-	Displays the last modified date of model data file" << endl;
	cout << "[-headerlen]	-	Displays the length of the model data header" << endl;
	cout << "[-dataoffset]	-	Displays the byte offset value of the start of data in file" << endl;
	cout << "[-headerver]	-	Displays the version of model data header" << endl;
	cout << "[-byteorder]	-	Displays the byte order - Little endian or big endian" << endl;
	cout << "[-platform]	-	Displays the platform on which this file is created" << endl;
	cout << "[-ver]		-	Displays the version of this tool" << endl;
    cout << "[-preproc]	-	Displays the preproc fields" << endl;
	cout << "[-all]		-	Displays all the fields" << endl;
	
	return SUCCESS;
}

/**********************************************************************************
* AUTHOR		: Vijayakumara M
* DATE			: 23 Aug 2005
* NAME			: mapOptions
* DESCRIPTION	: Do the mapping with the option and the respective description and value.
* ARGUMENTS		: - No -
* RETURNS		: SUCCESS on successful map operation.
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int mapOptions(stringStructMap &optMap, stringStringMap &headerSequence)
{
	option obj;
	char *ptr;

	obj.description = "Project name		   - ";
	obj.value = headerSequence[PROJNAME];
	optMap["-PROJNAME"] =obj;

	obj.description = "Number of shapes used	   - ";
	obj.value = headerSequence[NUMSHAPES];
	optMap["-NUMSHAPES"] = obj;	

	
	obj.description = "Shape recognizer name	   - ";
	obj.value = headerSequence[RECNAME];
	optMap["-RECNAME"] = obj;

	obj.description = "Shape recognizer version   - ";
	obj.value = headerSequence[RECVERSION];
	optMap["-RECVER"] = obj;
	
	obj.description = "Checksum		   - ";
	obj.value = headerSequence[CKS];
	optMap["-CHECKSUM"] = obj;
	
	obj.description = "Feature Extractor	   - ";
	obj.value = headerSequence[FE_NAME];
	optMap["-FEATEXTR"] = obj;

	obj.description = "Data creation time	   - ";
	obj.value = headerSequence[CREATETIME].c_str();
	optMap["-CREATETIME"] = obj;

	obj.description = "Data modified time	   - ";
	obj.value = headerSequence[MODTIME].c_str();
	optMap["-MODTIME"] = obj;
	
	obj.description = "Header length		   - ";
	obj.value = headerSequence[HEADERLEN];
	optMap["-HEADERLEN"] = obj;

	obj.description = "Data offset		   - ";
	obj.value = headerSequence[DATAOFFSET];
	optMap["-DATAOFFSET"] = obj;

	obj.description = "Header version		   - ";
	obj.value = headerSequence[HEADERVER];
	optMap["-HEADERVER"] = obj;

	obj.description = "Byte order		   - ";
	if(headerSequence[BYTEORDER] == "LE")
		obj.value = "Little endian";
	else
		obj.value = "Big endian";	
	optMap["-BYTEORDER"] = obj;

    return SUCCESS;
}

/*****************************************************************************
* AUTHOR		: Vijayakumara M
* DATE			: 23 Aug 2005
* NAME			: StringToUpper
* DESCRIPTION	: This method converts the string to upper case.
* ARGUMENTS		: String to be convert in to upper case.
* RETURNS		: The converted string in upper uase.
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*****************************************************************************/
char* stringToUpper(char *strToConvert)
{
	int i;

	//change each element of the string to upper case
   for(i=0;i< strlen(strToConvert);i++)
   {
      strToConvert[i] = toupper(strToConvert[i]);
   }

   return strToConvert;//return the converted string
}

/*****************************************************************************
* AUTHOR		: Nidhi Sharma
* DATE			: 11 Aug 2008
* NAME			: mapLogLevel
* DESCRIPTION	: 
* ARGUMENTS		: 
* RETURNS		: 
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*****************************************************************************/
int mapLogLevel(const string& logLevelStr, LTKLogger::EDebugLevel& outLogLevel)
{
    const char * strLogLevelPtr = logLevelStr.c_str();
	// mapping m_LogLevel to Logger log levels

	if(LTKSTRCMP(strLogLevelPtr, LOG_LEVEL_DEBUG) == 0)
	{
		outLogLevel = LTKLogger::LTK_LOGLEVEL_DEBUG;
	}
	else if(LTKSTRCMP(strLogLevelPtr, LOG_LEVEL_ALL) == 0)
	{
		outLogLevel = LTKLogger::LTK_LOGLEVEL_ALL;
	}
	else if(LTKSTRCMP(strLogLevelPtr, LOG_LEVEL_VERBOSE) == 0)
	{
		outLogLevel = LTKLogger::LTK_LOGLEVEL_VERBOSE;
	}
	else if(LTKSTRCMP(strLogLevelPtr, LOG_LEVEL_ERROR) == 0)
	{
		outLogLevel = LTKLogger::LTK_LOGLEVEL_ERR;
	}
	else if(LTKSTRCMP(strLogLevelPtr, LOG_LEVEL_OFF) == 0)
	{
		outLogLevel = LTKLogger::LTK_LOGLEVEL_OFF;
	}
	else if(LTKSTRCMP(strLogLevelPtr, LOG_LEVEL_INFO) == 0)
	{
		outLogLevel = LTKLogger::LTK_LOGLEVEL_INFO;
	}
	else
	{
		LTKReturnError(EINVALID_LOG_LEVEL); 
	}

	return SUCCESS;

}

/*****************************************************************************
* AUTHOR		: Balaji, M N A
* DATE			: 21 Aug 2008
* NAME			: DisplayPreProc
* DESCRIPTION	: 
* ARGUMENTS		: 
* RETURNS		: 
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*****************************************************************************/
void DisplayPreProc(stringStructMap optPreprocMap)
{
    char preprocOptIndex[NO_PREPROC_FIELDS][20]={"-TRACE_DIM" , 
                                                 "-RESAMP_POINT_ALLOC" , 
                                                 "-DOT_SIZE_THRES" , 
                                                 "-NORM_LN_WID_THRES" , 
                                                 "-PRESER_ASP_RATIO" , 
                                                 "-ASP_RATIO_THRES" , 
                                                 "-PRESER_REL_Y_POS" , 
                                                 "-SMOOTH_WIND_SIZE" , 
                                                 "-PREPROC_SEQ" };

    cout << "Preprocessing configuration values" << endl;
    for (int i=0; i < NO_PREPROC_FIELDS && preprocOptIndex[i][0] != '\0' ; i++)
	{
		if(!(optPreprocMap[preprocOptIndex[i]].description.empty()))
				cout << "\t" << optPreprocMap[preprocOptIndex[i]].description << optPreprocMap[preprocOptIndex[i]].value << endl;
	}

}

/**********************************************************************************
* AUTHOR		: Balaji, M N A
* DATE			: 21 Aug 2008
* NAME			: mapOptions
* DESCRIPTION	: Do the mapping with the preprocessing option and the respective description and value.
* ARGUMENTS		: - No -
* RETURNS		: SUCCESS on successful map operation.
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description of change
*************************************************************************************/
int mapPreprocOptions(stringStructMap &optPreprocMap, stringStringMap &headerSequence)
{
	option obj;

    //Preproc fields
    obj.description = TRACEDIMENSION;
    obj.description += "                   - ";
	obj.value = headerSequence[TRACE_DIM];
	optPreprocMap["-TRACE_DIM"] =obj;

    obj.description = RESAMPLINGMETHOD;
    obj.description += "                  - ";
	obj.value = headerSequence[RESAMP_POINT_ALLOC];
	optPreprocMap["-RESAMP_POINT_ALLOC"] =obj;

    obj.description = DOTTHRESHOLD;
    obj.description += "                   - ";
	obj.value = headerSequence[DOT_SIZE_THRES];
	optPreprocMap["-DOT_SIZE_THRES"] =obj;

    obj.description = SIZETHRESHOLD;
    obj.description += "                 - ";
	obj.value = headerSequence[NORM_LN_WID_THRES];
	optPreprocMap["-NORM_LN_WID_THRES"] =obj;

    obj.description = PRESERVEASPECTRATIO;
    obj.description += "                - ";
	obj.value = headerSequence[PRESER_ASP_RATIO];
	optPreprocMap["-PRESER_ASP_RATIO"] =obj;

    obj.description = ASPECTRATIOTHRESHOLD;
    obj.description += "       - ";
	obj.value = headerSequence[ASP_RATIO_THRES];
	optPreprocMap["-ASP_RATIO_THRES"] =obj;

    obj.description = PRESERVERELATIVEYPOSITION;
    obj.description += "          - ";
	obj.value = headerSequence[PRESER_REL_Y_POS];
	optPreprocMap["-PRESER_REL_Y_POS"] =obj;

    obj.description = SMOOTHFILTERLENGTH;
    obj.description += "                       - ";
	obj.value = headerSequence[SMOOTH_WIND_SIZE];
	optPreprocMap["-SMOOTH_WIND_SIZE"] =obj;

    obj.description = PREPROCSEQUENCE;
    obj.description += "                      - ";
	obj.value = headerSequence[PREPROC_SEQ];
	optPreprocMap["-PREPROC_SEQ"] =obj;

    return SUCCESS;
}
