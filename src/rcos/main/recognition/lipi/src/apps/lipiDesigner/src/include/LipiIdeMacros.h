/*****************************************************************************************
* Copyright (c) 2007 Hewlett-Packard Development Company, L.P.
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
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
 * $LastChangedDate$
 * $Revision$
 * $Author$
 *
 ************************************************************************/

/************************************************************************
 * FILE DESCR: Contains common macro Definitions.
 *
 * CONTENTS:
 *
 * AUTHOR:    
 *
 * DATE:       
 * CHANGE HISTORY:
 * Author		Date			Description of change
 ************************************************************************/

#ifndef __IDEMACORS_H
#define __IDEMACORS_H

#define FLDSEPERATOR " "

#define PROJECTS "projects"

#define CONFIG "config"

#define DEFAULT "default"

#define MDT_FILE "nn.mdt"

#define ADDCLASS_ACTION "Add Class"

#define ADDSAMPLE_ACTION "Add Sample"

#define DELETECLASS_ACTION "Delete Class"

#define DELETESAMPLE_ACTION "Delete Sample"

#define TRAININGLIST_FILE "trainlist.txt"

#define NUMOFCHOICES 5

#define CONFIDENCE_THRESHOLD 0.002f

#define CREATE_LTK_LIPIENGINE "createLTKLipiEngine"

#define GET_ToolKit_VERSION "getToolkitVersion"

#ifdef _WIN32
#define DLL_EXT ".dll"
#else
#define DLL_EXT ".so"
#endif

#ifdef _WIN32
        #define LTKLoadDLL(DLLName) (void*)LoadLibrary(DLLName)
#else
        #define LTKLoadDLL(DLLName) dlopen(DLLName, RTLD_LAZY)
#endif

#ifdef _WIN32
        #define LTKGetDLLFunc(Handle, FuncName) GetProcAddress((HMODULE)Handle, FuncName)
#else
        #define LTKGetDLLFunc(Handle, FuncName) dlsym(Handle, FuncName)
#endif

#ifdef _WIN32
        #define LTKUnloadDLL(Handle) FreeLibrary((HINSTANCE)Handle)
#else
        #define LTKUnloadDLL(Handle) dlclose(Handle)
#endif

#endif // #ifndef __IDEMACORS_H

