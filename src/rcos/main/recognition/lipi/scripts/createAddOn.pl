########################################################################################
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

#######################################################################################
# A perl script (createrecoaddon.pl) for creating a recognizer add-on
#	a.Inputs: -project <project name> -logicalname <logical name> [-profile <profile name>] [-lipiroot <path of the lipiroot>] [-ver or -v (displays the version)] [-help (displays this list)]
#	b.Ouput: A zip file having the name as logical name and is independent of the platform
#		i. Contains a dynamically generated perl script (?, alternatively batch and shell scripts)
#			1. Check for LipiTk installation
#			2. Update or create lipiengine.cfg with the logical name, project and profile.
#				a. If the logical name already exists, provide options to overwrite, provide new logical name or abort
#######################################################################################

use Getopt::Long; #qw(no_ignore_case);
use Time::gmtime qw/:FIELDS/;
use File::Copy;
use Archive::Zip qw( :ERROR_CODES :CONSTANTS );
use Cwd;

our @SECTIONS; # filled by read_config()
if ($#ARGV == -1  ) 
{
	display_usage();
}

##### Global variables #####
my $g_ostype =  $^O ; #Get OS type
my $g_major_version = "4";
my $g_minor_version = "0";
my $g_bugfix_version = "0";
my $g_project = "";
my $g_logicalname = "";
my $g_profile = "default";
my $g_lipitk_root = ""; 
my $g_ProjectDir = "projects";
my $g_PackageDir = "package";
my $g_Config = "config";
my $g_BatchFileName = "run.bat";
my $g_Lipiengine = "lipiengine.cfg";

#####linux related compiler option has to be filled properly#####
if ($g_ostype eq "linux") 
{
   $g_sep = "/";
   $g_copyDirectory = "cp -rf";
}
else 
{
   $g_sep = "\\";
   $g_copyDirectory = "xcopy /eyiq";
}

GetOptions('ver|v'     => \$version,
	'help'      => \$helpRequired,
	'project=s'       => \$g_project,
	'logicalname=s' => \$g_logicalname,
	'profile=s' => \$g_profile,
	'lipiroot=s' => \$g_lipitk_root);

if($version)
{
	print "package script version : $g_major_version.$g_minor_version.$g_bugfix_version\n";
	exit;
}

if($helpRequired)
{
	display_usage();
}


#Check command line options;
if ( ($g_project eq "") || ($g_logicalname eq "") )
{
	print "Error: Invalid arguments, please see usage(-help)." ;
	exit;
}

if($g_lipitk_root eq "")
{
	$g_lipitk_root = $ENV{'LIPI_ROOT'};
	if($g_lipitk_root eq "")
	{
		print "Error: LIPI_ROOT enviornment variable is not set";
		exit;
	}
}

main();

sub display_usage
{
     print "\n\nUsage : createrecoaddon.pl\n";
     print "\ncreaterecoaddon.pl\n";
     print "\n\t-project <project name>\n";
     print "\n\t-logicalname <logical name>\n";
     print "\n\t[-lipiroot <path of the lipiroot>]\n";
     print "\n\t[-ver or -v (displays the version)]\n";
     print "\n\t[-help (displays this list)]\n\n";
     exit;
}

sub main
{

#########################################################################
# 
# How to create zip file using Archive::Zip command line
#
#########################################################################
	
	# check project is exist in "projects" directory
	local $projectDirectory = $g_lipitk_root.$g_sep.$g_ProjectDir;
    if ( IsDirExist ($projectDirectory,$g_project) == -1)
	{
		print "Error:  given project $g_project does not exist in ${g_lipitk_root}/projects directory.";
		exit;
	}
	
	# check "profile" directory exists in config dir
	local $configDirectory = $g_lipitk_root.$g_sep.$g_ProjectDir.$g_sep.$g_project.$g_sep.$g_Config;
    if ( IsDirExist ($configDirectory,$g_profile) == -1)
	{
		print "Error: given profile $g_profile does not exist in $configDirectory directory.";
		exit;
	}
	
	# create temp folder in package directory for package
	local $packageDirectory = $g_lipitk_root.$g_sep.$g_PackageDir.$g_sep.$g_logicalname;
	local $Mkdir = "mkdir  $packageDirectory";
	local $error = system($Mkdir);
	if($error != 0)
	{
		print "Error: can not create $packageDirectory";
		exit;
	}
	
	# create projects directory under temp folder
	local $projsDirectory = $packageDirectory.$g_sep.$g_ProjectDir;
	$Mkdir = "mkdir  $projsDirectory";
	$error = system($Mkdir);
	if($error != 0)
	{
		print "Error: can not create $projsDirectory";
		exit;
	}
	
	# create project name directory under temp projects folder
	local $projDirectory = $projsDirectory.$g_sep.$g_project;
	$Mkdir = "mkdir  $projDirectory";
	$error = system($Mkdir);
	if($error != 0)
	{
		print "Error: can not create $projDirectory";
		exit;
	}
	
	my $workDir = getcwd;
	
	local $projFile = "${workDir}${g_sep}projDetails.txt";
	print "projFile=${projFile}\n";
	open (projFile, ">", $projFile) or die "failed to create projDetails.txt";
	print projFile "projName=${g_project}\n";
	print projFile "logicalName=${g_logicalname}\n";
	close projFile;
	
	# copy project to temporary project folder
	local $FromDirectory = $g_lipitk_root.$g_sep.$g_ProjectDir.$g_sep.$g_project;
	&CopyDirectory($FromDirectory,$projDirectory);
	
	sleep 5;
	
	my $packageDir = $g_lipitk_root.$g_sep.$g_PackageDir;
	my $package = "SHAPEREC_.${g_project}";
	
	if ( IsDirExist ($packageDir,$package) == -1)
	{
		&CreatePackage($packageDirectory);
	}
	exit;
	if (-e $projFile) 
	{
		unlink($projFile);		
	}
	return;
	
}

sub IsDirExist {
	local ($projectDirectory) = $_[0];
	local ($projectName) =  $_[1];
	
	opendir(PROJ_DIR,$projectDirectory);
	@allProjects = readdir(PROJ_DIR);

    foreach $project (@allProjects) {
		if ( $project eq $projectName ) {
			close PROJ_DIR;
			return 0;
		}
    }
	close PROJ_DIR;
	
	return -1;
};

sub CopyDirectory
{
	local ($fromLocation) = $_[0];
	local ($toLocation) = $_[1];

	opendir(DIR_HANDLE,$fromLocation ) || die " Could not open the $fromLocation\n\n";
	@temp = readdir(DIR_HANDLE);

	foreach $f (@temp)
	{
		unless ( ($f eq ".") || ($f eq "..") || ($f eq ".svn") )
		{ 
			push(@testProjects, $f);
		}
	}

	foreach $MoveProject (@testProjects )
	{
		local $toSubDir = "${toLocation}${g_sep}${MoveProject}";
		local $fromSubDir = "${fromLocation}${g_sep}${MoveProject}";
		if ( -d $fromSubDir )
		{
		   $command = "$g_copyDirectory $fromSubDir $toSubDir ";
		   #print "Copying command $command";
		   system($command);
		}
	}
    close DIR_HANDLE;
}

sub CreatePackage {
	local ($pkgLocation) = $_[0];
	$pkgPath = "${pkgLocation}${g_sep}${g_ProjectDir}${g_sep}${g_project}${g_sep}"; # path of temp project location
	$destZip = "${g_logicalname}.zip"; # zip file name
	
	my $zip = Archive::Zip->new();
		
	my $status = $zip->addTree( $pkgPath, $g_project );
	warn "Can't add tree \n" if $status != AZ_OK;

	my $file = $zip->addFile("./projDetails.txt");
	warn "can not add projDetails to zip file error \n" if $status != AZ_OK;
	
	foreach my $member ($zip->members)
	{
		$member->desiredCompressionMethod(COMPRESSION_DEFLATED );
		$member->desiredCompressionLevel(9);	
	}
	
	my $status = $zip->writeToFileNamed( $destZip );
    die "can not write to zip file error somewhere \n" if $status != AZ_OK;

	
	my $packageDir = $g_lipitk_root.$g_sep.$g_PackageDir;
	
	$command = "$g_copyDirectory $destZip $packageDir";
	
	sleep(3);
	
	$error = system($command);
	if($error != 0)
	{
		print "Error: can not copy zip file to package pirectory";
		#exit;
	}
	
	sleep(4);

	return;
}