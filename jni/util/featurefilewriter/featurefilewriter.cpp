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
#include "featurefilewriter.h"
#include "LTKLoggerUtil.h"
#include "LTKErrors.h"
#include "LTKErrorsList.h"
#include "LTKOSUtilFactory.h"
#include "LTKShapeFeatureExtractorFactory.h"
#include "LTKException.h"
#include "LTKConfigFileReader.h"
#include "LTKStringUtil.h"
#include "LTKPreprocDefaults.h"
#include "LTKShapeFeatureMacros.h"
#include "LTKInkFileReader.h"
#include "LTKClassifierDefaults.h"
#include "LTKShapeFeature.h"

featurefilewriter::featurefilewriter():
m_OSUtilPtr(LTKOSUtilFactory::getInstance()),
m_libHandlerFE(NULL)
{

}
featurefilewriter::~featurefilewriter()
{

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::~featurefilewriter()" << endl;

    int returnStatus = SUCCESS;

    returnStatus = deletePreprocessor();
    if(returnStatus != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " << returnStatus << " " <<
            " featurefilewriter::~featurefilewriter()" << endl;
        throw LTKException(returnStatus);
    }

    //Unloading the feature Extractor instance
    returnStatus = deleteFeatureExtractorInstance();
    if(returnStatus != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " <<  returnStatus << " " <<
            " featurefilewriter::~featurefilewriter()" << endl;
        throw LTKException(returnStatus);
    }

	if(NULL != m_OSUtilPtr)
	{
		delete m_OSUtilPtr;
		m_OSUtilPtr = NULL;
	}
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::~featurefilewriter()" << endl;
}

/**********************************************************************************
 * AUTHOR		: Saravanan R.
 * DATE			: 23-Jan-2007
 * NAME			: featurefilewriter
 * DESCRIPTION	: Default Constructor that initializes all data members
 * ARGUMENTS	: none
 * RETURNS		: none
 * NOTES		:
 * CHANGE HISTROY
 * Author			Date				Description
 *************************************************************************************/

void featurefilewriter::assignDefaultValues()
{
    m_nnCfgFilePath = "";
    m_FDTFilePath = "";
    m_ptrPreproc = NULL;
    m_preProcSeqn=NN_DEF_PREPROC_SEQ;
    m_ptrFeatureExtractor=NULL;
    m_featureExtractorName=NN_DEF_FEATURE_EXTRACTOR;
    m_deleteLTKLipiPreProcessor=NULL;
}

int featurefilewriter::Initalize(const string& cfgFilepath,
								 const string& strLipiPath,
								 const string& strOutputPath)
{
	try
	{
		LTKControlInfo tmpControlInfo;
		tmpControlInfo.lipiRoot = strLipiPath;
		tmpControlInfo.toolkitVersion = SUPPORTED_MIN_VERSION;
		tmpControlInfo.cfgFilePath = cfgFilepath;
		
	    if( (tmpControlInfo.lipiRoot).empty() )
	    {
	        throw LTKException(ELIPI_ROOT_PATH_NOT_SET);
	    }
	    if ( tmpControlInfo.toolkitVersion.empty() )
	    {
	        throw LTKException(ENO_TOOLKIT_VERSION);
	    }
	    if ( tmpControlInfo.cfgFilePath.empty() )
	    {
	        throw LTKException(EINVALID_CFG_FILE_ENTRY);
	    }

	    assignDefaultValues();

	    m_lipiRootPath = tmpControlInfo.lipiRoot;
	    m_nnCfgFilePath = cfgFilepath;
		m_FDTFilePath = strOutputPath;

	    int errorCode = initializePreprocessor(tmpControlInfo,&m_ptrPreproc);

	    if( errorCode != SUCCESS)
	    {
	        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " << errorCode << " " <<
	            "featurefilewriter::Initalize()" <<endl;
	        throw LTKException(errorCode);
	    }


		//Reading configuration file
	    errorCode = readClassifierConfig();

	    if( errorCode != SUCCESS)
	    {
			cout<<endl<<"Encountered error in readClassifierConfig"<<endl;
	        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " << 
				errorCode << " " <<
	            "featurefilewriter::Initalize()" <<endl;
	        throw LTKException(errorCode);
	    }

	    errorCode = initializeFeatureExtractorInstance(tmpControlInfo);

	    if( errorCode != SUCCESS)
	    {
	        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: " << errorCode << " " <<
	            "featurefilewriter::Initalize()" <<endl;
	        throw LTKException(errorCode);
	    }

	}
	catch(LTKException e)
	{
		deletePreprocessor();

		//Unloading the feature Extractor instance
	    deleteFeatureExtractorInstance();

		if(NULL != m_OSUtilPtr)
		{
			delete m_OSUtilPtr;
			m_OSUtilPtr = NULL;
		}
		throw e;
	}

	return 0;
}

/******************************************************************************
* AUTHOR		: Saravanan
* DATE			: 22-02-2007
* NAME			: initializeFeatureExtractorInstance
* DESCRIPTION	: This method get the Instance of the Feature Extractor
*				  from LTKShapeFeatureExtractorFactory
* ARGUMENTS		:
* RETURNS		: none
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description
******************************************************************************/
int featurefilewriter::initializeFeatureExtractorInstance(const LTKControlInfo& controlInfo)
{
	LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
		"featurefilewriter::initializeFeatureExtractorInstance()" << endl;

	LTKShapeFeatureExtractorFactory factory;
	int errorCode = factory.createFeatureExtractor(m_featureExtractorName,
			m_lipiRootPath,
			&m_libHandlerFE,
			controlInfo,
			&m_ptrFeatureExtractor);

	if(errorCode != SUCCESS)
	{
		LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<< EFTR_EXTR_NOT_EXIST << " " <<
			" featurefilewriter::initializeFeatureExtractorInstance()" << endl;
		LTKReturnError(errorCode);
	}
	LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
		"featurefilewriter::initializeFeatureExtractorInstance()" << endl;

	return SUCCESS;
}

/******************************************************************************
* AUTHOR		: Saravanan
* DATE			: 26-03-2007
* NAME			: deleteFeatureExtractorInstance
* DESCRIPTION	: This method unloads the Feature extractor instance
* ARGUMENTS		: none
* RETURNS		: none
* NOTES			:
* CHANGE HISTROY
* Author			Date				Description
******************************************************************************/
int featurefilewriter::deleteFeatureExtractorInstance()
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::deleteFeatureExtractorInstance()" << endl;

    if (m_ptrFeatureExtractor != NULL)
    {
        typedef int (*FN_PTR_DELETE_SHAPE_FEATURE_EXTRACTOR)(LTKShapeFeatureExtractor *obj);
        FN_PTR_DELETE_SHAPE_FEATURE_EXTRACTOR deleteFeatureExtractor;
        void * functionHandle = NULL;

        // Map createpreprocessor and deletePreprocessor functions
        int returnVal = m_OSUtilPtr->getFunctionAddress(m_libHandlerFE,
                                                DELETE_SHAPE_FEATURE_EXTRACTOR,
                                                &functionHandle);
        // Could not map the createLipiPreprocessor function from the DLL
    	if(returnVal != SUCCESS)
    	{
    	    LOG(LTKLogger::LTK_LOGLEVEL_ERR) <<
                "Error: "<< EDLL_FUNC_ADDRESS_DELETE_FEATEXT << " " <<
                getErrorMessage(EDLL_FUNC_ADDRESS_DELETE_FEATEXT) <<
                " featurefilewriter::deleteFeatureExtractorInstance()" << endl;

            LTKReturnError(EDLL_FUNC_ADDRESS_DELETE_FEATEXT);
    	}

        deleteFeatureExtractor = (FN_PTR_DELETE_SHAPE_FEATURE_EXTRACTOR)functionHandle;

        deleteFeatureExtractor(m_ptrFeatureExtractor);

        m_ptrFeatureExtractor = NULL;

        // unload feature extractor dll
        if(m_libHandlerFE != NULL)
    	{
            //Unload the DLL
    		m_OSUtilPtr->unloadSharedLib(m_libHandlerFE);
    		m_libHandlerFE = NULL;

    	}
    }

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::deleteFeatureExtractorInstance()" << endl;

    return SUCCESS;
}

/***********************************************************************************
 * AUTHOR		: Saravanan. R
 * DATE			: 19-01-2007
 * NAME			: initializePreprocessor
 * DESCRIPTION	: This method is used to initialize the PreProcessor
 * ARGUMENTS		: preprocDLLPath : string : Holds the Path of the Preprocessor DLL,
 *				  returnStatus    : int    : Holds SUCCESS or Error Values, if occurs
 * RETURNS		: preprocessor instance
 * NOTES			:
 * CHANGE HISTROY
 * Author			Date				Description
 *************************************************************************************/
int featurefilewriter::initializePreprocessor(const LTKControlInfo& controlInfo,
        LTKPreprocessorInterface** preprocInstance)
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::initializePreprocessor()" << endl;

    FN_PTR_CREATELTKLIPIPREPROCESSOR createLTKLipiPreProcessor = NULL;
    int errorCode;

    // Load the DLL with path=preprocDLLPath
    void* functionHandle = NULL;

    int returnVal = m_OSUtilPtr->loadSharedLib(controlInfo.lipiRoot, PREPROC, &m_libHandler);

    
	if(returnVal != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<<  ELOAD_PREPROC_DLL << " " <<
            getErrorMessage(ELOAD_PREPROC_DLL) <<
            " featurefilewriter::initializePreprocessor()" << endl;
        LTKReturnError(ELOAD_PREPROC_DLL);
    }

    // Map createpreprocessor and deletePreprocessor functions
    returnVal = m_OSUtilPtr->getFunctionAddress(m_libHandler,
                                            CREATEPREPROCINST,
                                            &functionHandle);
    // Could not map the createLipiPreprocessor function from the DLL
	if(returnVal != SUCCESS)
    {
        //Unload the dll
        unloadPreprocessorDLL();
        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<< EDLL_FUNC_ADDRESS_CREATE << " " <<
            getErrorMessage(EDLL_FUNC_ADDRESS_CREATE) <<
            " featurefilewriter::initializePreprocessor()" << endl;

        LTKReturnError(EDLL_FUNC_ADDRESS_CREATE);
    }

    createLTKLipiPreProcessor = (FN_PTR_CREATELTKLIPIPREPROCESSOR)functionHandle;

    functionHandle = NULL;

    // Map createpreprocessor and deletePreprocessor functions
    returnVal = m_OSUtilPtr->getFunctionAddress(m_libHandler,
                                            DESTROYPREPROCINST,
                                            &functionHandle);
    // Could not map the createLipiPreprocessor function from the DLL
	if(returnVal != SUCCESS)
    {
        //Unload the dll
        unloadPreprocessorDLL();

        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<< EDLL_FUNC_ADDRESS_CREATE << " " <<
            getErrorMessage(EDLL_FUNC_ADDRESS_CREATE) <<
            " featurefilewriter::initializePreprocessor()" << endl;
        LTKReturnError(EDLL_FUNC_ADDRESS_CREATE);
	}
    
    m_deleteLTKLipiPreProcessor = (FN_PTR_DELETELTKLIPIPREPROCESSOR)functionHandle;
    
    // Create preprocessor instance
    errorCode = createLTKLipiPreProcessor(controlInfo, preprocInstance);

    if(errorCode!=SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<<  errorCode << " " <<
            " featurefilewriter::initializePreprocessor()" << endl;
        LTKReturnError(errorCode);
    }

    // Could not create a LTKLipiPreProcessor
    if(*preprocInstance == NULL)
    {
        // Unload the DLL
        unloadPreprocessorDLL();

        LOG(LTKLogger::LTK_LOGLEVEL_ERR) << "Error: "<< ECREATE_PREPROC << " " <<
            getErrorMessage(ECREATE_PREPROC) <<
            " featurefilewriter::initializePreprocessor()" << endl;
        LTKReturnError(ECREATE_PREPROC);
    }

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::initializePreprocessor()" << endl;

    return SUCCESS;

}

/**********************************************************************************
 * AUTHOR		: Saravanan. R
 * DATE			: 25-01-2007
 * NAME			: deletePreprocessor
 * DESCRIPTION	: This method is used to deletes the PreProcessor instance
 * ARGUMENTS		: ptrPreprocInstance : Holds the pointer to the LTKPreprocessorInterface
 * RETURNS		: none
 * NOTES			:
 * CHANGE HISTROY
 * Author			Date				Description
 *************************************************************************************/
int featurefilewriter::deletePreprocessor()
{

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::deletePreprocessor()" << endl;

    //deleting the preprocessor instance
    if(m_ptrPreproc != NULL)
    {
        m_deleteLTKLipiPreProcessor(m_ptrPreproc);
        m_ptrPreproc = NULL;
    }

    //Unload the dll
    int returnStatus = unloadPreprocessorDLL();
    if(returnStatus != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Error: " <<
            getErrorMessage(returnStatus) <<
            " featurefilewriter::deletePreprocessor()" << endl;
        LTKReturnError(returnStatus);
    }

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::deletePreprocessor()" << endl;

    return SUCCESS;
}

/**************************************************************************
 * AUTHOR		: Nidhi Sharma
 * DATE			: 29-01-2007
 * NAME			: unloadPreprocessorDLL
 * DESCRIPTION	: This method is used to Unloads the preprocessor DLL.
 * ARGUMENTS		: none
 * RETURNS		: none
 * NOTES			:
 * CHANGE HISTROY
 * Author			Date				Description
 ****************************************************************************/
int featurefilewriter::unloadPreprocessorDLL()
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::unloadPreprocessorDLL()" << endl;


    //Check the preprocessor DLL was loaded already
    if(m_libHandler != NULL)
    {
        //Unload the DLL
		m_OSUtilPtr->unloadSharedLib(m_libHandler);
        m_libHandler = NULL;
    }

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::unloadPreprocessorDLL()" << endl;

    return SUCCESS;
}

/**********************************************************************************
 * AUTHOR		: Saravanan R.
 * DATE			: 23-Jan-2007
 * NAME			: readClassifierConfig
 * DESCRIPTION	: Reads the NN.cfg and initializes the data members of the class
 * ARGUMENTS	: none
 * RETURNS		: SUCCESS   - If config file read successfully
 *				  errorCode - If failure
 * NOTES			:
 * CHANGE HISTROY
 * Author			Date				Description
 *************************************************************************************/
int featurefilewriter::readClassifierConfig()
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::readClassifierConfig()" << endl;
    string tempStringVar = "";
    LTKConfigFileReader *shapeRecognizerProperties = NULL;
    int errorCode = FAILURE;

    try
    {
        shapeRecognizerProperties = new LTKConfigFileReader(m_nnCfgFilePath);
    }
    catch(LTKException e)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_INFO)<< "Info: " <<
            "Config file not found, using default values of the parameters" <<
            "featurefilewriter::readClassifierConfig()"<<endl;

        delete shapeRecognizerProperties;

		return FAILURE;
    }

    //Pre-processing sequence
    errorCode = shapeRecognizerProperties->getConfigValue(PREPROCSEQUENCE, m_preProcSeqn);

    if(errorCode != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_INFO) << "Info: " <<
            "Using default value of prerocessing sequence: "<< m_preProcSeqn <<
            " featurefilewriter::readClassifierConfig()"<<endl;

        m_preProcSeqn = NN_DEF_PREPROC_SEQ;
    }


    if((errorCode = mapPreprocFunctions()) != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<" Error: " << errorCode <<
            " featurefilewriter::readClassifierConfig()"<<endl;

        delete shapeRecognizerProperties;

        LTKReturnError(errorCode);
    }

    tempStringVar = "";
    errorCode = shapeRecognizerProperties->getConfigValue(FEATUREEXTRACTOR,
                                                          tempStringVar);
    if(errorCode == SUCCESS)
    {
        m_featureExtractorName = tempStringVar;
        LOG(LTKLogger::LTK_LOGLEVEL_DEBUG)<<
            FEATUREEXTRACTOR << " = "<<tempStringVar<<endl;
    }
    else
    {
        LOG(LTKLogger::LTK_LOGLEVEL_DEBUG)<<
            "Using default value for " << FEATUREEXTRACTOR << " : " <<
            m_featureExtractorName << endl;
    }

    delete shapeRecognizerProperties;

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::readClassifierConfig()" << endl;

    return SUCCESS;
}
/**********************************************************************************
 * AUTHOR		: Saravanan R
 * DATE          		: 23-Jan-2007
 * NAME          		: mapPreprocFunctions
 * DESCRIPTION   	: Maps the module name and its function names in the preprocessing
 sequence.
 * ARGUMENTS     	: none
 * RETURNS       	: SUCCESS on successful,
 *				  errorNumbers on Failure.
 * NOTES         	:
 * CHANGE HISTROY
 * Author            Date                Description
 *************************************************************************************/
int featurefilewriter::mapPreprocFunctions()
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::mapPreprocFunctions()" << endl;

    stringStringMap preProcSequence;

    stringStringPair tmpPair;

    stringVector moduleFuncNames;
    stringVector modFuncs;
    stringVector funcNameTokens;

    string module = "", funName = "", sequence = "";
    string::size_type indx;

    LTKTraceGroup local_inTraceGroup;

    LTKStringUtil::tokenizeString(m_preProcSeqn,  DELEMITER_SEQUENCE,  funcNameTokens);

    int numFunctions = funcNameTokens.size();

    if(numFunctions == 0)
    {
        LOG( LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<<
            "Wrong preprocessor sequence in cfg file : " + m_preProcSeqn <<
            " featurefilewriter::mapPreprocFunctions()"<<endl;

        LTKReturnError(EINVALID_PREPROC_SEQUENCE);
    }

    for (indx = 0; indx < numFunctions ; indx++)
    {
        moduleFuncNames.push_back(funcNameTokens[indx]);
    }

    int numModuleFunctions = moduleFuncNames.size();

    for(indx=0; indx < numModuleFunctions ; indx++)
    {
        sequence = moduleFuncNames[indx];

        LTKStringUtil::tokenizeString(sequence,  DELEMITER_FUNC,  modFuncs);

        if(modFuncs.size() >= 2)
        {
            module = modFuncs.at(0);

            funName =  modFuncs.at(1);

            if(!module.compare("CommonPreProc"))
            {
                FN_PTR_PREPROCESSOR pPreprocFunc = NULL;
                pPreprocFunc = m_ptrPreproc->getPreprocptr(funName);
                if(pPreprocFunc!= NULL)
                {
                    tmpPair.first = module;
                    tmpPair.second = funName;
                    m_preprocSequence.push_back(tmpPair);
                }
                else
                {
                    LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< EINVALID_PREPROC_SEQUENCE << " " <<
                        "Wrong preprocessor sequence entry in cfg file : " <<funName<<
                        " featurefilewriter::mapPreprocFunctions()"<<endl;
                    LTKReturnError(EINVALID_PREPROC_SEQUENCE);
                }
            }
            else
            {
                LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< EINVALID_PREPROC_SEQUENCE << " " <<
                    "Wrong preprocessor sequence entry in cfg file  : " << module<<
                    " featurefilewriter::mapPreprocFunctions()"<<endl;
                LTKReturnError(EINVALID_PREPROC_SEQUENCE);
            }
        }
        else
        {
            LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< EINVALID_PREPROC_SEQUENCE << " " <<
                "Wrong preprocessor sequence entry in cfg file  : "<<module<<
                " featurefilewriter::mapPreprocFunctions()"<<endl;
            LTKReturnError(EINVALID_PREPROC_SEQUENCE);
        }
    }

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::mapPreprocFunctions()" << endl;

    return SUCCESS;
}

/******************************************************************************
 * AUTHOR		: Saravanan
 * DATE			: 22-02-2007
 * NAME			: createFeaturefile
 * DESCRIPTION	: This method will do the extract feature by giving the List
 *				  file as input
 * ARGUMENTS	:
 * RETURNS		: none
 * NOTES		:
 * CHANGE HISTROY
 * Author			Date				Description
 ******************************************************************************/
int featurefilewriter::createFeaturefile(const string& listFilePath)
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::createFeaturefile()" << endl;

    //Ink File Path
    string path = "";

    //Line from the list file
    string line = "";

    //Line is split into tokens
    stringVector tokens;

    //ID for each shapes
    int shapeId = -1;

    ofstream featurefileFileHandle;
    ifstream listFileHandle;

    vector<LTKShapeFeaturePtr> shapeFeature;

    //Opening the train list file for reading mode
    listFileHandle.open(listFilePath.c_str(), ios::in);

    //Throw an error if unable to open the training list file
    if(!listFileHandle)
    {
		return -1;
	}

    //Open the Model data file for writing mode
    featurefileFileHandle.open(m_FDTFilePath.c_str(), ios::out);

    //Throw an error if unable to open the Model data file
    if(!featurefileFileHandle)
    {
        listFileHandle.close();
    }
	featurefileFileHandle << "<FE_NAME=" << m_featureExtractorName << ">" << endl;

    int errorCode = SUCCESS;
    while(!listFileHandle.eof())
	{
		//Get the line from the list file
		getline(listFileHandle, line, NEW_LINE_DELIMITER);

		path  = "";

		//Skip commented line
		if ( line[0] == COMMENTCHAR )
		{
			continue;
		}

		//Tokenize the string
		errorCode = LTKStringUtil::tokenizeString(line,  LIST_FILE_DELIMITER,  tokens);

		if( errorCode != SUCCESS )
		{
			listFileHandle.close();
			featurefileFileHandle.close();

			LTKReturnError(errorCode);
		}

		//Tokens must be of size 2, one is pathname and other is shapeId
		if( tokens.size() != 2 )
			continue;

		//Tokens[0] indicates the path name
		path = tokens[0];

		//Tokens[1] indicates the shapeId
		shapeId = atoi( tokens[1].c_str() );

		if(shapeId < 0 )
			break;

		// Sample from the same class, extract features, and write the extracted features to output fiel
		if(! path.empty())
		{
			if( getShapeFeatureFromInkFile(path, shapeFeature) != SUCCESS )
				continue;

			LTKShapeSample shapeSampleFeatures;

			shapeSampleFeatures.setFeatureVector(shapeFeature);
			shapeSampleFeatures.setClassID(shapeId);

			//Writing results to the feature file
			errorCode = writeFeatureFile(shapeSampleFeatures, featurefileFileHandle);
			if( errorCode != SUCCESS )
			{
				listFileHandle.close();
				featurefileFileHandle.close();
				LTKReturnError(errorCode);
			}

			shapeFeature.clear();

		}

	}//End of while

    //Closing the Train List file and Model Data file
    listFileHandle.close();
    featurefileFileHandle.close();

    return SUCCESS;
}
/**********************************************************************************
 * AUTHOR		: Saravanan R.
 * DATE			: 23-Jan-2007
 * NAME			: preprocess
 * DESCRIPTION	: calls the required pre-processing functions from the LTKPreprocessor library
 * ARGUMENTS		: inTraceGroup - reference to the input trace group
 *				  outPreprocessedTraceGroup - pre-processed inTraceGroup
 * RETURNS		: SUCCESS on successful pre-processing operation
 * NOTES			:
 * CHANGE HISTROY
 * Author			Date				Description
 *************************************************************************************/
int featurefilewriter::preprocess (const LTKTraceGroup& inTraceGroup,
        LTKTraceGroup& outPreprocessedTraceGroup)
{
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::preprocess()" << endl;

    int indx = 0;
	int errorCode = -1;

    string module = "";
    string funName = "" ;

    LTKTraceGroup local_inTraceGroup;

    local_inTraceGroup = inTraceGroup;

    if(m_preprocSequence.size() != 0)
    {
        while(indx < m_preprocSequence.size())
        {
            module = m_preprocSequence.at(indx).first;
            funName =  m_preprocSequence.at(indx).second;

            FN_PTR_PREPROCESSOR pPreprocFunc = NULL;
            pPreprocFunc = m_ptrPreproc->getPreprocptr(funName);

            if(pPreprocFunc!= NULL)
            {
                outPreprocessedTraceGroup.emptyAllTraces();


                if((errorCode = (m_ptrPreproc->*(pPreprocFunc))
                            (local_inTraceGroup,outPreprocessedTraceGroup)) != SUCCESS)
                {
                    LOG(LTKLogger::LTK_LOGLEVEL_ERR) <<"Error: "<<  errorCode << " " <<
                        " featurefilewriter::preprocess()" << endl;
                    LTKReturnError(errorCode);
                }

                local_inTraceGroup = outPreprocessedTraceGroup;
            }
            indx++;
        }
    }
    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG)<<"Exiting featurefilewriter::preprocess()"<<endl;
    return SUCCESS;
}
/******************************************************************************
 * AUTHOR		: Nidhi sharma
 * DATE			: 22-02-2007
 * NAME			: getShapeSampleFromInkFile
 * DESCRIPTION	: This method will get the ShapeSample by giving the ink
 *				  file path as input
 * ARGUMENTS	:
 * RETURNS		: SUCCESS
 * NOTES		:
 * CHANGE HISTROY
 * Author			Date				Description
 ******************************************************************************/
int featurefilewriter::getShapeFeatureFromInkFile(const string& inkFilePath,
        vector<LTKShapeFeaturePtr>& shapeFeatureVec)
{

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Entering " <<
        "featurefilewriter::getShapeFeatureFromInkFile()" << endl;

    if ( inkFilePath.empty() )
        return FAILURE;

    LTKCaptureDevice captureDevice;
    LTKScreenContext screenContext;

    LTKTraceGroup inTraceGroup, preprocessedTraceGroup;
    inTraceGroup.emptyAllTraces();

    int returnVal = m_shapeRecUtil.readInkFromFile(inkFilePath,
            m_lipiRootPath, inTraceGroup,
            captureDevice, screenContext);

    if (returnVal!= SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<<returnVal<<
            " featurefilewriter::getShapeFeatureFromInkFile()" << endl;
        LTKReturnError(returnVal);
    }

    m_ptrPreproc->setCaptureDevice(captureDevice);
    m_ptrPreproc->setScreenContext(screenContext);

    preprocessedTraceGroup.emptyAllTraces();

    //Preprocessing to be done for the trace group that was read
	int errorCode = preprocess(inTraceGroup, preprocessedTraceGroup);
    if(  errorCode != SUCCESS )
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< errorCode << " " <<
            " featurefilewriter::getShapeFeatureFromInkFile()" << endl;
		LTKReturnError(errorCode);
    }

    errorCode = m_ptrFeatureExtractor->extractFeatures(preprocessedTraceGroup,
            shapeFeatureVec);

    if (errorCode != SUCCESS)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< errorCode << " " <<
            " featurefilewriter::getShapeFeatureFromInkFile()" << endl;
        LTKReturnError(errorCode);
    }

    LOG(LTKLogger::LTK_LOGLEVEL_DEBUG) << "Exiting " <<
        "featurefilewriter::getShapeFeatureFromInkFile()" << endl;

    return SUCCESS;
}

/**********************************************************************************
 * AUTHOR		: Saravanan R.
 * DATE			: 23-Jan-2007
 * NAME			: writeFeatureFile
 * DESCRIPTION	:
 * ARGUMENTS	:
 * RETURNS		: none
 * NOTES		:
 * CHANGE HISTROY
 * Author			Date				Description
 *************************************************************************************/
int featurefilewriter::writeFeatureFile(const LTKShapeSample& prototypeVec,
        ofstream & featurefileFileHandle)
{
    string strFeature = "";

    if(!featurefileFileHandle)
    {
        LOG(LTKLogger::LTK_LOGLEVEL_ERR)<<"Error: "<< EINVALID_FILE_HANDLE << " " <<
            "Invalid file handle for feature file"<<
            " featurefilewriter::writeFeatureFile()" << endl;
        LTKReturnError(EINVALID_FILE_HANDLE);
    }


    //Write the class Id
    int classId = prototypeVec.getClassID();
	featurefileFileHandle << classId << " ";

    const vector<LTKShapeFeaturePtr>& shapeFeatureVector = prototypeVec.getFeatureVector();

    vector<LTKShapeFeaturePtr>::const_iterator shapeFeatureIter = shapeFeatureVector.begin();
    vector<LTKShapeFeaturePtr>::const_iterator shapeFeatureIterEnd = shapeFeatureVector.end();

    for(; shapeFeatureIter != shapeFeatureIterEnd; ++shapeFeatureIter)
    {
        (*shapeFeatureIter)->toString(strFeature);
        featurefileFileHandle << strFeature << FEATURE_EXTRACTOR_DELIMITER;
    }

    featurefileFileHandle << "\n";

    return SUCCESS;
}
