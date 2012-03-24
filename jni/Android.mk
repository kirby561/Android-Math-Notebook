LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS = -fexceptions

		   ## Files removed from original LTK Build
		   #apps/samples/wordrectst/wordrectst.cpp \
		   #apps/samples/shaperectst/shaperectst.cpp \
		   #util/lib/LTKWindowsUtil.cpp \
		   #util/lib/LTKWinCEUtil.cpp \
		   #util/imgwriter/main.cpp \
		   #util/featurefilewriter/main.cpp \
		   #util/mdv/mdv.cpp \
		   #util/run/runwordrec/RunWordrec.cpp \
		   #util/run/runshaperec/RunShaperec.cpp \

		   ## Feature extractors taken out...maybe put back in?		   
		   #reco/shaperec/featureextractor/l7/l7.cpp \
		   #reco/shaperec/featureextractor/l7/L7ShapeFeatureExtractor.cpp \
		   #reco/shaperec/featureextractor/l7/L7ShapeFeature.cpp \
		   #reco/shaperec/featureextractor/pointfloat/PointFloatShapeFeatureExtractor.cpp \
		   #reco/shaperec/featureextractor/pointfloat/PointFloatShapeFeature.cpp \
		   #reco/shaperec/featureextractor/pointfloat/PointFloat.cpp \
		   #reco/shaperec/featureextractor/npen/NPenShapeFeature.cpp \
		   #reco/shaperec/featureextractor/npen/NPenShapeFeatureExtractor.cpp \
		   #reco/shaperec/featureextractor/npen/NPen.cpp \
		   #reco/shaperec/preprocessing/preprocessing.cpp \
		   #reco/shaperec/preprocessing/LTKPreprocessor.cpp \
		   #reco/shaperec/activedtw/ActiveDTWClusterModel.cpp \
		   #reco/shaperec/activedtw/ActiveDTWAdapt.cpp \
		   #reco/shaperec/activedtw/ActiveDTWShapeModel.cpp \
		   #reco/shaperec/activedtw/ActiveDTWShapeRecognizer.cpp \
		   #reco/shaperec/activedtw/ActiveDTW.cpp \
		   #reco/shaperec/common/LTKShapeRecoUtil.cpp \
		   #reco/shaperec/common/LTKShapeRecoConfig.cpp \
		   #reco/shaperec/common/LTKShapeRecoResult.cpp \
		   #reco/shaperec/common/LTKShapeSample.cpp \
		   #reco/shaperec/common/LTKShapeRecognizer.cpp \
		   #reco/shaperec/featureextractor/substroke/SubStrokeShapeFeature.cpp \
		   #reco/shaperec/featureextractor/substroke/SubStrokeShapeFeatureExtractor.cpp \
		   #reco/shaperec/featureextractor/substroke/SubStroke.cpp \
		   #reco/shaperec/featureextractor/common/LTKShapeFeatureExtractor.cpp \
		   #reco/shaperec/featureextractor/common/LTKShapeFeatureExtractorFactory.cpp \
		   #reco/shaperec/neuralnet/NeuralNetShapeRecognizer.cpp \
		   #reco/shaperec/neuralnet/NeuralNet.cpp \
		   #reco/shaperec/nn/NN.cpp \
		   #reco/shaperec/nn/NNAdapt.cpp \
		   #reco/shaperec/nn/NNShapeRecognizer.cpp \
		   #reco/wordrec/boxfld/BoxFieldRecognizer.cpp \
		   #reco/wordrec/boxfld/boxfld.cpp \
		   #reco/wordrec/common/LTKWordRecoResult.cpp \
		   #reco/wordrec/common/LTKRecognitionContext.cpp \
		   #reco/wordrec/common/LTKWordRecoConfig.cpp \
		   #util/featurefilewriter/featurefilewriter.cpp \


# Here we give our module name and source file(s)
LOCAL_MODULE    := lipitk
LOCAL_SRC_FILES := util/logger/LTKLogger.cpp \
		   util/logger/logger.cpp \
		   util/lib/LTKErrors.cpp \
		   util/lib/LTKStrEncoding.cpp \
		   util/lib/LTKInkUtils.cpp \
		   util/lib/LTKInkFileReader.cpp \
		   util/lib/LTKOSUtilFactory.cpp \
		   util/lib/LTKConfigFileReader.cpp \
		   util/lib/LTKInkFileWriter.cpp \
		   util/lib/LTKImageWriter.cpp \
		   util/lib/LTKLoggerUtil.cpp \
		   util/lib/LTKStringUtil.cpp \
		   util/lib/LTKLinuxUtil.cpp \
		   util/lib/LTKVersionCompatibilityCheck.cpp \
		   util/lib/LTKCheckSumGenerate.cpp \
		   common/LTKTraceGroup.cpp \
		   common/LTKScreenContext.cpp \
		   common/LTKException.cpp \
		   common/LTKCaptureDevice.cpp \
		   common/LTKTrace.cpp \
		   common/LTKChannel.cpp \
		   common/LTKTraceFormat.cpp \
		   lipiengine/lipiengine.cpp \
		   lipiengine/LipiEngineModule.cpp \
		   reco/shaperec/common/LTKShapeRecoUtil.cpp \
		   reco/shaperec/common/LTKShapeRecoConfig.cpp \
		   reco/shaperec/common/LTKShapeRecoResult.cpp \
		   reco/shaperec/common/LTKShapeSample.cpp \
		   reco/shaperec/common/LTKShapeRecognizer.cpp \
		   reco/shaperec/featureextractor/common/LTKShapeFeatureExtractor.cpp \
		   reco/shaperec/featureextractor/common/LTKShapeFeatureExtractorFactory.cpp \
		   reco/shaperec/nn/NN.cpp \
		   reco/shaperec/nn/NNAdapt.cpp \
		   reco/shaperec/nn/NNShapeRecognizer.cpp
		   
#######################################
##        Application Link           ##
#######################################
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_MODULE    := jnilink
LOCAL_SRC_FILES := LipiTKJNIInterface.cpp

include $(BUILD_SHARED_LIBRARY)
