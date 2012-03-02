# vim: set sw=8 ts=4 si et:
# written by: Rajesh pandry K
# contact id: pandry@hp.com
#
######################
# Commands
######################
CP= cp 
RM= rm -rf
REMOVE=rm
AR= ar rc
LD= /usr/bin/ld
LD_SHARED = -b
MV= /bin/mv
MAKE= /usr/bin/make
SHELL=/bin/sh
RANLIB  =/usr/bin/ranlib
CHMOD = @chmod 0777
MKDIR = mkdir -p
TAR_CVF= tar -Pzvcf
TAR_XVF = tar -xzvf
TAR_EXT = tar.gz
CURRENT_DIR = .
CHDIR = cd
RMDIR = rm -rf
MOVE = mv
LIPITKTK = lipitk
ECHO = echo
MAKE = make
PERL=perl


PLATFORM=Linux
F=-f

# SLIB_SUFFIX stands for the share library's suffix
SLIB_SUFFIX = so

#####################################
#Compiler and Compiler Flags
####################################
CC=g++
CC_x64=g++ -m64

# link librariues 
LINKLIB=-ldl -lc 
# CC_SHARED used in link a shared object
SHFLAGS=-shared 
#FPIC used in object creation
FPIC=-fPIC


# to compile for debugging add -g to CFLAGS
# CFLAGS is used for compiler options
CPPFLAGS=-Wno-deprecated 
CFLAGS=-c 

# LFLAGS is used for link options
LFLAGS= -c  

DIST_COMMON =  README AUTHORS COPYING TODO ChangeLog INSTALL Makefile.linux Makefile.win 
DISTDATA=
DISTSRCDIR=
DISTINC=
DISTLIB=

DISTFILES=$(DIST_COMMON) $(DISTDATA) $(DISTSRCDIR) $(DISTINC) $(DISTLIB)

LIPITK_BIN=$(LIPI_ROOT)/bin
LIPITK_DOC=$(LIPI_ROOT)/doc
LIPITK_LIB=$(LIPI_ROOT)/lib
LIPITK_PACK=$(LIPI_ROOT)/package
LIPITK_PROJ=$(LIPI_ROOT)/projects
LIPITK_SCRIPT=$(LIPI_ROOT)/scripts
LIPITK_UTILS=$(LIPI_ROOT)/utils
LIPITK_SRC=$(LIPI_ROOT)/src
LIPITK_SRC_LIB=$(LIPITK_SRC)/lib
LIPITK_TMP=$(LIPITK_PACK)/tmp
LIPITK=lipitk
VERSION=v1.0
OS=linux

LINUX_LOCATION=linux
LINUX_x64_LOCATION=linux-x64
LIPITK_DYNAMIC_LIBDIR=$(LIPI_ROOT)/lib
LIPITK_BIN=$(LIPI_ROOT)/bin
LIPITK_STATIC_LIBDIR=$(LIPITK_SRC)/lib
LIPITK_SRC_INCLUDE=$(LIPITK_SRC)/include
LIPITK_LIPIENGINE=$(LIPITK_SRC)/lipiengine
LIPITK_RECO=$(LIPITK_SRC)/reco
LIPITK_RECO_UTIL=$(LIPITK_RECO)/util
LIPITK_TOOLS=$(LIPITK_SRC)/tools
LIPITK_SRC_APPS=$(LIPITK_SRC)/apps
LIPITK_SRC_UTILS=$(LIPITK_SRC)/util
LIPITK_SRC_UTILS_LIB=$(LIPITK_SRC_UTILS)/lib
LIPITK_LOGGER=$(LIPITK_SRC_UTILS)/logger
LIPITK_SRC_UTILS_TRAINHMM=$(LIPITK_SRC_UTILS)/trainhmm
LIPITK_SRC_UTILS_IMGWRITER=$(LIPITK_SRC_UTILS)/imgwriter
LIPITK_SRC_UTILS_RUN=$(LIPITK_SRC_UTILS)/run
LIPITK_SRC_UTILS_RUNSHAPEREC=$(LIPITK_SRC_UTILS)/run/runshaperec
LIPITK_SRC_UTILS_RUNWORDREC=$(LIPITK_SRC_UTILS)/run/runwordrec
LIPITK_SRC_UTILS_MDV=$(LIPITK_SRC_UTILS)/mdv
LIPITK_SRC_UTILS_FFW=$(LIPITK_SRC_UTILS)/featurefilewriter
LIPITK_COMMON=$(LIPITK_SRC)/common
LIPITK_SAMPLES=$(LIPITK_SRC_APPS)/samples

#############################################
#   SHAPEREC MACROS
#############################################

LIPITK_SHAPEREC=$(LIPITK_RECO)/shaperec
LIPITK_SHAPEREC_COMMON=$(LIPITK_SHAPEREC)/common
LIPITK_SHAPEREC_PREPROC=$(LIPITK_SHAPEREC)/preprocessing
LIPITK_SHAPEREC_PCA=$(LIPITK_SHAPEREC)/pca
LIPITK_SHAPEREC_NN=$(LIPITK_SHAPEREC)/nn
LIPITK_SHAPEREC_UTILS=$(LIPITK_SHAPEREC)/util
LIPITK_SHAPEREC_TST=$(LIPITK_SHAPEREC)/tst
LIPITK_SHAPEREC_HMM=$(LIPITK_SHAPEREC)/hmm
LIPITK_SHAPEREC_ACTIVEDTW=$(LIPITK_SHAPEREC)/activedtw
LIPITK_SHAPEREC_NEURALNET=$(LIPITK_SHAPEREC)/neuralnet

#############################################
#   SHAPEREC FEATURE EXTRACTOR MACROS
#############################################

LIPITK_SHAPEREC_FE=$(LIPITK_SHAPEREC)/featureextractor
LIPITK_SHAPEREC_FE_COMMON=$(LIPITK_SHAPEREC_FE)/common
LIPITK_SHAPEREC_FE_POINTFLOAT=$(LIPITK_SHAPEREC_FE)/pointfloat
LIPITK_SHAPEREC_FE_L7=$(LIPITK_SHAPEREC_FE)/l7
LIPITK_SHAPEREC_FE_NPEN=$(LIPITK_SHAPEREC_FE)/npen
LIPITK_SHAPEREC_FE_SS=$(LIPITK_SHAPEREC_FE)/substroke

#############################################
#   WORDREDC MACROS
#############################################

LIPITK_WORDREC=$(LIPITK_RECO)/wordrec
LIPITK_WORDREC_COMMON=$(LIPITK_WORDREC)/common
LIPITK_WORDREC_BOXFLD=$(LIPITK_WORDREC)/boxfld


#############################################
#   SAMPLE MACROS
#############################################
LIPITK_SAMPLES_SHAPERECTST=$(LIPITK_SAMPLES)/shaperectst
LIPITK_SAMPLES_WORDRECTST=$(LIPITK_SAMPLES)/wordrectst
