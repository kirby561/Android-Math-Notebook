                                 README

                         Lipi Designer Version 4.0

Contents

   * Introduction
   * System Requirements
   * Executing binary
   * Building the source
   * Release Notes
   * Bug Reports and Feedback   
   

Introduction

	Lipi Designer is a Java application which provides a GUI for creating, 
	loading & training and packaging shapre recognizer projects. It can be 
	used for the rapid creation of recognizers for a set of characters or 
	gestures. Once the recognizer has been trained, and the accuracy of 
	recognition is found to be satisfactory, it may be packaged using the
	packaging feature provided by the application

System Requirements & Installation

	*Requirements*:

	Windows 7 	               
	Microsoft Visual Studio 2008
	Java Development Kit (JDK) jdk 1.6.0_26 or above
	Java Runtime Environment (JRE) 1.6.0_26 and above

	Ubuntu 10.10 (x86_64, i686)

Executing binary

	The user needs to set LIPI_ROOT environment variable before executing 
	the binary. On Windows, this variable is set automatically on installation
	of tool kit. On Linux, the user can either export LIPI_ROOT manually or can
	use build.sh/build-x64.sh script with -s option. This script can be found
	under $LIPI_ROOT/src/apps/lipiDesigner directory.

	For example (on Linux):
	# export LIPI_ROOT=/opt/lipi
		where /opt/lipi is the installed location of lipi toolkit
	OR
	# sh build.sh -s (on 32 bit)
	
	# sh build-x64.sh -s (on 64 bit)

	Once LIPI_ROOT is set, lipiDesigner can be launched as shown below:
	
	# cd bin
	# sh lipiDesigner.sh

Building the source

	For building source on Windows, please refer 'Building Lipi Designer source 
	code' section of Lipi Designer User Manual.

	For building source on Linux, please refer the same section of Lipi Designer 
	User Manual. The source on Linux can also be build using build.sh or
	build-x64.sh script.

	For example:
	# sh build.sh -b (To build source on Linux 32-bit)
	# sh build.sh -c (To clean source on Linux 32-bit)
	
	# sh build-x64.sh -b (To build source on Linux 64-bit)
	# sh build-x64.sh -c (TO clean source on Linux 64-bit)


Release Notes/User Manual

	See the lipi-designer-4.0_user_manual.pdf/lipi-designer-4.0_release_notes.doc  document for additional 
	information pertaining to this release. 

Bug Reports and Feedback

	The Bug Parade Web Page on the LipiTk web 
	site lets you search for and examine existing bug reports, submit 
	your own bug reports, and tell us which bug fixes matter most to you. 
 
      <lipitk.sourceforge.net>

