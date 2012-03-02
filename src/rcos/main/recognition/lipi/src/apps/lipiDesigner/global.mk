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
LIPIIDETK = LIPIIDE
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

LIPIIDE_BIN=$(LIPI_ROOT)/src/apps/lipiDesigner/bin
LIPIIDE_DOC=$(LIPI_ROOT)/src/apps/lipiDesigner/doc
LIPIIDE_LIB=$(LIPI_ROOT)/src/apps/lipiDesigner/lib
LIPIIDE_PACK=$(LIPI_ROOT)/src/apps/lipiDesigner/package
LIPIIDE_PROJ=$(LIPI_ROOT)/src/apps/lipiDesigner/projects
LIPIIDE_SCRIPT=$(LIPI_ROOT)/src/apps/lipiDesigner/scripts
LIPIIDE_SRC=$(LIPI_ROOT)/src/apps/lipiDesigner/src
LIPIIDE_SRC_LIB=$(LIPIIDE_SRC)/lib
LIPIIDE=LIPIIDE
VERSION=v2.0
OS=linux

LINUX_LOCATION=linux
LIPIIDE_DYNAMIC_LIBDIR=$(LIPIIDE_LIB)
LIPIIDE_BIN=$(LIPI_ROOT)/bin
LIPIIDE_STATIC_LIBDIR=$(LIPIIDE_LIB)
LIPIIDE_SRC_INCLUDE=$(LIPIIDE_SRC)/include
LIPIIDE_SRC_JAVA=$(LIPIIDE_SRC)/javauserinterface
LIPIIDE_JNI=$(LIPIIDE_SRC)/lipijniinterface
LIPIIDE_SRC_APPS=$(LIPIIDE_SRC)/apps
LIPIIDE_SAMPLES=$(LIPIIDE_SRC_APPS)/samples
LIPIIDE_SRC_UTILS=$(LIPIIDE_SRC)/util
LIPIIDE_SRC_UTILS_LIB=$(LIPIIDE_SRC_UTILS)/lib
LIPIIDE_SAMPLES_SHAPERECTST=$(LIPIIDE_SAMPLES)/shaperectst
