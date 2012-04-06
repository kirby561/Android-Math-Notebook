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
 * $LastChangedDate: 2011-08-23 13:35:44 +0530 (Tue, 23 Aug 2011) $
 * $Revision: 875 $
 * $Author: jitender $
 *
 ************************************************************************/
/************************************************************************
 * FILE DESCR: Header file for RunShaperec tool
 *
 * CONTENTS:
 *  main
 *
 * AUTHOR:     Thanigai Murugan K
 *
 * DATE:       August 30, 2005
 * CHANGE HISTORY:
 * Author       Date            Description of change
 * Balaji MNA   18th Jan 2010   Receiving LTKShapeRecognizer as single pointer 
 *                              instead of double pointer in CleanUp
 ************************************************************************/
#ifndef __RUNSHAPEREC_H__
#define __RUNSHAPEREC_H__

#pragma  once
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "LTKMacros.h"
#include "LTKInc.h"
#include "LTKLoggerUtil.h"
#include "LTKInkFileReader.h"
#include "LTKShapeFeatureMacros.h"


// Class forward declarations
class LTKLipiEngineInterface;
class LTKShapeRecognizer;
class LTKPreprocessorInterface;
class LTKShapeFeatureExtractor;
class LTKOSUtil;

// List of all possible command line options
#define OPTION_ADAPT "-adapt"
#define OPTION_TRAIN "-train"
#define OPTION_TEST "-test"
#define OPTION_PERF "-perf"
#define OPTION_PROJECT "-project"
#define OPTION_PROFILE "-profile"
#define OPTION_OUTPUT "-output"
#define OPTION_NUMCHOICES "-numchoices"
#define OPTION_HEADERINFO "-h"
#define OPTION_LOGFILE "-logfile"
#define OPTION_VERSION "-ver"
#define OPTION_HELP1 "/?"
#define OPTION_HELP2 "-help"
#define OPTION_COMMENT "-comment"
#define OPTION_DATASET   "-dataset"
#define OPTION_LOGLEVEL "-loglevel"
#define OPTION_INFILETYPE "-infiletype"
#define LIPI_ROOT "-lipiroot"
#define OPTION_CONFTHRESHOLD "-confthreshold"
#define DEFAULT_PROFILE "default"
#define SAMPLE_SET "-sampleset"
#define MIN_ARGS 2

#define MAX_PROJECT_NAME 128
#define MAX_PROFILE_NAME 128

#ifndef _WIN32
     #define MAX_PATH 1024
#endif

#define SUPPORTED_MIN_VERSION   "3.0.0"
#define CURRENT_VERSION   "4.0.0"

void printUsage();
int getIntValue(int index, int* iValue);
int getFloatValue(int index, float* fValue);
int getStringValue(int index, char* strOption);
int ValidateCommandLineArgs();
int processCommandLineArgs();
int evaluateShapeRecognizer(LTKShapeRecognizer **pReco);
int startLogging(const string& logFileName, const LTKLogger::EDebugLevel debugLevel);
int CheckForOption(char* strVal);
int getAbsolutePath (string &pathName, const string lipiRootPath);
int cleanUp(LTKShapeRecognizer* pReco, LTKOSUtil* utilPtr);
int ShapeRecognizerFromFeatureFile(LTKShapeRecognizer **pReco);
int validateFeatureFile(const string& listFile, string& outFeatureExtractorName);
int loadFeatureExtractor(string featureExtractorName, LTKShapeFeatureExtractor** ptrFeatureExtractor,void **libHandlerFE);
int unloadFeatureExtractor(LTKShapeFeatureExtractor* ptrFeatureExtractor,void *libHandlerFE);
int evaluateAdapt(LTKShapeRecognizer **pReco, const string strProjectName);
int getRecognizerName(const string strProjectName,const string
                 strProfileName, string& outRecognizerString);
int evaluateAdaptActiveDTW(LTKShapeRecognizer **addReco, const string
                    strProjectName);

void fillSampleSetVec(char*);

#endif //#ifndef __RUNSHAPEREC_H__
