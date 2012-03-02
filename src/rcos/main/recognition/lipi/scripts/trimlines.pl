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
use File::Find;
use File::Path;

 
my $Major_Version= 4;
my $Minor_Version= 0;
my $Bugfix_Version= 0;
my $version = "";
my $helpRequired = "";
my $DATAROOT = "" ;      #The root directory of the dataset

{

          # Reading Command line arguements
     if($#ARGV==-1) 
     {
          display_usage();
          exit;
     }
     GetOptions('ver|v'   => \$version,
             'help'    => \$helpRequired,
             'indir=s'   => \$DATAROOT);

          
     # Command Line Options, see case -help for details
     if($version)
     {
          print "Trimlines script version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
          exit;
     }

     if($helpRequired)
     {
          display_usage();
          exit;
     }

     &dir_exists
     if !(-e $DATAROOT);
     
     # Replacing \ seperator with / seperator in the path
     if($DATAROOT =~ /\\/)
     {
          $DATAROOT =~ s/\\/\//g;
     }
     
     # Call find to recurse through the file tree
     find(\&traverse_file_tree, $DATAROOT);

}

sub traverse_file_tree 
{
     my $SOURCEFILE = $File::Find::name; 
     my $SOURCEDIR = $File::Find::dir;
     
     if($SOURCEFILE=~/(\.txt|\.new)$/)
     {
          # It is could be ink file further check it with header info. For the time being assume it is ink file.
               system("dos2unix $SOURCEFILE");
     }
}


sub dir_exists
{
     print "Oops... the input directory does not exist!!! \n"; 
     exit;
}

sub display_usage
{
     print "\n\nUsage : trimlines.pl\n";
     print "\ntrimlines.pl\n";
     print "\n\t-indir <root of input directory>\n";
     print "\n\t[-ver or -v (displays the version)]\n";
     print "\n\t[-help (displays this list)]\n\n";
}


