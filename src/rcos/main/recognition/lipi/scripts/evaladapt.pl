###########################################################
# Copyright (c) 2006 Hewlett-Packard Development Company, L.P.
# Permission is hereby granted, free of charge, to any person obtaining a copy of 
# this software and associated documentation files (the "Software"), to deal in 
# the Software without restriction, including without limitation the rights to use, 
# copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
# Software, and to permit persons to whom the Software is furnished to do so, 
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial 
# portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
#############################################################

#!/bin/perl -w
use Getopt::Long;
use strict;
use Cwd;
use Cwd 'abs_path'; 

################################################################
## VERSION INFORMATION
################################################################

my $Major_Version = 4;        
my $Minor_Version = 0;   
my $Bugfix_Version = 0; 

################################################################
# command line arguments
################################################################
my $version      = "";
my $helpRequired = "";
my $lipiRoot     = "";
my $inputFile    = "";
my $binSize      = "" ;  # binsize for eval
my $overlap      = "" ;
my $finalBin     = "" ;
my $outDir       = "";

################################################################
# GLOBAL VARIABLES
################################################################
my $operatingSystem       =  $^O ;
my $separator             = "";
my $comment               = "%";
my $evalScriptPath        = "";
my $filteredFilePath      = "runshaperec.out_filtered";
my $count                 = 0;
my $filteredFileSize      = 0;
my $filteredFileArray     = ();
my @finalBinArray         = ();
my $finalBinCount         = 0;
my $offset                = 0;
my $spliceLength          = 0;
my $evalDirectory         = "eval";
my $runshaperecOutputFile = "";
my $evalInputFile         = "evalInputFile.txt"; 
my $evalOutputFile        = "evalOutputFile.txt"; 
my @performanceArray      = ();
my $resultFile            = "";
my $presentWorkingDir     = "" ;

if($#ARGV == -1)
{
    displayUsage();
    exit;
}

# Command Line Options, see case -help for details
GetOptions('ver|v'           => \$version,
           'help'            => \$helpRequired,
           'input=s'         => \$inputFile,
           'outdir=s'        => \$outDir,
           'binsize=i'       => \$binSize,
           'overlap=i'       => \$overlap,
           'finalbin=i'      => \$finalBin,
           'resultfile=s'    => \$resultFile,
           'lipiroot=s'      => \$lipiRoot);

validateCmdLineArgs();

################################################################
# ASSIGNING VALUES TO GLOBAL VARIABLES
################################################################


#####linux related compiler option has to be filled properly#####
if ($operatingSystem eq "linux")
{
   $separator = "/";
}
else
{
   $separator = "\\";
}

$evalScriptPath = "$lipiRoot${separator}scripts${separator}eval.pl";
$spliceLength   = $binSize - $overlap;


($presentWorkingDir = getcwd ) =~  s#/#$separator#;
chomp $presentWorkingDir;

$runshaperecOutputFile = abs_path($inputFile);

#create eval directory
if ( -e $outDir )
{
    print "The output directory \"$outDir\" exists. Please specify a different output directory\n\n";
    exit;
}
else
{
    mkdir $outDir;
}

chdir $outDir;

filterCommentedLines($filteredFilePath);

open TEMP, "<$filteredFilePath " || die " Unable to open $filteredFilePath for reading\n\n";
    
my @filteredFileArray = <TEMP>;
$filteredFileSize = @filteredFileArray;

print "file length = $filteredFileSize\n";
    
    #get final bin
    
    my $fileIndex = $#filteredFileArray - $finalBin +1;
    
    while($fileIndex <= $#filteredFileArray)
    {
        $finalBinArray[$finalBinCount] = $filteredFileArray[$fileIndex];
        
        $finalBinCount++;
        $fileIndex++;
    }
    
    while ($binSize <= $filteredFileSize)
    {
        my @tempArray = splice(@filteredFileArray,$offset,$spliceLength);
        my $overlapIndex = 0;
        my $overlapStartIndex = $offset;
        $filteredFileSize = @filteredFileArray;
        
        while ($overlapIndex < $overlap)
        {
            push (@tempArray,$filteredFileArray[$overlapStartIndex++] );
            $overlapIndex++;
        }
        
        writeFileForEval($count, @tempArray);
        
        @tempArray = ();
        $count++;
    }
    
    writeFileForEval($count,@finalBinArray);

    writePerformance();


#############################################################
#
# Function name : validateCmdLineArgs
# Reponsibility   : validates commad line arguments
# Arguments : None
#
#############################################################
sub validateCmdLineArgs
{
    if( $version )
    {
        print "LipiTk Evaluation Tool Version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
        exit;
    }

    if($helpRequired)
    {
        displayUsage();
        exit;
    }

    if($lipiRoot eq "")
    {
        $lipiRoot = $ENV{'LIPI_ROOT'};

     if($lipiRoot eq "")
     {
            print "Error: LIPI_ROOT env variable is not set\n";
            exit;
        }
    }

    if($inputFile eq "")
    {
        print "Error: Input file not specified\n";
        exit;
    }

    if ( $outDir eq "" )
    {
        print "\n\nWarning: Output directory not specified. The results will be written to default directory: $evalDirectory\n\n";
     $outDir = $evalDirectory;
    }

    if ( $resultFile eq "" )
    {
        print "\n\nWarning: Result file not specified. The results will be written to default file: resultfile.txt\n\n";
     $resultFile = "resultfile.txt";

    }

    if ($finalBin == 0)
    {
        print "Error: final bin must be non zero\n\n";
        exit;
    }

    if ($binSize == 0)
    {
        print "Error: bin size must be non zero\n\n";
        exit;
    }

    if ($binSize <= $overlap)
    {
        print "Error: Binsize must be greater than overlap\n\n";
        exit;
    }

}

sub getAccuracy
{
    my $inputFile = shift;

    my $key = "";
    my $value = "";

    open(INFILE,"<$inputFile" ) || warn "getAccuracy: Failed to open input file $inputFile\n\n";

    while (<INFILE>)
    {
        chomp $_;

        if ( $_ =~ m/accuracy/i)
        {
            ($key,$value) = split(/=/,$_, 2);
         $value =~ s/^\s+|\s+$//g;
        }
    }

    close INFILE;
    return $value;
}

#############################################################
#
# Function name : writePerformance
# Reponsibility   : Writes the performance array to result file
# Arguments : None
#
#############################################################
sub writePerformance
{
    open INFILE, ">$resultFile" || warn "could not open result file for writing\n\n";

    my $line = "";
    my $result = "";

    foreach $result (@performanceArray)
    {
     $line .= "$result,";
    }    

    chop $line;

    print INFILE "$line\n";

    close INFILE;

}

#############################################################
#
# Function name   : filterCommentedLines
# Reponsibility   : Removes the commented lines from inputfile, runshaperec.out
# Arguments : None
#
#############################################################
sub filterCommentedLines
{
    my $outfilePath = shift;
    my $offset = 0;
    my @overlapLines = ();
    my @evalLines = ();
    
    open HANDLE, "<$runshaperecOutputFile" || die "could not open $runshaperecOutputFile \n\n";
    
    open TEMP, ">$outfilePath " || die " Unable to open $outfilePath for writing\n\n";
    
    while(<HANDLE>)
    {
        my $line = $_;
        chomp $line;
        
        if ($line !~ /^$comment/)
        {
            print TEMP "$line\n";  
        }
    }
    
    close HANDLE;
    close TEMP;
}

sub writeFileForEval
{
    my $count         = shift;
    my @linesArray        = @_;
    my $line              = "";

    my $dirName = "evalRun_$count";
    mkdir $dirName;
    chdir $dirName;

    open EVAL, ">$evalInputFile" || die "Could open $evalInputFile for writing\n\n";
    
    foreach $line (@linesArray)
    {
        print EVAL "$line" ;
    }
    
    close EVAL;

    my $command = "perl $evalScriptPath -input $evalInputFile -output $evalOutputFile ";
     
    my $exitStatus = system($command);

    if ($exitStatus != 0)
    {
      print "Error in eval.pl. Aborting... \n\n";
      exit;
    }

    $performanceArray[$count] = getAccuracy($evalOutputFile);

    chdir("..");
}

sub displayUsage
{
    print "\n\nUsage : evalAdapt.pl\n";
    print "\nevalAdapt.pl\n";
    print "\n\t-input <input file>\n";
    print "\n\t-outdir <output directory>\n";
    print "\n\t-resultfile <output text file to which the computed accuracies will be written>\n";
    print "\n\t-binsize <the number of samples per bin on which accuracy has to be computed>\n";
    print "\n\t-overlap <overlap value for each bin>\n";
    print "\n\t-finalbin <the size of the final bin on which final accuracy has to be calculated>\n";
    print "\n\t[-lipiroot <lipi root path>]\n";
    print "\n\t[-ver or -v (Displays Version of the Tool)]\n";
    print "\n";
}

