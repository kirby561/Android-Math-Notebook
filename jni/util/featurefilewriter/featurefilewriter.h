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
 * $Revision:  $
 * $Author:  $
 *
 ************************************************************************/

/************************************************************************
 * FILE DESCR:
 * CONTENTS:
 * AUTHOR:     Balaji MNA
 * DATE:       22-November-2008
 * CHANGE HISTORY:
 * Author      Date           Description
 ************************************************************************/
#ifndef __FEATUREFILEWRITER_H__
#define __FEATUREFILEWRITER_H__

#include "LTKTypes.h"
#include "LTKMacros.h"
#include "LTKOSUtil.h"
#include "LTKShapeFeatureExtractor.h"
#include "LTKShapeRecoResult.h"
#include "LTKCaptureDevice.h"
#include "LTKScreenContext.h"
#include "LTKPreprocessorInterface.h"
#include "LTKShapeRecoUtil.h"
#include "LTKShapeSample.h"

typedef int (*FN_PTR_CREATELTKLIPIPREPROCESSOR)(const LTKControlInfo& , LTKPreprocessorInterface** );
typedef int (*FN_PTR_DELETELTKLIPIPREPROCESSOR)(LTKPreprocessorInterface* );

#define SUPPORTED_MIN_VERSION "3.0.0"
#define CURRENT_VERSION "4.0.0"

class featurefilewriter
{
public:
     featurefilewriter();
     ~featurefilewriter();

     int Initalize(const string& cfgFilepath, const string& strLipiPath ,
                      const string& strOutputPath);
     int createFeaturefile(const string& listFilePath);
private:

    FN_PTR_DELETELTKLIPIPREPROCESSOR m_deleteLTKLipiPreProcessor;

    void *m_libHandler;

     string m_currentVersion;
    string m_nnCfgFilePath;
    string m_FDTFilePath;
    string m_preProcSeqn;
    string m_lipiRootPath;
     string m_featureExtractorName;
     vector<stringStringPair> m_preprocSequence;

     LTKPreprocessorInterface *m_ptrPreproc;
    LTKShapeFeatureExtractor *m_ptrFeatureExtractor;
    LTKShapeRecoUtil m_shapeRecUtil;
    LTKOSUtil*  m_OSUtilPtr;
    void *m_libHandlerFE;

     void assignDefaultValues();
    int deletePreprocessor();
    int unloadPreprocessorDLL();
    int readClassifierConfig();
    int mapPreprocFunctions();
     int deleteFeatureExtractorInstance();

     int initializeFeatureExtractorInstance(const LTKControlInfo& controlInfo);
    int GetShapeRecognizer(const string& strProfileCFGPath,string& strOutShapeRec);
    int initializePreprocessor(const LTKControlInfo& controlInfo,
            LTKPreprocessorInterface** preprocInstance);
     int preprocess (const LTKTraceGroup& inTraceGroup, LTKTraceGroup& outPreprocessedTraceGroup);

    int getShapeFeatureFromInkFile(const string& inkFilePath,
                                             vector<LTKShapeFeaturePtr>& shapeFeatureVec);

    int writeFeatureFile(const LTKShapeSample& prototypeVec,
                           ofstream & mdtFileHandle);

};
#endif //#ifndef __FEATUREFILEWRITER_H__