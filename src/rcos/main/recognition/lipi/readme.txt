
Welcome to Lipi core toolkit 4.0.0
--------------------------------

README for Lipi core toolkit 4.0.0

Introduction
----------------

The Lipi core toolkit provides a set of components which can be used for the creation and evaluation of recognizers for isolated shapes such as handwritten gestures and characters. It also provides pre-built recognizers that can be readily integrated into client applications. 

The supported platforms
-----------------------------
Windows 7 (x86, x64)
Ubuntu 10.10 (x86_64, i686)

TABLE OF CONTENTS
----------------------------------
1. Installing lipi-core-toolkit 4.0.0
2. Executing binaries
3. Building the source 
4. User Manual
5. Known Issues / Limitations


1. Installing lipi-core-toolkit 4.0.0
------------------------------
a. lipi-core-toolkit 4.0.0 Packages : 

lipi-core-toolkit4.0.0-bin - Binary package for Windows 7 edition for VC2008

lipi-core-toolkit4.0.0-src-win - Source package for Windows 7 [VC2008] 

lipi-core-toolkit4.0.0-linux-x86_64 - Binary package for Linux

lipi-core-toolkit4.0.0-linux-i686 - Binary package for Linux

lipi-core-toolkit4.0.0-src.linux - Source package for Linux

b. Unpacking / Extracting the package:
 
	On Linux platform use the following command to extract
	              	tar -xzvf  lipi-core-toolkit4.0.0-linux-i686.tar.gz
	              	tar -xzvf  lipi-core-toolkit4.0.0-linux-x86_64.tar.gz

	On Windows use installer
	
		Double click/execute the installable file
		
			lipi-core-toolkit4.0.0-src-win.exe
		
2. Executing binaries
-----------------------
The user needs to set LIPI_ROOT environment variable before executing the binaries. On Windows, this variable is set automatically on installation of tool kit. On Linux, the user can either export LIPI_ROOT manually or can use build.sh/build-x64.sh with -s option.

For example (on Linux):
# export LIPI_ROOT=/opt/lipi
	where /opt/lipi is the installed location of lipi toolkit
OR
# sh build.sh -s (on 32 bit)

# sh build-x64.sh -s (on 64 bit)

3. Building the source
------------------------
For building source on Windows, please refer 'Building Source package' section of User Manual.
For building source on Linux, please refer the same section of User Manual. The source on Linux can also be build using build.sh or build-x64.sh script.

For example (On Linux):

# sh build.sh -b (To build source on 32-bit)
# sh build.sh -c (To clean source on 32-bit)

# sh build-x64.sh -b (To build source on 64-bit)
# sh build-x64.sh -c (TO clean source on 64-bit)

4. User Manual
---------------------
The detailed user manual can be found at doc/lipi-core-toolkit_4_0_User_Manual.pdf

5. Known Issues / Limitations
-----------------------------------------
1. Spaces in file paths in training/test list files

Issue:
In list files for training or testing, if the file path contains spaces, then runshaperec.exe reports an error, and training or testing fails. 

Workaround: 
Do not use directory or files names with spaces in them, or (for Windows platforms) use the DOS path instead (e.g. for C:\program files, use “C:\progra~1”).
