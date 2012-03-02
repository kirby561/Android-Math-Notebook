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
use File::Find;
use File::Path;
use Getopt::Long;
my $Major_Version= 4;
my $Minor_Version= 0;
my $Bugfix_Version= 0;
# Extracts data from files with .txt, or .dat, or .new extensions only. 
my $IN_DATAROOT = "" ;   #The root directory of the dataset
my $OUT_DATAROOT = "" ; #Config file containing the regular expressions
my $hLevel = "";    #Hierarchy level 
my @dirInfo = ();
my @fileInfo = ();
my $version = "";
my $helpRequired = "";

{
     if($#ARGV==-1) 
     {
          display_usage();
          exit;
     }

     GetOptions('ver|v'      => \$version,
             'help'       => \$helpRequired,
             'indir=s'    => \$IN_DATAROOT,
             'outdir=s'     => \$OUT_DATAROOT,
             'hlevel=s'  => \$hLevel);

     if($version)
     {
          print "Extract Handwriting data script Version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
          exit;
     }

     if($helpRequired)
     {
          display_usage();
          exit;
     }
     
     # Verifying all command line options 
     if($hLevel eq "")
     {
          print "Error : Input hierarchy level using -hlevel option\n";
          exit;
     }
     &dir_exists
     if !(-e $IN_DATAROOT);
     
     # Replacing \ seperator with / seperator in the path
     if($IN_DATAROOT =~ /\\/)
     {
          $IN_DATAROOT =~ s/\\/\//g;
     }
     if($OUT_DATAROOT =~ /\\/)
     {
          $OUT_DATAROOT =~ s/\\/\//g;
     }

     # Remove the / seperator at the end of the path
     if($IN_DATAROOT =~ /\/$/)
     {
          $IN_DATAROOT =~ s/\/$//;
     }
     if($OUT_DATAROOT =~ /\/$/)
     {
          $OUT_DATAROOT =~ s/\/$//;
     }
     
     
     # Call find to recurse through the file tree and get the directory and file list info
     find(\&traverse_file_tree, $IN_DATAROOT);

     # create the output directory tree structure
     foreach (@dirInfo)
     {
          &create_dir($_)
          if -d $_;
          
          push(@fileInfo,$_)
          if !(-d $_);
     }

     
     #split files
     foreach (@fileInfo)
     {
          #print "$_ \n";
          my $SOURCEFILE = $_;
          my $TARGETDIR = $_;
          if($TARGETDIR=~/(\.txt|\.new|\.dat)$/)
          {
               
               if($TARGETDIR =~ s/\/(\w+)\.(txt|new|dat)$//)
               {
                    $TARGETDIR =~ s/$IN_DATAROOT/$OUT_DATAROOT/;
                    # print "$TARGETDIR\n";
                    &split_file($SOURCEFILE, $TARGETDIR);
               }
          }
     }
     
}

sub traverse_file_tree 
{
     my $SOURCEFILE = $File::Find::name; 
     push(@dirInfo,$SOURCEFILE);
}



sub split_file
{
     my $SOURCEFILE = shift @_;
     my $TARGETDIR = shift @_;
     
     #opening the Source File in read mode
     unless(open _READ, "<$SOURCEFILE")
     {
          die "cannot open $SOURCEFILE file \n";
     }
     
     my $strokeNumber = -1;
     my @writerInfo = ();
     my @segmentInfo = ();
     my @ink = ();
     my $numSegments = 0;
     my $flag;
     my $point;
     my $i;
     my $j;
     my $k;
     my $l;
     # Reading the source file and saving Writer Info, segment Info (annotation) and ink 
     while(<_READ>)
     {
          #$_ = $_."\n"; 
          if(/^\.SEGMENT/)
          {
               if(/^\.SEGMENT\s+($hLevel)/)
               {
                    $segmentInfo[$numSegments] = $_;
                    $numSegments++;
               }
          }
          elsif(/PEN_DOWN/)
          {
               $flag = 1;
               $point = 0; 
               $strokeNumber++;
          }
          elsif(/^(\d+)\s+(\d+)\s+(\d+)/ && $flag == 1)
          {
               $ink[$strokeNumber][$point++] = $1." ".$2." ".$3."\n";
          }
          elsif(/PEN_UP/)
          {
               $flag = 0;
          }
          elsif(/^\.HIERARCHY/)
          {
               # skipping the .hierarchy line, the new file has a different hierarchy;
          }
          else
          {
               push(@writerInfo,$_);
          }
     }# end of while<_READ>
     
     # close the input file
     close _READ;
     
     # Creating split files and saving data
     # print "segment info size is  $#segmentInfo\n";
          
     my %unitID; # map between frequency of the segment and it's id
     
     if($#segmentInfo == -1)
     {
          print "Warning : No matching segment info found in the file $SOURCEFILE\n";
     }
     else
     {
          print "Extracting data from file $SOURCEFILE\n";
     }
     
     for($i = 0; $i <= $#segmentInfo; $i++)
     {    
          
          # get the strokes of the segment
          my @spaceSplit = split /\s/, $segmentInfo[$i] ;
          my @commaSplit = split /,/, $spaceSplit[2];
          my @strokeInfo = ();
          my @numStrokeInfo = ();
          my @pointInfo = ();
          for($j = 0; $j <= $#commaSplit; $j++)
          {
               my @hypenSplit = split /-/, $commaSplit[$j];
               if($hypenSplit[0] =~ /:/)
               {
                    my @colonSplit1 = split /:/, $hypenSplit[0];
                    push @strokeInfo, $colonSplit1[0];
                    push @pointInfo, $colonSplit1[1];
                    my @colonSplit2 = split /:/, $hypenSplit[1];
                    push @strokeInfo, $colonSplit2[0];
                    push @pointInfo, $colonSplit2[1];
                    push @numStrokeInfo, $colonSplit1[0]..$colonSplit2[0];                                    
               }
               else
               {
                    if($#hypenSplit==0)
                    {
                         push @strokeInfo, $hypenSplit[0];
                    }
                    else
                    {
                         push @strokeInfo, $hypenSplit[0]..$hypenSplit[1];
                    }
               }
          }
          
          if($spaceSplit[2] =~ /:/)
          {
               if($strokeInfo[$#strokeInfo] == $strokeInfo[$#strokeInfo-1])
               {
                    my $temp = $pointInfo[$#pointInfo]-$pointInfo[$#pointInfo-1];
                    $spaceSplit[2]="0:0-$#numStrokeInfo:$temp";
               }
               else
               {
                    $spaceSplit[2]="0:0-$#numStrokeInfo:$pointInfo[$#pointInfo]";
               }
          }
          else
          {
               
               #updating $spaceSplit[2] value, to write the new .segment line
               if($#strokeInfo==0)
               {
                    $spaceSplit[2]="0";
               }
               else
               {
                    $spaceSplit[2]="0-$#strokeInfo"; 
               }
          }
          
          # get the symbol id
          $segmentInfo[$i] =~ /\"(\w+)\"/;
               
          my $symbolID = $1;
          
     
          # updating the unit id, i.e the frequency of symbol id
          if(defined($unitID{$symbolID}))
          {
               $unitID{$symbolID}++;
          }
          else
          {
               $unitID{$symbolID} = 1;
          }
                    
          # create the target file name and open the target file to write the segment
          my $TARGETFILE = "";
          if($SOURCEFILE =~ /([0-9]+)t([0-9]+)\.(txt|new|dat)$/) 
          {
               my $dataID = $1;
               my $trialID = $2;
               $TARGETFILE = sprintf("d%dt%d_i%dsym_%s.txt", $dataID,$trialID,($unitID{$symbolID}),$symbolID);
          }
          elsif($SOURCEFILE =~ /\/(\w+)\.(txt|new|dat)$/)
          {
               my $dataString = $1;
               $TARGETFILE = sprintf("%s_i%dsym_%s.txt",$dataString, ($unitID{$symbolID}),$symbolID);
          }
          $TARGETFILE = $TARGETDIR."/".$TARGETFILE;
          unless(open _WRITE, ">$TARGETFILE")
          {
               die "cannot open $TARGETFILE file \n";
          }
          
          
          # Printing the writer Info & Meta data
          for($j = 0; $j <= $#writerInfo; $j++)
          {
               print _WRITE "$writerInfo[$j]";
          }
          #print _WRITE "\n";
               
          #write the hierarchy information
          print _WRITE ".HIERARCHY $hLevel";
          print _WRITE "\n";
          
          #Printing the Annotation
          print _WRITE "@spaceSplit\n";
          print _WRITE "\n";  
          
          if($spaceSplit[2] =~ /:/)
          {
               for($k = 0; $k <= $#strokeInfo; $k=$k+2)
               {
                    for($j = $strokeInfo[$k]; $j <= $strokeInfo[$k+1]; $j++)
                    {
                         print _WRITE ".PEN_DOWN\n";
                         if($j == $strokeInfo[$k] || $j == $strokeInfo[$k+1])
                         {
                              if($strokeInfo[$k] != $strokeInfo[$k+1])
                              {
                                   if($j == $strokeInfo[$k])
                                   {
                                        my @stroke = @{$ink[$j]};
                                        for($l = $pointInfo[$k]; $l <= $#stroke; $l++)
                                        {
                                             print _WRITE "$ink[$j][$l]";
                                        }
                                   }
                                   else
                                   {
                                        for($l = 0; $l <= $pointInfo[$k+1]; $l++)
                                        {
                                             print _WRITE "$ink[$j][$l]";
                                        }
                                   }
                              }
                              else
                              {
                                   for($l = $pointInfo[$k]; $l <= $pointInfo[$k+1]; $l++)
                                   {
                                        print _WRITE "$ink[$j][$l]";
                                   }
                              }
                         }
                         else
                         {
                              my @stroke = @{$ink[$j]};
                              print _WRITE "@stroke";
                         }    
                         print _WRITE ".PEN_UP\n";
                    }
               }    
          }
          else
          {
               #Printing the ink coords of the corresponding stroke
               for($j = 0; $j <= $#strokeInfo; $j++)
               {
                    print _WRITE ".PEN_DOWN\n";
                    my @stroke = @{$ink[$strokeInfo[$j]]};
                    print _WRITE "@stroke";
                    print _WRITE ".PEN_UP\n";
               }
     
               #Closing the target File
               close _WRITE;
          }
     }

}                        

sub create_dir
{
     my $SOURCEFILE = shift @_;
     my $CREATEDIR = $SOURCEFILE;
     $CREATEDIR =~ s/$IN_DATAROOT/$OUT_DATAROOT/;
     #mkdir $CREATEDIR;
     mkpath($CREATEDIR);
}

sub dir_exists
{
     print "Oops... the input directory does not exist!!! \n"; 
     exit;
}

sub display_usage
{
     print "\n\nUsage : extracthwdata.pl\n";
     print "\nextracthwdata.pl\n";
     print "\n\t-indir <root of input directory>\n";
     print "\n\t-outdir <root of output directory>\n";
     print "\n\t-hlevel <Hierarchy level to be extracted (e.g. CHARACTER)>\n";
     print "\n\t[-ver or -v (displays the version)]\n";
     print "\n\t[-help (displays this list)]\n\n";     
}
