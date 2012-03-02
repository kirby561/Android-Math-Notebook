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
use strict;
use Getopt::Long;
use File::Find;
use File::Path;
my $Major_Version= 4;
my $Minor_Version= 0;
my $Bugfix_Version= 0;
my $g_ostype =  $^O ; # Get OS type 


###############################################################################
#
# Variables for Command line arguments
#
###############################################################################

my $inDataRoot = "" ;    # The root directory of the dataset
my $outDataRoot = "" ; # The root directory of the images
my $lipiRoot   = "";      # The Path directory of the lipiRoot
my $inFileExt  = "";      # The input file extension
my $version = "";
my $helpRequired = "";
my @dirInfo = ();
my @fileInfo = ();
my $imageSize = "";      # The size of the images to be generated 

{

          # Reading Command line arguements
     if($#ARGV==-1) 
     {
          display_usage();
          exit;
     }

     # Command Line Options, see case -help for details
     GetOptions('indir=s'          => \$inDataRoot,
             'infileext=s'         => \$inFileExt,
             'imagesize=i'         => \$imageSize,
             'ver|v'               => \$version,
             'help'                => \$helpRequired,
             'outdir=s'            => \$outDataRoot,
             'lipiroot=s'               => \$lipiRoot);

     if ( $version )
     {
          print "Image Writer script Version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
          exit;
     }
     if ( $helpRequired )
     {
          display_usage();
          exit;
     }
     
     if($inDataRoot eq "" )
     {
          print "Specify input data root, using -indir option\n";
          exit;
     }    
     
     if($outDataRoot eq "" )
     {
          print "Specify output data root, using -outdir option\n";
          exit;
     }

     if($lipiRoot eq "")
     {
          $lipiRoot = $ENV{"LIPI_ROOT"};
          if($lipiRoot eq "")
          {
               print "Environment variable not set";
               exit;
          }
     }
     

     # No extension specified for the input files
     if($inFileExt eq "" )
     {
          print "No extension specified for the input file at command line, assuming default extension'.txt'\n";
          $inFileExt = "\.txt";
     }
     else
     {
          if ($inFileExt !~ /^\./)
          {
               my $tempString = "\.$inFileExt";
               $inFileExt = $tempString;
          }
     }

     # image size not specified
     if($imageSize eq "" )
     {
          print "Image size not specified at command line, assuming default image size (100)'\n";
          $imageSize = 100;
     }
     else
     {
          if ($imageSize < 1)
          {
               print "Image size can not be less than 1\n";
               exit;
          }
     }

     &dir_exists
     if !(-e $inDataRoot);
     
     &outdir_exists
     if !(-e $outDataRoot);
     
     if (-e $outDataRoot && !(-w $outDataRoot))
     {
          print " The out data root directory doesn't have write permissions";
          exit;
     }    
     
     # Replacing \ seperator with / seperator in the path
     if($inDataRoot =~ /\\/)
     {
          $inDataRoot =~ s/\\/\//g;
     }
     if($outDataRoot =~ /\\/)
     {
          $outDataRoot =~ s/\\/\//g;
     }

     # Remove the / seperator at the end of the path
     if($inDataRoot =~ /\/$/)
     {
          $inDataRoot =~ s/\/$//;
     }
     if($outDataRoot =~ /\/$/)
     {
          $outDataRoot =~ s/\/$//;
     }
     
     # Call find to recurse through the file tree and get the directory and file list info
     find(\&traverse_file_tree, $inDataRoot);

     # create the output directory tree structure
     foreach (@dirInfo)
     {
          
          &create_dir($_)
          if -d $_;
          
          push(@fileInfo,$_)
          if !(-d $_);
          
     }
     
     # Get the lipiRoot
     # Replacing \ seperator with / seperator in the path
     if($lipiRoot =~ /\\/)
     {
          $lipiRoot =~ s/\\/\//g;
     }
     

     # generate images for each file
     foreach (@fileInfo)
     {
          my $sourceFile = $_;
          my $targetFile = $_;
          if($sourceFile=~/($inFileExt)$/)
          {
               
               $targetFile =~ s/$inDataRoot/$outDataRoot/;
               $targetFile =~ s/($inFileExt)$/\.bmp/;
               
               my $command_line = "";
               
               if ($g_ostype eq "linux")
               {
                    $command_line = $lipiRoot."/bin/imgwriter";
               }
               elsif ($g_ostype eq "MSWin32")
               {
                    $command_line = $lipiRoot."/bin/imgwriter.exe";
               }
               
               &file_exists
               if !(-e $command_line);
     
               $command_line = $command_line." -show off -dim ".$imageSize;          
               $command_line = $command_line." ".$sourceFile." ".$targetFile;   

               system("$command_line");
               
          }
     }
     
}

sub traverse_file_tree 
{
     my $sourceFile = $File::Find::name; 
     push(@dirInfo,$sourceFile);
}

sub create_dir
{
     my $sourceFile = shift @_;
     my $CREATEDIR = $sourceFile;
     $CREATEDIR =~ s/$inDataRoot/$outDataRoot/;
     #mkdir $CREATEDIR;
     mkpath($CREATEDIR);
}

sub dir_exists
{
     print "The input directory does not exist!!! \n"; 
     exit;
}

sub outdir_exists
{
     print "The output directory does not exist!!! \n"; 
     print "Creating the output directory\n";
}

sub file_exists
{
     print "The exe file  does not exist!!! \n"; 
     exit;
}

sub display_usage
{
     print "\n\nUsage : imagewriter.pl\n";
     print "\nimagewriter.pl\n";
     print "\n\t-indir  <root of the dataset>\n";
     print "\n\t-outdir <output directory>\n";
     print "\n\t[-infileext <extension of the input file>]\n";
     print "\n\t[-imagesize <Size of the output image in Pixels>]\n";
     print "\n\t[-lipiroot <path of the lipiroot>]\n";
     print "\n\t[-ver or -v (displays the version)]\n";
     print "\n\t[-help (displays this list)]\n\n";
}
