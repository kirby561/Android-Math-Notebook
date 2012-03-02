#######################################################################################
# Copyright (c) 2006 Hewlett-Packard Development Company, L.P.
# Permission is hereby granted, free of charge, to any person obtaining a copy of 
# this software and associated documentation files (the "Software"), to deal in 
# the Software without restriction, including without limitation the rights to use, 
# copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
# Software, and to permit persons to whom the Software is furnished to do so, 
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
#######################################################################################

#!/bin/perl -w
use Getopt::Long;
use strict;
my $Major_Version= 4;
my $Minor_Version= 0;
my $Bugfix_Version= 0;

##########################################################################################
#
#   Variables initialized with command line arguments
#
##########################################################################################


my $projectName;
my $profileName;
my $helpRequired = "";
my $version = "";
my $LIPI_ROOT = "";
my $trainListFilePath = ""; # The train list file
my $testListFilePath = ""; # The test list file
my $logFilePath  = ""; # Logfile 
my $exitStatus = 0;


##########################################################################################
#
#   
#
##########################################################################################
my $resultFile = "runshaperec.out";
my $defaultLogFile = "lipi.log";

my $Command = "";
{
     	# Reading Command line arguements
	if($#ARGV==-1)	
	{
		display_usage();
		exit;
	}
	GetOptions('ver|v'        => \$version,
		   'help'         => \$helpRequired,
		   'project=s'    => \$projectName,
		   'profile=s'    => \$profileName,
		   'train=s'      => \$trainListFilePath,
	   	   'test=s'       => \$testListFilePath,
		   'logfile=s'    => \$logFilePath,
		   'lipiroot=s'   => \$LIPI_ROOT);

	if($version)
	{
		print "Benchmark script Version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
		exit;
	}

	if($helpRequired)
	{
		display_usage();
		exit;
	}
	
	# replacing \ with / supported in WINDOWS and LINUX
	if($trainListFilePath =~ /\\/)
	{
		$trainListFilePath =~ s/\\/\//g;
	}	
	# replacing \ with / supported in WINDOWS and LINUX
	if($testListFilePath =~ /\\/)
	{
		$testListFilePath =~ s/\\/\//g;
	}
	# replacing \ with / supported in WINDOWS and LINUX
	if($logFilePath =~ /\\/)
	{
		$logFilePath =~ s/\\/\//g;
	}
    
	# Verifying all command line options 
	if ($projectName eq "")
	{
		print "\n\n\nError : No project mentioned. Please enter the project name using -project option\n\n\n";
		exit;
	}

	if($trainListFilePath eq ""){
		print "\n\n\nError : Missing train list file. Input train list file using -train option\n\n\n";
		exit;
	}
	if($testListFilePath eq ""){
		print "\n\n\nError : Missing test list file. Input test list file using -test option\n\n\n";
		exit;
	}
	if($logFilePath eq ""){
		print "warning : Missing  log file. Using default log file : $defaultLogFile\n";
		$logFilePath = $defaultLogFile;
	}
	
	if($LIPI_ROOT eq "")
	{
		$LIPI_ROOT = $ENV{'LIPI_ROOT'};
		if($LIPI_ROOT eq "")
		{
			print "Error: LIPI_ROOT env variable is not set";
			exit;
		}
	}


	if ($profileName eq "")
	{
		print "warning : No profile mentioned . Using default profile\n";
		$profileName = "default";
	}
	
	$Command = "$LIPI_ROOT/bin/runshaperec -train $trainListFilePath -logfile $logFilePath -project $projectName -profile $profileName -lipiroot $LIPI_ROOT ";
	$exitStatus = system($Command);

	if ($exitStatus != 0)
	{
		print "\n\n\n Error training the shape recognizer. Please see the log file for more details\n\n\n";
		exit;
	}

	$exitStatus = 0;
	
	$Command = "$LIPI_ROOT/bin/runshaperec -test $testListFilePath -logfile $logFilePath -project $projectName -profile $profileName -output $resultFile -lipiroot $LIPI_ROOT ";
	$exitStatus = system($Command);


	if ($exitStatus != 0)
	{
		print "\n\n\n Error testing the shape recognizer. Please see the log file for more details\n\n\n";
		exit;
	}

	$Command = "perl $LIPI_ROOT/scripts/eval.pl -input $resultFile " ;
	system($Command);

}

sub display_usage
{
	print "\n\nUsage : benchmark.pl\n";
	print "\nbenchmark.pl\n";
	print "\n\t-project <project name>\n";
	print "\n\t-train <train list file path>\n";
	print "\n\t-test  <test list file path>\n";
	print "\n\t-logfile   <logfilename>\n";
	print "\n\t[-profile <profile name>]\n";
	print "\n\t[-lipiroot  <root path of the Lipi>]\n";
	print "\n\t[-ver or -v (displays the version)]\n";
	print "\n\t[-help (displays this list)]\n\n";
}
