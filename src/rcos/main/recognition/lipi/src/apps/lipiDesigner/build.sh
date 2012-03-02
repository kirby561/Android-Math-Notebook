#! /bin/sh

# system libraries
SYS_LIB="libdl libm libc libgcc_s"
SYS_USR_LIB="libstdc++"

# Lipitk libraries
LIPITK_LIB="libcommon.a libshaperecommon.a libwordreccommon.a libfeatureextractorcommon.a libutil.a"

# system executable
SYS_EXE=/usr/bin/g++

# java-6-openjdk
JAVA_DIR=/usr/lib/jvm/java-6-openjdk
JAR_FILE=lipiDesigner.jar

# perl module (Zip)
ZIP="Zip"

# flag for system libraries
flag=0

# flag for Lipitk libraries
lflag=0

# lipitk directory
LIPITK_DESIGNER=`pwd`
DESIGNER_PATH=src/apps/lipiDesigner
LIPITK_DIR=${LIPITK_DESIGNER%$DESIGNER_PATH}

# LIPI_ROOT for LipiDesigner build
export LIPI_ROOT=$LIPITK_DIR

# build.sh usage
usage="
Usage: $0 [OPTIONS]
Builds LipiDesigner to current machine

-b,--build
	Builds LipiDesigner
-c,--clean
	Cleans LipiDesigner
-s,--set
	Sets LipiDesigner's Runtime Environment
-h,--help
	Shows this help screen
"

while [ "$1" ]; do
	case $1 in
	-b|--build)
		build=yes
		;;
	-c|--clean)
		clean=yes
		;;
	-s|--set)
		sets=yes
		;;
	-h|--help) 
		echo "$usage"
		exit 0
		;;
	*)
		echo "\nERROR: Unrecognized option $1"
		exit 1
	esac
	shift
done

# default mode is usage
if [ ! "$build" = yes ] && [ ! "$clean" = yes ] && [ ! "$sets" = yes ]; then
	echo "$usage"
	exit 0	
fi

# validity check
if [ "$build" = yes ] && [ "$clean" = yes ]; then
	echo "\nERROR: -b and -c flags cannot be used together.\n"
	exit 1
fi

if [ "$sets" = yes ] && [ "$clean" = yes ]; then
	echo "\nERROR: -c and -s flags cannot be used together.\n"
	exit 1
fi

# build/clean option
if [ "$build" = yes ]; then

	# check operating system 
	os=`uname -a | cut -f4 -d " " | cut -f2 -d "-"`

	if [ $os != "Ubuntu" ]
	then
		echo "\nERROR: Lipitk can be built only on Ubuntu Linux.\n"
		exit 1
	fi

	# check system libraries/execuatble
	for filename in $SYS_LIB; do
		find /lib -print | grep "$filename\." 1>/dev/null 2>&1
		if [ $? -ne 0 ]
		then
			flag=1
		fi
	done

	for filename in $SYS_USR_LIB; do
		find /usr/lib -print | grep "$filename\." 1>/dev/null 2>&1
		if [ $? -ne 0 ]
		then
			flag=1
		fi
	done

	ls $SYS_EXE 1>/dev/null 2>&1
	if [ $? -ne 0 ]
	then
		flag=1
	fi

	if [ $flag -eq 1 ]
	then
		echo "\nERROR: Required library missing from /lib & /usr/lib - $SYS_LIB $SYS_USR_LIB $SYS_EXE or the executable - $SYS_EXE.\n\n"
		exit 1
	fi

	# check Lipitk libraries
	for filename in $LIPITK_LIB; do
		ls $LIPI_ROOT/src/lib/$filename 1>/dev/null 2>&1
		if [ $? -ne 0 ]
		then	
			lflag=1
		fi
	done

	if [ $lflag -eq 1 ]
	then
		echo "\nERROR: Lipitk library missing from $LIPI_ROOT/src/lib, please build Lipitk first.\n"
		exit 1
	fi

	ls $JAVA_DIR/include 1>/dev/null 2>&1
	if [ $? -ne 0 ]
	then
		echo "\nERROR: openjdk-6-jdk package not installed.\n"
		exit 1
	fi

	find /usr/share/perl -print | grep Archive | grep -w $ZIP 1>/dev/null 2>&1
	if [ $? -ne 0 ]
	then 
		echo "\nWARNING: Archive::Zip perl module under /usr/share/perl not installed. The package functionality in lipiDesigner may not work."
	fi

	# JAVA_HOME for LipiDesigner build
	export JAVA_HOME=$JAVA_DIR

	printf "\nBuilding lipijniinterface objects....."
	make -f $LIPITK_DESIGNER/src/lipijniinterface/linux/Makefile.linux  1> $LIPITK_DESIGNER/build.log 2>&1

	if [ $? -ne 0 ]
	then
		printf "FAILED. Please check build.log for details.\n"
		exit 1
	else
		printf "OK"
	fi

	printf "\nBuilding javauserinterface jar....."
	sh $LIPITK_DESIGNER/src/javauserinterface/linux/buildjar.sh 1>> $LIPITK_DESIGNER/build.log 2>&1

	if [ $? -ne 0 ]
	then
		printf "FAILED. Please check build.log for details.\n"
		exit 1
	else
		printf "OK\n\n"
	fi
elif [ "$clean" = yes ]; then
	printf "\nRemoving lipijniinterface objects....."
	make -f $LIPITK_DESIGNER/src/lipijniinterface/linux/Makefile.linux clean 1> $LIPITK_DESIGNER/build.log 2>&1

	if [ $? -ne 0 ]
	then
		printf "FAILED. Please check build.log for details.\n\n"
		exit 1
	else
		printf "OK"
	fi

	printf "\nRemoving javauserinterface jar....."
	rm -f $LIPITK_DESIGNER/bin/$JAR_FILE 1>> $LIPITK_DESIGNER/build.log 2>&1

	if [ $? -ne 0 ]
	then
		printf "FAILED. Please check build.log for details.\n"
		exit 1
	else
		printf "OK\n\n"
	fi
fi

if [ "$sets" = yes ]; then
	printf "\nSetting lipiDesigner runtime environment....."

	export LIPI_ROOT=$LIPITK_DIR
	printf "OK\n\n"
	echo "The environment is set to run lipiDesigner. Please type <exit> once you are done with this environment.\n"
	/bin/bash
	exit 0
fi

