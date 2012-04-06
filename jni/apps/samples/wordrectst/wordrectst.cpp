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
 * $LastChangedDate: 2011-02-08 16:57:52 +0530 (Tue, 08 Feb 2011) $
 * $Revision: 834 $
 * $Author: mnab $
 *
 ************************************************************************/
 
/************************************************************************
 * FILE DESCR: Sample test application for Word Recognition
 *
 * CONTENTS:
 *  main
 *
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

#include "wordrectst.h"
#include "LTKLoggerUtil.h"
#include "LTKErrors.h"
#include "LTKStringUtil.h"
#include "LTKWordRecoResult.h"
#include "LTKException.h"
#include "LTKOSUtilFactory.h"
#include "LTKOSUtil.h"

LTKOSUtil* utilPtr = LTKOSUtilFactory::getInstance();

int main(int argc, char** argv)
{
	char *envstring = NULL;
	int iResult;
	string tempStr(REC_UNIT_INFO), tempStr1(REC_MODE);
	string path;
	string strShapeId;
    string strWordId;
	char infilelist[MAX_PATH];
	string outfile("wordrectst.out");
	vector<LTKTraceGroup> fieldInk;
	int charIndex;
	wstring eolstr(L"\r\n");
	int i;
    

	// first argument is the logical project name 
	// second argument is the ink file to recognize
	// third argument is the output file
	if(argc < 4)
	{
		cout << "\nUsage:";
		cout << "\nwordrectst <logical projectname> <list file to recognize> <outputfile>";
		cout << "\nlist of valid <logicalname>s is available in $LIPI_ROOT/projects/lipiengine.cfg file";
		cout << endl;
        delete utilPtr;
		return -1;
	}

	// Get the LIPI_ROOT environment variable 
	envstring = getenv(LIPIROOT_ENV_STRING);
	if(envstring == NULL)
	{
		cout << "\nError, Environment variable is not set LIPI_ROOT\n";
        delete utilPtr;
		return -1;
	}

	// Load the LipiEngine.DLL
	hLipiEngine = NULL;
	iResult = utilPtr->loadSharedLib(envstring, LIPIENGINE_MODULE_STR, &hLipiEngine);

	if(iResult != SUCCESS)
	{
		cout << "Error loading LipiEngine module" << endl;
        delete utilPtr;
		return -1;
	}

	if(MapFunctions() != 0)
	{
		cout << "Error fetching exported functions of the module" << endl;
        delete utilPtr;
		return -1;
	}

	// create an instance of LipiEngine Module
	ptrObj = createLTKLipiEngine();

	// set the LIPI_ROOT path in Lipiengine module instance
	ptrObj->setLipiRootPath(envstring);

	// Initialize the LipiEngine module
	iResult = ptrObj->initializeLipiEngine();
	if(iResult != SUCCESS)
	{
		cout << iResult << ": Error initializing LipiEngine.\n";
		utilPtr->unloadSharedLib(hLipiEngine);
        delete utilPtr;

		return -1;
	}

//	Assign the logical name of the project to this string, i.e. TAMIL_WORD
	string strLogicalName = string(argv[1]);

	strcpy(infilelist,  argv[2]);
	outfile = argv[3];

	LTKWordRecognizer *pWordReco = NULL;
	ptrObj->createWordRecognizer(strLogicalName,&pWordReco);
	if(pWordReco == NULL)
	{
		cout << "\nError creating Word Recognizer\n";
	
		utilPtr->unloadSharedLib(hLipiEngine);
        delete utilPtr;
		return -1;
	}

//	You can also use project and profile name to create LipiEngine instance as follows...
//	string strProjectName = "tamil_boxed_field";
//	string strProfileName = "default";
//	LTKWordRecognizer *pWordReco = ptrObj->createWordRecognizer(&strProjectName, &strProfileName);

	int iErrorCode = 0;
	LTKRecognitionContext *recoContext = new LTKRecognitionContext();

	if(iErrorCode != 0)
	{
		cout << "\nError creating recognition context.\n";
		ptrObj->deleteWordRecognizer(pWordReco);

        utilPtr->unloadSharedLib(hLipiEngine);
        delete utilPtr;
        
		return -1;
	}

	LTKCaptureDevice deviceContext;
	LTKScreenContext screenContext;
	int numChoices = 2;

	// Setting the device attributes
	deviceContext.setSamplingRate(120);	
	deviceContext.setXDPI(2500);
	deviceContext.setYDPI(2500);
	deviceContext.setUniformSampling(true);

	// Set the engine to recognizer
	recoContext->setWordRecoEngine(pWordReco);
	
	// set the device context
	recoContext->setDeviceContext(deviceContext);
	// set the screen context
	recoContext->setScreenContext(screenContext);

	recoContext->setFlag(tempStr,REC_UNIT_CHAR);
	recoContext->setFlag(tempStr1,REC_MODE_STREAMING);

	// set the number of choices required
	recoContext->setNumResults(numChoices);

	ifstream in(infilelist);
	if(in == NULL)
	{
		cout << "Test list file open error : " << infilelist << endl;
        delete utilPtr;
		return -1;
	}

	ofstream resultfile(outfile.c_str(),ios::out|ios::binary);

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

		cout << path << endl;

		try
		{
			fieldInk.clear();
		
			//read the word file
			readWordFile(path, fieldInk, deviceContext, screenContext);
			
			for(charIndex = 0; charIndex < fieldInk.size(); ++charIndex)
			{
				recoContext->beginRecoUnit();
				recoContext->addTraceGroups(LTKTraceGroupVector(1,fieldInk.at(charIndex)));
				recoContext->endRecoUnit();

			}
		}
		catch(LTKException e)
		{
			LOG(LTKLogger::LTK_LOGLEVEL_ERR) << e.getExceptionMessage();
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

	}

	resultfile.close();

	//delete word recognition instance
	if(pWordReco)
	{
		ptrObj->deleteWordRecognizer(pWordReco);
	}

	//delete recognition context object
	if(recoContext)
	{
		//ptrObj->deleteRecognitionContext(recoContext);
		delete recoContext;
	}

	

	//unload the LipiEngine module from memory...
	utilPtr->unloadSharedLib(hLipiEngine);
    delete utilPtr;

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
	LTKInkFileReader::readUnipenInkFileWithAnnotation(fileName,"CHARACTER","ALL",traceGroup,anotInfo, dc, sc);

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
			LTKTrace trace;
			traceGroup.getTraceAt(intIds.at(loopIndex), trace);
			charTraceGroup.addTrace(trace);
		}

		cout<<"Character has "<<charTraceGroup.getNumTraces()<<" traces."<<endl;
		//push back this trace vector.
		traceGroupVec.push_back(charTraceGroup);


		intIds.clear();
	}

	return SUCCESS;
}

/**********************************************************************************
* NAME          : MapFunctions
* DESCRIPTION   : This method fetches the address of the exported function of
*				  lipiengine module
* ARGUMENTS     : 
* RETURNS       : 0 on success, -1 on Failure.
* NOTES         :
* CHANGE HISTROY
* Author            Date                Description of change
* 
*************************************************************************************/
int MapFunctions()
{
	createLTKLipiEngine = NULL;
    void* functionHandle = NULL;

	int iErrorCode = utilPtr->getFunctionAddress(hLipiEngine, 
                                             "createLTKLipiEngine", 
                                             &functionHandle);


    createLTKLipiEngine = (FN_PTR_CREATELTKLIPIENGINE)functionHandle;
    
    if(iErrorCode != SUCCESS)
	{
		cout << "Error mapping the createLTKLipiEngine function" << endl;
		return -1;
	}

	return 0;
}


