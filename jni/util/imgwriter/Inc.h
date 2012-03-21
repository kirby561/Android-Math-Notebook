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
 * FILE DESCR: The OS specific parts in the Code is abstracted
 *
 * CONTENTS:   
 *
 * AUTHOR:     Bharath A.
 *
 * DATE:       April 5, 2005
 * CHANGE HISTORY:
 * Author               Date                    Description of change
 ************************************************************************/
#ifndef __INC_H
#define __INC_H
//Windows specific dirent.h is used
#ifdef _WIN32
   #include <direct.h>
   #include "dirent.h"
#else
   #include <sys/dir.h>
   #include <sys/stat.h>
   #include "/usr/include/dirent.h"
#endif


#ifdef _WIN32
    #define makedir(X) _mkdir(X)
	#define TEMPDIR getenv("TEMP")

#else
    #define makedir(X) mkdir(X,0644)
	#define TEMPDIR "/var/tmp"
#endif

#endif// __INC_H
