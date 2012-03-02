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

#!/usr/bin/perl

use Getopt::Long;
use File::Basename;

##########################################################################################
# Global variables
##########################################################################################
my $osType =  $^O;
my $inputFilePath = "";
my $outputFilePath = "";
my $outputFileName = "";
my $version = "";
my $helpRequired = "";
my $majorVersion = "4";
my $minorVersion = "0";
my $bugfixVersion = "0";
my $separator = "";

#####Check if user has supplied command line arguments

if ( $#ARGV == -1 )
{
     displayUsage();
     exit;
}

GetOptions('input=s'     => \$inputFilePath,
           'ver|v'  => \$version,
           'help'   => \$helpRequired,
            );

##### Check command line options #####

if ( $version )
{
    print "validateListFile.pl script version : $majorVersion.$minorVersion.$bugfixVersion\n";
    exit;
}

if ( $helpRequired )
{
    displayUsage();
    exit;
}

if ( $inputFilePath eq "")
{
     print "Input file path not specified\n";
     exit;
}

if ($osType eq "linux") 
{
    $separator = "/";
}
else
{
    $separator = "\\";
}

#####################################################################################################
# Construct the path of output file
#####################################################################################################

my $filename = basename($inputFilePath, ".txt");
my $dirname = dirname($inputFilePath);
$outputFilePath = $dirname.$separator.$filename."_new.txt";

#####################################################################################################
# Sort the input file and write it to output file
#####################################################################################################

open(INPUT_LIST_FILE, "<$inputFilePath") || die " Couldnot open the input list file\n\n";
open(OUTPUT_LIST_FILE, ">$outputFilePath") || die " Couldnot open the file for writing\n\n";

@oldListFile = <INPUT_LIST_FILE>;

@newListFile = sort {
     ($a =~ / (\d+)/)[0] <=> ($b =~ / (\d+)/)[0]
    } @oldListFile;

print OUTPUT_LIST_FILE "@newListFile";

close INPUT_LIST_FILE;
close OUTPUT_LIST_FILE;

#####################################################################################################
# display usage
#####################################################################################################
sub displayUsage
{
     print "*" x 80;
     print "\n*";
     print "\n* Usage: ";
     print "\n*";
     print "\n* perl validateListFile.pl ";
     print "\n* \t-input input list file";
     print "\n* \t-v|ver";
     print "\n* \t-help";
     print "\n*";
     print "\n*";
     print "*" x 80;
     print "\n";
}
