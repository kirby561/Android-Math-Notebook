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
# A perl script (installAddOn.pl) for adding a recognizer add-on to existion lipi toolkit structure
#	a.Inputs:  [-lipiroot <path of the lipiroot>] [-ver or -v (displays the version)] [-help (displays this list)]
#	b.Ouput: Copies contentents of zip packages to toolkit projects directory and is independent of the platform
#
#			1. Check for LipiTk installation
#			2. Update or create lipiengine.cfg with the logical name, project and profile.
#				a. If the logical name already exists, provide options to overwrite, provide new logical name or abort
#######################################################################################

use Getopt::Long; #qw(no_ignore_case);
use Time::gmtime qw/:FIELDS/;
use File::Copy;
use File::Path;
use Archive::Zip qw( :ERROR_CODES :CONSTANTS );
use Cwd;
use Term::UI;
use Term::ReadLine;
use File::Basename;

#our @SECTIONS; # filled by read_config()
if ($#ARGV == -1  ) 
{
	display_usage();
}

##### Global variables #####
my $g_ostype =  $^O ; #Get OS type
my $g_major_version = "4";
my $g_minor_version = "0";
my $g_bugfix_version = "0";
my $g_profile = "default";
my $g_project = "";
my $g_lipitk_root = ""; 
my $g_zipFile = "";
my $g_ProjectDir = "projects";
my $g_Config = "config";
my $g_Lipiengine = "lipiengine.cfg";

#####linux related compiler option has to be filled properly#####
if ($g_ostype eq "linux") {
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
	'lipiroot=s' => \$g_lipitk_root,
	'file=s' => \$g_zipFile);

if($version)
{
	print "package script version : $g_major_version.$g_minor_version.$g_bugfix_version \n";
	exit;
}

if($helpRequired)
{
	display_usage();
}


#Check command line options;
if ( ($g_zipFile eq "") )
{
	print "Error: Please provide zip file name, please see usage(-help). \n" ;
	exit;
}


if($g_lipitk_root eq "")
{
	$g_lipitk_root = $ENV{'LIPI_ROOT'};
	if($g_lipitk_root eq "")
	{
		print "Error: LIPI_ROOT enviornment variable is not set, please check your lipi toolkit installation. \n";
		exit;
	}
}

main();

sub display_usage
{
     print "\n\nUsage : installAddOn.pl\n";
     print "\n installAddOn.pl\n";
	 print "\n\t-file <zip file name>\n";
	 print "\n\t[-lipiroot <path of the lipiroot>]\n";
     print "\n\t[-ver or -v (displays the version)]\n";
     print "\n\t[-help (displays this list)]\n\n";
     exit;
}

sub main
{

#########################################################################
# 
# extract files from a zip file to existing lipi toolkit projects directory using Archive::Extract command line
#
#########################################################################

	my $projectDirectory = "${g_lipitk_root}${g_sep}${g_ProjectDir}";
	
	print "$projectDirectory \n";
	
	############### Added to parse projDetails.txt created at time of package creation ################
	my $tempInstallDir = getcwd;
		
	&extractAddon();
	my $i = 0;		
	my $name;
	my $value;
	
	my $projDetails = "${tempInstallDir}${g_sep}projDetails.txt";
	
	unless (-e $projDetails) 
	{
		print "projDetails.txt Doesn't Exist! in current dir, please check your zip file \n";
		exit;
	} 
	open (FILE, $projDetails);
	while (<FILE>) 
	{
		
		chomp;
		($name, $value) = split("=");
		print "Name: $name\n";
		print "Value: $value\n";
		print "---------\n";
		
		if( 0 == $i)
		{
			$g_project = $value;
		}
		elsif ( 1 == $i)
		{
			$logicName = $value;
		}
		else
		{
		;
		}
		$i++;
	}
	close (FILE);
	my $tempDir = "${tempInstallDir}${g_sep}${g_project}";
	my $destDir = $projectDirectory.$g_sep.$g_project;
   
############################################################################
	if ( doesDirExist ($projectDirectory,$g_project) == -1)
	{
		# update lipiengine.cfg with new addon	

		#print "doesDirExist\n";		
		&updateLipiengineConfig($projectDirectory, $logicName);	

			#print "tempDir = $tempDir\n";
			#print "destDir = $destDir\n";
		&CopyDirectory($tempDir, $destDir);		
	}
	else
	{
		#print "lse doesDirExist\n";
		
		my $console = Term::ReadLine->new("addon");
		my $overwrite = $console->get_reply( prompt => "Project already exists in projects directory, do you want to overwrite or exit (y/n)?",
											choices => [qw|y n|],
											default => n,
											);
		
		if($overwrite eq "y")
		{
			# update lipiengine.cfg with new addon			
			&updateLipiengineConfig($projectDirectory, $logicName);
			&CopyDirectory($tempDir, $destDir);			
		}
		else
		{
			foreach $dir ($tempDir) 
			{
				rmtree($dir);
			}
			exit;
		}
	}
	#print "after copy \n";
	
	foreach $dir ($tempDir) 
	{
		rmtree($dir);
	}
	
	if (-e $projDetails) 
	{
		unlink($projDetails);		
	}
}

sub doesDirExist {
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

sub extractAddon {
		
	$CurrentDir = getcwd;
	$destZip = "${CurrentDir}${g_sep}${g_zipFile}"; # zip file name
	print " zip file $destZip \n";
	
	unless (-e $destZip) 
	{
		print "zip file can not be found in $CurrentDir \n";
		exit;
	}
		
	my $zip = Archive::Zip->new();
	unless ( $zip->read( $destZip ) == AZ_OK ) 
	{ 
		print "unable to read $destZip \n";
		die 'read error';
	
	}
	
	my $stat = $zip->extractMemberWithoutPaths("projDetails.txt");
	die "failed to extract file $name \n" if $stat != AZ_OK;


	# (Linux)         root  dest
	unless($zip->extractTree() == AZ_OK)
	{ 
		print "failed to extract $g_project in  $ExtractionPath \n";
		die 'extract error';
		
	}
}

sub updateLipiengineConfig 
{
	local ($projectsLocation) = $_[0];
	local ($logicalName) = $_[1];
	$lipiengineConfig = "${projectsLocation}${g_sep}${g_Lipiengine}";
	
	
	unless (-e $lipiengineConfig) 
	{
		print "config file can not be found in $projectsLocation \n";
		exit;
	}
	else
	{
		open(CONFIG, ">>$lipiengineConfig") || die("failed to open $lipiengineConfig !");
		my @line = <CONFIG>;
		for (@lines) 
		{
			if ($_ =~ /$logicalName/) 
			{
				print "$logicalName already exists please chose a different name.\n";
				exit;
			}
		}
		print CONFIG "\n";
		print CONFIG "${logicalName}=${g_project}(default)";
		close(CONFIG);
	}
}

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
		#print "toSubDir = $toSubDir \n";
		#print "fromSubDir = $fromSubDir \n";
		if ( -d $fromSubDir )
		{
		   $command = "$g_copyDirectory \"$fromSubDir\" \"$toSubDir\"";
		   #print "Copying command $command";
		   system($command);
		}
		#print "Copying command 3 \n";
	}
    close DIR_HANDLE;
}