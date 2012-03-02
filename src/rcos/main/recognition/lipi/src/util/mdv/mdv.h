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
 * $LastChangedDate: 2011-08-23 13:35:03 +0530 (Tue, 23 Aug 2011) $
 * $Revision: 873 $
 * $Author: jitender $
 *
 ************************************************************************/

/************************************************************************
 * FILE DESCR: Declration : Macros and Functions for Model Data Viewer.
 *
 * CONTENTS:
 *
 * AUTHOR:     Vijayakumara M.
 *
 * DATE:       Aug 22, 2005
 * CHANGE HISTORY:
 * Author      Date           Description of change
 ************************************************************************/

#ifndef _MDV_H_
#define _MDV_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <time.h>
#include <list>
#include <map>

#include "LTKTypes.h"
#include "LTKMacros.h"
#include "LTKLoggerUtil.h"

#define SUPPORTED_MIN_VERSION   "3.0.0"
#define CURRENT_VERSION   "4.0.0"
#define OPTION_LOGFILE          "-logfile"
#define OPTION_LOGLEVEL         "-loglevel"
#define OPTION_LIPI_ROOT        "-lipiroot"
#define OPTION_INPUT            "-input"
#define OPTION_HELP             "-help"
#define OPTION_ALL              "-all"
#define OPTION_VER              "-ver"
#define OPTION_PREPROC          "-preproc"

#define NO_PREPROC_FIELDS       9
using namespace std;
// struction with description and its value.
struct option
{
     string description;
     string value;
};

typedef vector<string> stringVector;
typedef map<string, string> stringStringMap;
typedef map <string, option> stringStructMap;

/**
 * The main method, key method of the model data viewer.
 *
 * @param number of arguments ( Number of options users requested ).
 * @param argument text ( option names)
 *
 * @return SUCCESS on successful exection.
 */

int main(int argc, char* argv[]);

/**
 * This method displays the help notice on the standard output.
 * @return SUCCESS on successful display.
 */

int helpDisplay();


/**
 * This method maps the options with their values.
 *
 * @param string struct maping variable, which holds the option name and the
 *        values in the structure.
 * @param headerSequence, which contains the header key names and its values respectively.
 *
 * @return SUCCESS on successful map operation.
 */

int mapOptions(stringStructMap &optMap, stringStringMap &headerSequence);

/**
 * This method changes the text into upper case.
 *
 * @param string to be converted into uppercase.
 * @return the converted string into uppercase.
 */

char* stringToUpper(char *strToConvert);

int mapLogLevel(const string& logLevelStr, LTKLogger::EDebugLevel& outLogLevel);

int mapPreprocOptions(stringStructMap &optPreprocMap, stringStringMap &headerSequence);

void DisplayPreProc(stringStructMap optPreprocMap);

#endif //#ifndef _MDV_H_
