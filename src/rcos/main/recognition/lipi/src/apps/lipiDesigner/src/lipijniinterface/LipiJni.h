#if !defined(__W32__)
#define LIPIJNIINTERFACE_API
//#define HMODULE void*
#else
#ifdef LIPIJNIINTERFACE_EXPORTS
#define LIPIJNIINTERFACE_API __declspec(dllexport)
#else
#define LIPIJNIINTERFACE_API __declspec(dllimport)
#endif
#endif


#include <jni.h>
#include <sys/stat.h>
#include "LipiIdeMacros.h"
#include "LTKLipiEngineInterface.h"
#include "LTKMacros.h"
#include "LTKInc.h"
#include "LTKTypes.h"
#include "LTKTrace.h"
#include "LTKTraceGroup.h"
#include "LTKScreenContext.h"
#include "ListFileManager.h"
#include "LTKChannel.h"
#include "LTKLoggerInterface.h"
using namespace std;

// Function pointer declarations for exported functions from algorithm DLL...
typedef LTKLipiEngineInterface* (*FN_PTR_LTKLIPIENGINE) (void);
FN_PTR_LTKLIPIENGINE fptrLTKLipiEngine = NULL;

typedef int (*FN_PTR_LIPIGETCURRENTVER) (void);
FN_PTR_LIPIGETCURRENTVER fptrLipiGetCurrentVer = NULL;

typedef int (*FN_PTR_LIPIRECOGNIZE)(LTKTraceGroup& traceGroup, 
					  LTKScreenContext& uiParams, 
					  vector<bool> &subSetOfClasses, 
					  float confThreshold, 
      				  int  numChoices, 
					  vector<LTKShapeRecoResult>& resultVec);

FN_PTR_LIPIRECOGNIZE fptrRecognize = NULL;

typedef int (*FN_PTR_LIPISETDEVICEINFO)(LTKCaptureDevice &CaptureDevice);
FN_PTR_LIPISETDEVICEINFO fptrLipiSetDeviceInfo = NULL;


LIPIJNIINTERFACE_API int LoadLipiEngineModelData();
LIPIJNIINTERFACE_API int LoadLipiInterface();
LIPIJNIINTERFACE_API int UnloadModelData();


JNIEnv *envlocal;
jobjectArray ResultSetArray;
jclass CoordClass;
jint ArrayStrokeSize;
jint numstrokes;
jobjectArray CoordArray;
jmethodID  getgetX_id;
jmethodID  getgetY_id;
LTKShapeRecognizer *pShapeReco =  NULL;
LTKLipiEngineInterface *ptrObj = NULL;
void *hLipiModule = NULL;
string envstring = "";
const char* projName;
ListFileManager lfm;

int CopyTraceGroup(LTKTraceGroup &oTraceGroup);
int CopyScreenContext(LTKScreenContext oScreenContext);
int CreateShapeRecognizer();
int LoadModelData();
void Train();
int UnloadLipiEngine();
int GetLipiEnginePath(char *lipiEnginePath);
int InitializeLipiEngine(char *algoDllName);
int GetAllFunctions();
int UnloadLipiInterface();




