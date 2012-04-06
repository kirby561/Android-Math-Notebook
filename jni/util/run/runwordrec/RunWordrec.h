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
 * $LastChangedDate: 2011-08-23 13:36:18 +0530 (Tue, 23 Aug 2011) $
 * $Revision: 877 $
 * $Author: jitender $
 *
 ************************************************************************/

/************************************************************************
 * FILE DESCR: Header file for RunWordrec tool
 *
 * CONTENTS:
 *  main
 *
 * AUTHOR:     Thanigai Murugan K
 *
 * DATE:       August 30, 2005
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <string>
#include "LTKInkFileReader.h"

#include "LTKLipiEngineInterface.h"
#include "LTKMacros.h"
#include "LTKInc.h"
#include "LTKTypes.h"
#include "LTKTrace.h"
#include "LTKLoggerUtil.h"

// List of all possible command line options
#define OPTION_TEST "-test"
#define OPTION_PERF "-perf"
#define OPTION_PROJECT "-project"
#define OPTION_PROFILE "-profile"
#define OPTION_OUTPUT "-output"
#define OPTION_NUMCHOICES "-numchoices"
#define OPTION_LOGFILE "-logfile"
#define OPTION_VERSION "-ver"
#define OPTION_HELP1 "/?"
#define OPTION_HELP2 "-help"
#define OPTION_LOGLEVEL "-loglevel"
#define LIPI_ROOT "-lipiroot"

#define DEFAULT_PROFILE "default"

#define MIN_ARGS 2

#ifndef _WIN32
#define MAX_PATH 1024
#endif

#define MAX_PROJECT_NAME 128
#define MAX_PROFILE_NAME 128

#define SUPPORTED_MIN_VERSION   "3.0.0"
#define CURRENT_VERSION   "4.0.0"

/**
 * The function pointer declaration to get the function address of "createLTKLipiEngine"
 */

typedef LTKLipiEngineInterface* (*FN_PTR_CREATELTKLIPIENGINE) (void);
FN_PTR_CREATELTKLIPIENGINE createLTKLipiEngine;


/**
 * The function pointer declaration to get the current version.
 *
 * @param iMajor - An integer variable for Major number.
 * @param iMinor - An integer variable for Minor number.
 * @param iBugFix - An integer variable for BugFix number.
 *
 * @return 0 on successful execution.
 */

typedef int (*FN_PTR_GETCURRENTVERSION) (int *iMajor, int *iMinor, int *iBugfix);
FN_PTR_GETCURRENTVERSION LipiEngine_getCurrentVersion;

/**
 * Global pointer to the LipiEngine interface
 */

LTKLipiEngineInterface *ptrObj = NULL;

/**
 * This method to print the usage on an error
 * function as its arguments.
 *
 * @param void - no argument
 */
void printUsage();

/**
 *  To get the integer value which is passed as command line argument
 *  index specifies that the value to be fetched from the next command
 *  line argument position
 *  function as its arguments.
 *
 * @param  index - current index in the list of command line arguments
 * @param  iValue - Output value
 *
 * @return -1 on error 0 on success
 */
int getIntValue(int index, int* iValue);

/**
 *  To get the string value which is passed as command line argument
 *  index specifies that the value to be fetched from the next command
 *  line argument position
 *  function as its arguments.
 *
 * @param  index -  current index in the list of command line arguments
 * @param  strOption - Output value
 *
 * @return -1 on error 0 on success
 */
int getStringValue(int index, char* strOption);

/**
 *   Validate the command line arguments. Report an error if value is
 *   missing for any command line option
 *  function as its arguments.
 *
 * @param  void -  No arguments
 *
 * @return -1 on error 0 on success
 */
int ValidateCommandLineArgs();

/**
 *  Processes all the command line arguments. Report an error if value is
 *  missing for any command line option
 *  function as its arguments.
 *
 * @param  void -  No arguments
 *
 * @return -1 on error 0 on success
 */
int processCommandLineArgs();

/**
 *  Load the model data and call recognize function and display the
 *  results.
 *  function as its arguments.
 *
 * @param  pReco - handle to LTKWordRecognizer object
 *
 * @return -1 on error 0 on success
 */
int evaluateWordRecognizer(LTKWordRecognizer *pReco, const string& infilelist);

/**
 *  Check if the argument is matching with any of the option strings
 *  function as its arguments.
 *
 * @param  strVal - Value to match
 *
 * @return -1 on error 0 on success
 */
int CheckForOption(char* strVal);

/**
 * Global declaration
 */
void *hLipiEngine;
char strLogFile[MAX_PATH] = "";
string strLogFileName;
char strLogLevel[MAX_PATH] = "";

/**
 *  Reading annotated ink information unipen file
 *  function as its arguments.
 *
 * @param  fileName -  annotated unipen file name
 * @param  - traceGroupVec - LTK Trace Group vector
 * @param  - dc - LTK Capture device object handler
 * @param  - sc - LTK Screen context object handler
 *
 * @return  0 on success
 */
int readWordFile(string fileName, vector<LTKTraceGroup>& traceGroupVec, LTKCaptureDevice& dc, LTKScreenContext& sc);

/**
 *  This method starts the log operation by specifying the file name.
 *
 * @param  logFileName -  log file name
 * @param  debugLevel - logging debug level
 *
 * @return -1 on error 0 on success
 */
int startLogging(const string& logFileName, const LTKLogger::EDebugLevel debugLevel);



