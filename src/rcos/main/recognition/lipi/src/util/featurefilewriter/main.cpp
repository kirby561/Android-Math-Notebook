/*****************************************************************************************
* Copyright (c) 2006 Hewlett-Packard Development Company, L.P.
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
 * $LastChangedDate: 2008-07-10 15:23:21 +0530 (Thu, 10 Jul 2008) $
 * $Revision: 556 $
 * $Author: sharmnid $
 *
 ************************************************************************/

/************************************************************************
 * FILE DESCR: 
 * CONTENTS:   
 * AUTHOR:     Balaji MNA
 * DATE:       22-November-2008
 * CHANGE HISTORY:
 * Author		Date			Description
 ************************************************************************/
#pragma  once
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "featurefilewriter.h"

void displayHelp();
/**********************************************************************************
* AUTHOR		: Balaji MNA
* DATE			: 21-NOV-2008
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
{
	try
	{
		if(argc==1)
		{
			displayHelp();
			exit(1);
		}

		string cfgPath;
		string listfile;
		string lipiroot;
		string outputDir;
		string projectName;
		for(int i=1;i<argc;i++)
		{
			
			string arg(argv[i]);
			if(arg=="-help")
			{
				displayHelp();
				exit(1);
			}
			else if(arg=="-cfg")
			{
				i++;
				string value;
				if(i!=argc)
				    value = argv[i];

				if(value.empty())
				{
					cout << "Invalid or no value specified for -cfg option." << endl;
					return -1;
				}

				cfgPath = value;
				
			}
			else if(arg=="-list")
			{
				i++;
				string value;
				if(i!=argc)
				    value = argv[i];

				if(value.empty())
				{
					cout << "Invalid or no value specified for -list option." << endl;
					return -1;
				}

				listfile = value;
			}
			else if(arg=="-lipiroot")
			{
				i++;
				string value;
				if(i!=argc)
				    value = argv[i];

				if(value.empty())
				{
					cout << "Invalid or no value specified for -lipiroot option." << endl;
				}

				lipiroot = value;
			}
			else if(arg=="-output")
			{
				i++;
				string value;
				if(i!=argc)
				    value = argv[i];

				if(value.empty())
				{
					cout << "Invalid or no value specified for -output option." << endl;
					return -1;
				}

				outputDir = value;
			}
			else if ((arg == "-ver")||(arg == "-v"))
			{
				cout << "\n version of featurefilewriter tool: " << CURRENT_VERSION << endl;
				exit(0);
			}

		}

		if(lipiroot.empty())
		{
			char *lipipath = getenv(LIPIROOT_ENV_STRING);

			if(NULL == lipipath)
			{
				cout << "Error, Environment variable is not set LIPI_ROOT" << endl;
				exit(0);
			}
			lipiroot = lipipath;

		}

		if(!cfgPath.empty() &&  
		   !listfile.empty() &&
		   !outputDir.empty())
		{
			featurefilewriter ffwriter;
			ffwriter.Initalize(cfgPath, lipiroot, outputDir);
			ffwriter.createFeaturefile(listfile);
		}
		else
		{
			cout << "Options are missing" << endl;
		}
	}
	catch(...)
	{
	
	}
	return 0;
}

void displayHelp()
{
	printf("\n\nUsage : featurefilewriter\n");
	printf("\nfeaturefilewriter\n");
	printf("\n	-cfg <cfg file path>\n");
	printf("\n	-list <list filename>\n");
	printf("\n	-output <output filename>\n");
	printf("\n	[-lipiroot <root name of the lipitk>]\n");
	printf("\n	[-loglevel <DEBUG|ERR|ALL|OFF|INFO>]\n");
	printf("\n	[-ver]\n\n");
}

