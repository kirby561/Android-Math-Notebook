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
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#######################################################################################

#!/bin/perl -w
use Getopt::Long;
use List::Util 'shuffle';
use strict;
my $Major_Version= 4;
my $Minor_Version= 0;
my $Bugfix_Version= 0;
my $version = "";
my $helpRequired = "";

{
     my $DATAROOT   = ""; #The root directory of the dataset
     my $CFGMAP     = ""; #Config file containing the regular expressions
     my $OUTFILE    = ""; #Full path of the output file
     my $adapt      = ""; #Flag for randomizing the samples for adaptation evalutaion
     my $prototypes  = ""; #Number of prototypes per class to be used in the evaluation of adaptation

     # Reading Command line arguements

     if ($#ARGV == -1 )
     {
          display_usage();
          exit;
     }

     GetOptions('indir=s'      => \$DATAROOT,
             'ver|v'        => \$version,
             'help'         => \$helpRequired,
             'output=s'     => \$OUTFILE,
             'config=s'     => \$CFGMAP,
             'adapt'        => \$adapt,
             'prototypes=i' => \$prototypes);

     if($version)
     {
          print "List Files script Version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
          exit;
     }

     if($helpRequired)
     {
          display_usage();
          exit;
     }

     if ($DATAROOT ne "")
     {
          # replacing \ with / supported in WINDOWS and LINUX
          if($DATAROOT =~ /\\/)
          {
               $DATAROOT =~ s/\\/\//g;
          }
     }
     else
     {
          print "Error : Input directory missing. Enter the input directory using -indir option\n";
          display_usage();
          exit;
     }

     # Verifying all command line options
     if($CFGMAP eq ""){
          print "Error : Missing map file. Input map file using -config option\n";
          display_usage();
          exit;
     }
     if($OUTFILE eq ""){
          print "Error : Output file missing. Input output file using -output option\n";
          display_usage();
          exit;
     }

     # Open the config file and output file
     # and throw an error if not valid
     open READ,"<".$CFGMAP or die ("Cannot Open config file : $CFGMAP : $!");
     open OUT, ">".$OUTFILE or die ("Cannot Open output file : $OUTFILE: $!");


     my @allfiles;
     my $file;
     my $numberofclasses=-1;
     while( <READ> )
     {
          my ($ID, $regexp);
          my @files;



               s/#.*//;            # ignore comments by erasing them
               next if /^(\s)*$/;  # skip blank lines
               chomp;              # remove trailing newline characters
          ($ID,$regexp) = split; # split into ID and regular expression
          if(!($DATAROOT eq ""))# find if config file has relative path
          {
               if($DATAROOT=~/\/$/)
               {
                    $regexp = $DATAROOT.$regexp;
               }
               else
               {
                    $regexp = $DATAROOT."/".$regexp;
               }
          }
          @files = glob "$regexp";
          print "$regexp $ID \n";
          if(@files == 0)
          {
               warn("No Files corresponding to ID $ID (the regular expession is $regexp)")
          }
          else
          {
               foreach $file (@files)
               {
                    if($numberofclasses<=$ID)
                    {
                         $numberofclasses=$ID+1;
                    }
                    $allfiles[$#allfiles+1] = "$file $ID \n";
               }
          }

        }

        if($adapt)
        {
          my @prototypestofill;
          my ($current_id,$current_prototype,$current_sample);
          my ($numberofsamples,$count) ;
                my @prototypeslist;

                @allfiles=shuffle(@allfiles);

          $#prototypestofill=$numberofclasses;
                $#prototypeslist=$prototypes*$numberofclasses;
                $numberofsamples   =$#allfiles;



          for($count=0;$count<$numberofclasses;$count++)
          {
               $prototypestofill[$count]=$prototypes;
          }

          while(sum(@prototypestofill)>0)
          {
                 $current_prototype = int(rand($numberofsamples));
                       $current_sample=$allfiles[$current_prototype];
                       $current_id= find_class($current_sample);

                       if(($current_sample ne "")&&($prototypestofill[$current_id]>0))
                       {
                           $prototypestofill[$current_id]=$prototypestofill[$current_id]-1;
                  $current_sample = $allfiles[$current_prototype];
                           $current_sample =~ s/^\s+|\s+$//g;
                           chomp($current_sample);
                         
                  if ( $current_sample !~ m/#/ )
                  {
                      $current_sample = $current_sample . " #\n";
                               $allfiles[$current_prototype]=$current_sample ;
                  }
                       }
          }
                print("\nNumber of classes = $numberofclasses\n");
          }


        foreach $file (@allfiles)
     {
          print OUT $file;
     }

}

sub find_class
{
   if($_[0]=~/\s\d/)
    {
       return substr $_[0], @-[0], length($_[0])-1;
    }
}

sub sum
{
  my ($sum, $elem);
  $sum = 0;
  foreach $elem (@_)
  {
    $sum += $elem;
  }
  return($sum);
}

sub display_usage
{
     print "\n\nUsage : listfiles.pl\n";
     print "\nlistfiles.pl\n";
     print "\n\t-indir <root path of the dataset>\n";
     print "\n\t-output <output list file>\n";
     print "\n\t-config <config file containing the regular expression map>\n";
     print "\n\t[-adapt <randomizes the input file for the evaluation of adaptation>]\n";
     print "\n\t[-prototypes <number of prototypes for adaptation. To be used with -adapt>]\n";
     print "\n\t[-ver or -v (displays the version)]\n";
     print "\n\t[-help (displays this list)]\n\n";
     exit;
}