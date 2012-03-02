#! /bin/sh

# system libraries
SYS_LIB="libdl libm libc libgcc_s"
SYS_USR_LIB="libstdc++"

# system executable
SYS_EXE=/usr/bin/g++

# flag for system libraries
flag=0

# lipitk directory
LIPITK_DIR=`pwd`

# LIPI_ROOT for Lipitk build
export LIPI_ROOT=$LIPITK_DIR

# build.sh usage
usage="
Usage: $0 [OPTIONS]
Builds LipiTK to current machine

-b,--build
	Builds LipiTK
-c,--clean
	Cleans LipiTK
-s,--set
	Sets LipiTK's Runtime Environment
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
		echo "\nERROR: Unrecognized option $1\n"
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
	echo "\nERROR: -b and -c flags cannot be used together\n"
	exit 1
fi

if [ "$sets" = yes ] && [ "$clean" = yes ]; then
	echo "\nERROR: -c and -s flags cannot be used together\n"
	exit 1
fi

# build/clean option
if [ "$build" = yes ]; then
	# check operating system 
	os=`uname -a | cut -f4 -d " " | cut -f2 -d "-"`

	if [ $os != "Ubuntu" ]
	then
		echo "\nERROR: Lipitk can be built only on Ubuntu Linux\n"
		exit 1
	fi

	# check system libraries
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
		echo "\nERROR: Required library missing from /lib & /usr/lib - $SYS_LIB $SYS_USR_LIB or the executable - $SYS_EXE\n"
		exit 1
	fi

	printf "\nBuilding lipitk objects and binaries....."
	make -f linux/Makefile.linux  1> $LIPITK_DIR/build.log 2>&1

	if [ $? -ne 0 ]
	then
		printf "FAILED. Please check build.log for details.\n\n"
		exit 1
	else
		printf "OK\n\n"
	fi
elif [ "$clean" = yes ]; then
	printf "\nRemoving lipitk objects and binaries....."
	make -f linux/Makefile.linux clean 1> $LIPITK_DIR/build.log 2>&1

	if [ $? -ne 0 ]
	then
		printf "FAILED. Please check build.log for details.\n\n"
		exit 1
	else
		printf "OK\n\n"
	fi
fi

if [ "$sets" = yes ]; then
	printf "\nSetting lipitk runtime environment....."

	export LIPI_ROOT=`pwd`
	printf "OK\n\n"
	echo "The environment is set to run lipitk executables. Please type <exit> once you are done with this environment.\n"
	/bin/bash
	exit 0
fi
