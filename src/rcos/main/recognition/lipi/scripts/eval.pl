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

use strict;
use File::Copy;
use Getopt::Long;

# global to all the subroutines inside this

my $Major_Version = 4;        
my $Minor_Version = 0;   
my $Bugfix_Version = 0;  
my $conf_file="confusion_matrix.html";  # File name for displaying the Confusion Matrix
my $perf_file="performance.html";       # File name for displaying the Performance analysis
my $out_file ;
my $PATH;                # is the abolute path where the i/p and o/p files are stored 
my $COMMON_PATH;        # the common path of all the data files
my @CONFUSION_MATRIX;              # is the 2d array that stores the confusion matrix
my @CONFUSION_MATRIX_PERCENTAGE;   # is the 2d array that stores the confusion matrix classwise percentage
my @PERFORMANCE_PERCENTAGE;        # is the 2d array that stores the performance analysis
my @CORRECTCLASS_MATRIX;      # 2d array with class_id in 1st diamention and TopNchoice as the 2nd diamention
my @NUMSAMPLES_PER_CLASS;          # 1d array with class_id as the 1st diamention 
my @NUMBER_IMAGES_MATRIX;               # 2d array which has the count of the samples in that row / col
my @CREATED_HTML_FILES;            # 1d array for storing the created file names
my @AVERAGE_PERFORMANCE;      # 1d-array containing average performance for TOPCHOICES
my @Sample_images;            # file names of images to be displayed as an indication of that class
my %HASHTABLE_CLASSMAP;            # hashtable mapping the original class_id to class_id's starting from zero
my %REVERSE_HASHTABLE_CLASSMAP;         # hashtable mapping the mapped class_id to original class_id's (i.e reverse of the 
                         #    previous hash map !!
my $TOT_NUM_SAMPLES;               # Total number of samples in the results file
my $NUM_CLASSES;              # is the counter for storing the number of classes in the test file
my $COUNT;                    # is the counter to find the number of elements in the matrix
my $TOPCHOICES;                    # Choices for printing Performance Analysis
my $TOPNCHOICES;              # Choices for printing Confusion Matrix
my $NUM_COL_PER_ROW;               # this value is to set the number of Columns per Row
my $IMAGE_EXTN;                    # this is to have a common extension
my $EXE_PATH;                 # has the command line options for MarchTool
my $PATH_q;                   # Quoted path, / replaced with // to identify that / is not the special character
my $TOPERRORS;                # No. of top errors to be printed
my $HTMLCNT;                  # No. of HTML files created
my $resultsinfile;            # No. of computed class values present in the results file
my $ENABLE_HTML;      # Enable or disable HTML output

my $version = "";
my $helpRequired = "";
my $lipiPath = "";
#$ENV{ 'LIPI_ROOT'};

sub initialize
{
     #-------Initializing the num samples array----------------------------------------------------
          
          {
               my($row); 
               for($row=0;$row < $NUM_CLASSES; ++$row)
               {
                    $NUMSAMPLES_PER_CLASS[$row] = 0;   
               }
          }

     #-------Initializing numsamples array Complete-------------------------------------------------               
     
     # ------Initializing the confusion matrix----------------------------------------------------
          
          {
               my($row,$col); 
               for($row=0;$row < $NUM_CLASSES;++$row)
               {
                    for($col=0;$col < ($NUM_CLASSES + 1);++$col)
                    {
                         $CONFUSION_MATRIX[$row][$col] = 0;
                    }
               }
          }

     # ------Initializing confusion matrix Complete-----------------------------------------------
     
     # ------Initializing the confusion matrix percentage-------------------------------------------
          
          {
               my($row,$col); 
               for($row=0;$row < $NUM_CLASSES;++$row)
               {
                    for($col=0;$col < ($NUM_CLASSES + 1);++$col)
                    {
                         $CONFUSION_MATRIX_PERCENTAGE[$row][$col] = 0;
                    }
               }
          }

     # ------Initializing confusion matrix Percentage Complete----------------------------------------   

     # ------Initializing the correct class matrix------------------------------------------------
          
          {
               my($row,$col); 
               for($row=0;$row < $NUM_CLASSES; ++$row)
               {
                    for($col=0;$col < $TOPCHOICES; ++$col)
                    {
                         $CORRECTCLASS_MATRIX[$row][$col] = 0;
                    }
               }
          }

     # ------Initializing correct class Complete--------------------------------------------------- 

     #------Initializing the NUMBER_IMAGE_MATRIX----------------------------------------------------     
     
          {
               my($row,$col); 
               for($row = 0 ; $row < $NUM_CLASSES; ++$row)
               {
                    for($col = 0;$col < $NUM_CLASSES + 1 ;++$col)
                    {
                         $NUMBER_IMAGES_MATRIX[$row][$col] = 0;
                    }
               }
          }
     
     #------Initializing the MATRICES FOR PERFORMANCE ANALYSIS---------------------------------------------------- 
          {    
               my($row,$col); 
               for($row=0;$row<$NUM_CLASSES;$row++)
               {
                    for($col=0;$col<$TOPCHOICES;$col++)
                    {
                         $PERFORMANCE_PERCENTAGE[$row][$col]=0;
                         $AVERAGE_PERFORMANCE[$col]=0;
                    }    
               }
          }
     
     #------Initialization Complete-------------------------------------------------------------------   
}


# this has the subroutine for printing the confusion matrix into the file.
# input paraameters
# 1. path of infile (i.e. file containing the filenames along with the class id. eg. pcainfile.txt)
# output parameters
#  the resultarray having the confusion matrix

sub confusion_matrix 
{
  #if(@_ != 1)
     if(@_ != 2)
     {
          print "the number of arguments should be 1 infile";
               die;
     }
     
     my($resultfile,$key,$HTMLflagEnabled);
     
     #$resultfile = shift @_; # result file
     ($resultfile, $HTMLflagEnabled) = @_;
     #print("\nFILE = $resultfile. FLAG = $HTMLflagEnabled\n\n");
     
     #if flag (HTML output flag) is true 
     if($HTMLflagEnabled == 1)
     {    
      # ----------- Creating Directories IMAGES and HTML --------------------------------------
     
      if($PATH ne "")
      {
        chdir $PATH or die " could not change into the $PATH \n";
      }
      mkdir "HTML", 0700;
          
      # -----------  Creation of Directories IMAGES and HTML Complete --------------------------
          
     
      # ------Reversing the Hashtable %HASHTABLE_CLASSMAP -----------------------------------------
     
      %REVERSE_HASHTABLE_CLASSMAP = reverse %HASHTABLE_CLASSMAP; 

      foreach $key (keys %HASHTABLE_CLASSMAP)
      {
        # creating the sub directory for the class inside HTML folder
        my $temp_path = $PATH."HTML/";
        mkdir $temp_path."/".$key, 0700;     # creating the inner class directory
    }

    # ------Reversing the Hashtable Complete -----------------------------------------------------
  }
     
     print "\nNumber of classes = $NUM_CLASSES \n";    
          
      # ------Populating the Confusion Matrix and Correct Class -> TopNChoices Matrix along with SAMPLES_PER_CLASS 
          
      #print "-Populating Confusion matrix and generating HTML files\n";
          
          
          populate_confusion_matrix($resultfile, $HTMLflagEnabled);

     #------Populating confusion matrix Complete----------------------------------------------------
     
     #------Computing Classwise percentages of confusion matrix -------------------------------------
     
          compute_confusion_matrix_percentages();
          
          compute_performance_analysis();
          performance_analysis_per_class($HTMLflagEnabled);
          
          if($HTMLflagEnabled == 1)
          {
          
      #------Computing Classwise percentages of confusion matrix Complete------------------------------  

      #-----Printing the Confusion matrix to the filename specified above------------------------------  

      # print "Enter the output filename for confusion matrix: ";
      # getting the o/p filename and appending to the $path

      print "\n-lipiPath is $lipiPath\n";
      mkdir "HTML", 0700;          # my $confusionmatrix_outputfile = <STDIN>;  
      copy($lipiPath."/scripts/lipitk.css", "HTML") or warn "Style sheet is not found $!";          
      # $confusionmatrix_outputfile = $PATH.$confusionmatrix_outputfile;   
     
      # Hardcoding the filename here

      my $confusionmatrix_outputfile = $PATH.$conf_file;    
      print_confusion_matrix($confusionmatrix_outputfile);

      #-----Printing confusion matrix Complete-----------------------------------------------------------
     
      # ------Closing the tags of HTML Files for the row/col of the conf. matrix-------------------------

      close_html_tags();
          
      #print "-Generation of HTML Files Complete\n";
          
      # ------Closing Complete---------------------------------------------------------------------------     
     
      #-----Printing the performance analysis per class matrix to the filename specified above-----------
     
      # print "Enter the output filename for performance analysis: ";
      # getting the o/p filename and appending to the $path
      # $output_file_name_performance_analysis = <STDIN>;
      # $output_file_name_performance_analysis = $PATH.$output_file_name_performance_analysis;      
     
      my $output_file_name_performance_analysis = $PATH.$perf_file;
      #compute_performance_analysis();
      print_performance_analysis_per_class($output_file_name_performance_analysis);
      #performance_analysis_per_class();
     
      #-----Printing performance analysis per class matrix Complete--------------------------------------
    }
     
}    # end of subroutine confusion_matrix


# Subroutine findpath to find the path to create HTML directories.
# It is the common of all the paths of the input samples present in the classifier results file
# ! WARNING : Modifies global variable $COMMON_PATH !

sub findpath
{
     my ($string,$path);
     $path = "";
     $string=shift @_;
     if($COMMON_PATH eq "")
     {
          $COMMON_PATH = $string;
     }
     my @spstring=split(/\//,$string);
     my @spPATH=split(/\//,$COMMON_PATH);
     for(my $i=0;$i<=$#spPATH;$i++)
     {
          if($spPATH[$i] eq $spstring[$i])
          {
               $path = $path.$spPATH[$i]."/";
          }    
     }
     $COMMON_PATH=$path;
}


# Subroutine to find the number of values in the result file, assigns a value to $resultsinfile
sub find_number_results
{
my $resultsinfileTemp;
     my @lineRead;
     my $source = shift @_;
     unless (open READ, "<$source")
     {
          die "cannot open $source file \n";
     }
     while(<READ>)
     {
          chomp;
          @lineRead = split;
          # skip this iteration of loop if it is blank line or commented line
          if(($lineRead[0]=~/^#/) or ($lineRead [0] eq ""))
          {
               next;
          }
          $resultsinfileTemp = ($#lineRead-1)/2;
          if($resultsinfile < $resultsinfileTemp)
          {
               $resultsinfile = $resultsinfileTemp;
          }
     }
     close READ;    
}

# this subroutine is for reading the input file and find the number of classes
# input paraameters
# 1. path of infile (i.e. file containing the filenames along with the class id. eg. pcainfile.txt)
# # output parameters
#  $NUM_CLASSES being populated with the corr. values.

sub find_num_classes
{
     if(@_ != 1)
     {
          print "the number of arguments should be 2: infile, hashtable ";
          die;
     }
     my $resultsinLine = 0;
     my $source = shift @_;
     %HASHTABLE_CLASSMAP=();
     %REVERSE_HASHTABLE_CLASSMAP=();
     unless (open READ, "<$source")
     {
          die "cannot open $source file \n";
     }
     my @line;           #this stores the individual lines as an array
     my $value = -1;               #this is for putting the value of hashtable from zero to (num_classes - 1)      
     my $temp;
     $NUM_CLASSES = 0;
     while(<READ>)
     {
          chomp;
          @line = split;
          # skip this iteration of loop if it is blank line or commented line
          if(($line[0]=~/^#/) or ($line[0] eq ""))
          {
               next;
          }
          if($line[0] =~ /\\/)
          {
               $line[0] =~ s/\\/\//g;
          }
          findpath($line[0],$PATH);  # Finding common path to store the HTML
          # finding the number of classes in the infile to know the matrix size
          unless (exists($HASHTABLE_CLASSMAP{$line[1]}))
          {    
               $HASHTABLE_CLASSMAP{$line[1]} = ++$value;
               $line[0]=~s/\.[a-z]+$/$IMAGE_EXTN/;     
               $Sample_images[$NUM_CLASSES] = $line[0];
               $NUM_CLASSES += 1; # increasing the class count
          }
     }
     close READ;

     # open the resultfile again to find if there are new classes in the other columns
     unless (open READ, "<$source")
     {
          die "cannot open $source file \n";
     }
     
     while(<READ>)
     {
          chomp;
          @line = split;
          if(($line[0]=~/^#/) or ($line[0] eq ""))
          {
               next;
          }
          my $lineIndex = 0;
          $resultsinLine = ($#line-1)/2;
          for(my $counter=0; $counter<$resultsinLine; $counter++)
          {
               $lineIndex=$lineIndex+2;
               if($line[$lineIndex]!=-1)
               {
                    unless (exists($HASHTABLE_CLASSMAP{$line[$lineIndex]}))
                    {
                         $HASHTABLE_CLASSMAP{$line[$lineIndex]} = ++$value;
                         $NUM_CLASSES += 1; # increasing the class count
                    }
               }
          }
     }
     close READ;
}    # end of subroutine find_num_classes


# this subroutine is for reading the output file and populating the 2dArray @CONFUSION_MATRIX
# NOTE: @CONFUSION_MATRIX is a global variable
# input paraameters
# 1. path of outfile (i.e. file containing the filenames along with the class id. with matches eg. pcaresults.txt)
# # output parameters
#  the 2dArray - @CONFUSION MATRIX being populated with the corr. values.
#   

sub populate_confusion_matrix
{
     #if(@_ != 1)
     if(@_ != 2)
     {
          print "the number of arguments should be 1: infile";
          die;
     }

     my($source,$HTMLflag);
     
     #$source = shift @_;          # source file
     ($source, $HTMLflag) = @_;
     #print("\nFILE = $source. FLAG = $HTMLflag\n\n");
     
     # reading the result file

     unless (open READ, "<$source")
     {
          die "cannot open $source \n";
     }
     
     my @line;      # stores the readline as an array of words
     my $class_id;       # this has the class id of the currently read line (i.e of the filename)
     my $counter;        # loop variable
     my $flag1;          # flag for indicating whether the match has been done or not
     my $flag2;          # flag for indicating whether the match for accuracy has been done or not
     my $index;          # stores the value index corr. to the class id in the read line
     my $file_path;      # has the path of the source ink file
     my $image_source;   # has the path of the image source
     my $img_dest_path;  # has the path of the image dest
     my $image_index;    # has the counter for the image number for the corr. html file 
     my $confidence_level;   # stores the confidence level of the choice
     my $mapped_class_id;     # stores the mapped class_id of the org class_id
     
     $COUNT = 0;
     $HTMLCNT = 0;  
     
     # Converting absolute path of sample images to relative path
     # these images are used as identifiers for their respective classes
     $PATH_q=$COMMON_PATH;    
     chomp($PATH_q);
     for(my $i=0;$i<$NUM_CLASSES;$i++)
     {
          $Sample_images[$i] =~s/$PATH_q//; # Changing absolute path to relative
     }

     while(<READ>)
     {
          chomp;         
          @line = split; # space is used as a delimiter
          if(($line[0]=~/^#/) or ($line[0] eq ""))     # leaving out commented lines in the results file
          {
               next;
          }
          if($line[0] =~ /\\/)
          {
               $line[0] =~ s/\\/\//g;
          }
          $image_source = $line[0];     
          $img_dest_path = $line[0];
          $img_dest_path=~s/\.[a-z]+$/$IMAGE_EXTN/;    
          $PATH_q=$COMMON_PATH;
          chomp($PATH_q);
          $img_dest_path =~s/$PATH_q//; # Changing absolute path to relative
          $image_source =~s/$PATH_q//;
          $image_source =~s/\\/\//g;
          
          $class_id = $line[1];
          $mapped_class_id = $HASHTABLE_CLASSMAP{$class_id};
          
          # if the $class_id is not valid reading the next line
          unless( exists( $HASHTABLE_CLASSMAP{$class_id} ) )
          {
               next;
          }    
          
          ++$NUMSAMPLES_PER_CLASS[$mapped_class_id];        
               
               
          $flag1 = 0;    # flag1 is initially set to off
          $flag2 = 0;    # flag2 is initially set to off
          
          # TOPCHOICES is variable for performance analysis computation
          for($counter = 0; ($counter < $#line) && ($counter < $TOPCHOICES); ++$counter)
          {
               
               $index = ($counter * 2) + 2;
               
               # This is for Confusion Matrix
               # first check for the flag then for topNchoices given by the user and finally the class id 
               # NOTE: currently we are setting the topNchoices to 1 !!
               
               if( (!$flag1) && ($counter < $TOPNCHOICES) && ($line[$index] == $class_id) )
               {
                    $flag1 = 1;    # match is found so setting the flag              
                    ++$CONFUSION_MATRIX[$mapped_class_id][$mapped_class_id];
                    ++$COUNT;
                    
                    # if this is the first image added to the html file we create the html file
                    if($NUMBER_IMAGES_MATRIX[$mapped_class_id][$mapped_class_id] == 0)
                    {    
                         my $img_src = $line[0];
                         # Generating the html file
                         if($HTMLflag == 1)
                         {
            create_html_file($class_id,$class_id);
                         }                        
          
                    }         
                    # adding the image to the file
                         if($HTMLflag == 1)
                         {
               $file_path = $PATH."HTML/$class_id/".$class_id."_".$class_id.".html";
              ++$NUMBER_IMAGES_MATRIX[$mapped_class_id][$mapped_class_id];
              $image_index = $NUMBER_IMAGES_MATRIX[$mapped_class_id][$mapped_class_id];
                         }
     
                    $confidence_level = $line[($index + 1)];
                    if($HTMLflag == 1)
                    {
          add_image($image_source,$img_dest_path,$file_path,$image_index,$confidence_level);
                    }
     
               }
               else
               { 
                    
                    if(($line[$index]) == -1 ) # Not classified at all !
                    {
                         ++$CONFUSION_MATRIX[$mapped_class_id][$NUM_CLASSES]; # counter for -1 
                         if($HTMLflag == 1)
                         {
            $file_path = $PATH."HTML/$class_id/".$class_id."_"."-1"."\.html";
            #if this is the first image added to the html file we create the html file
            if($NUMBER_IMAGES_MATRIX[$mapped_class_id][$NUM_CLASSES] == 0)
            {  
               create_html_file($class_id,-1);
            }                      
            ++$NUMBER_IMAGES_MATRIX[$mapped_class_id][$NUM_CLASSES];
            $image_index = $NUMBER_IMAGES_MATRIX[$mapped_class_id][$NUM_CLASSES];
                   }
                         $confidence_level = $line[($index + 1)];
                         if($HTMLflag == 1)
                         {
              add_image($image_source,$img_dest_path,$file_path,$image_index,$confidence_level);
                         }              
                    }
     
               }

               # Note this step can be combined with the above step but is given separately for readability !!
               # This is for the Accuracy Calculation
               # if the first choice is filled or topN-1 choice is true then topN choice is also true

               if( ($flag1) || ($flag2) )
               {
                    ++$CORRECTCLASS_MATRIX[$mapped_class_id][$counter];
               }
               else
               {       # if the first and topN-1 choice also fails we check the CounterTH Choice  
                    if($line[$index] == $class_id)
                    {
                         ++$CORRECTCLASS_MATRIX[$mapped_class_id][$counter];
                         $flag2 = 1; # other choices are filled by default 
                    }
               }
          }
          
          # if the match is not found !!     
          unless($flag1)
          {
               # NOTE: Currently we are having the $TOPNCHOICES to 1 !!
               ++$COUNT; 
               for( $counter =0 ; $counter < $TOPNCHOICES; ++$counter)
               {
                    my $index = ($counter * 2 ) + 2; 
                    my $var = $line[$index];
                    my $mapped_var = $HASHTABLE_CLASSMAP{$var};
                    if($var >= 0)
                    {    
                         ++$CONFUSION_MATRIX[$mapped_class_id][$mapped_var]; # increases the counter 
                         
                         if($HTMLflag == 1)
                         {
            $file_path = $PATH."HTML/$class_id/".$class_id."_".$var."\.html";
                         }                   
                         
               
                         # if this is the first image added to the html file we create the html file
                         if($NUMBER_IMAGES_MATRIX[$mapped_class_id][$mapped_var] == 0)
                         {    
                           if($HTMLflag == 1)
                           {
              create_html_file($class_id,$var);
            }
                         }
                         if($HTMLflag == 1)
                         {
            ++$NUMBER_IMAGES_MATRIX[$mapped_class_id][$mapped_var];
            $image_index = $NUMBER_IMAGES_MATRIX[$mapped_class_id][$mapped_var];
                  }
                         $confidence_level = $line[($index + 1)];
                         if($HTMLflag == 1)
                         {
             add_image($image_source,$img_dest_path,$file_path,$image_index,$confidence_level);
          }
                    }
               }              
          }
     } # end of while loop

     close READ;    
     
} # end of subroutine populate_confusion_matrix



# this subroutine prints the confusion matrix
# input parameter $confusionmatrix_outputfile
sub print_confusion_matrix
{
     if(@_ != 1)
     {
          print " There has to be 1 argument: target filename \n";
          die;
     }
     my $target = shift @_;   # Target HTML File
     my $html_source;    # variable storing the html file names
     my $temp_value;          # temporary variable to store the value
     my $sample_string; # Stores the list of file names of samples
     my @nonzero_confusions; # stores the list containing nonzero confusion pairs
     my $temp; #to store the current number of non zero confusions
     print "-Confusion Matrix stored at: $target\n";
     
     unless(open WRITE, ">$target")
     {
          die " Cannot open $target \n";
     }

          select WRITE; # printing to the file (changing the default o/p to the output file
     
     # printing the initial HTML Tags
     print "\<html\> \n";
     print "<!-- Generated by LipiTk Evaluation Tool Ver. $Major_Version.$Minor_Version -->\n";

     print "<head> \n";
     print "<LINK REL=StyleSheet HREF=\"HTML/lipitk.css\" TITLE=\"Lipi\">\n";
     print "<TITLE>LipiToolkit Evaluation Tool</TITLE> \n";
     
     # Prinitng the script to change the image dymanically
     print "<script language=\"javascript\"> \n";
     print "var str=location.search.split(\"?\")[1],link;
     var path1=str.split(\"\$\")[0];\n
     var path2=str.split(\"\$\")[1];\n";
     print "var Samples_array=new Array(";
     
     for(my $i=0;$i<$NUM_CLASSES;$i++)
     {
          $sample_string.="\"$Sample_images[$i]\",";
     }
     
     chop($sample_string);
     print "$sample_string);";
     print "function changeImage\(original\,recognised\) \n";
     print "{\n" ;
     print "       document.frm.originalImage.src=path2+Samples_array[original];";
     print "       document.frm.recognisedImage.src=path2+Samples_array[recognised]";
     print "}\n";
     
     # printing the script to popup html files depending on the value of the row & col selected from the combo box
     print "function showSamples() \n";
     print "{ \n";
     print "var row=document.frm.seloriginalclass.value;\n";
     print "var col=document.frm.selrecognizedclass.value;\n";
     print "if(col==-1)\n";
     print "{\n";
     print "col=numClasses-1;\n";
     print "}\n";
        print "var found=0;\n";
        print "for(var i=0;i<nonZeroConfusions.length;++i)\n";
        print "{\n";
     print "   var rowFound=Math.floor(nonZeroConfusions[i]/numClasses);\n";
     print "   var colFound=nonZeroConfusions[i]\%numClasses;\n";
     print "   if(row==rowFound && col==colFound)\n";
     print "   {\n";
     print "   found=1;\n";
     print "   break;\n";
     print "   }\n";
     print "}\n";
        print "if(found==1)\n";
        print "{\n";
     
     print "var path3=\"HTML/\"+document.frm.seloriginalclass.value+\"/\" \n";
     print "        var fileName=document.frm.seloriginalclass.value+\"_\"+document.frm.selrecognizedclass.value";
     print "+\".html\" \n";
     print "        path3+=fileName+\"?\"+str \n";
     print "        window.open(path3); \n";
     print "} \n";
     print "else\n";
     print "{\n";
     print "alert(\"There are no samples corresponding to this confusion\");\n";
        print "}\n";
     print "}\n";
     
     
     
     # Closing the srcipt and head tags
     print "</script>\n";
     print "</head>\n";
     
     # creating the body of the html file 
     # idea is to have a table and embedd the original class image confusion matrix table and the 
     #    recognized class image as columns
     
     print "<body>\n";
     print "<form name=frm>\n";
     # main table
     print "<table>";
     # only row of the mail table 
     print "<tr> \n";
     
     # 1st col (i.e the original class image
     print "<td> \n";    
     # inserting the Orignial Class Image 
     print "<img name=\"originalImage\" src=\"\"/> ";
     print "<br><b><font color=\"blue\">TRUE CLASS</font></b>";
     print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \n";
     print "</td> \n";
     
     # 2nd column (i.e the Confusion Matrix Table)
     print "<td> \n";
     
     # inner table 
     print "<table ALIGN = \"center\" BORDER = \"1\" CELLSPACING = \"4\" RULES = \"ALL\" > \n";
     print " <CAPTION> <H1> LipiTk Evaluation Tool  $Major_Version.$Minor_Version </H1></CAPTION> \n";
     print " <CAPTION> <H3> Confusion Matrix of the Classes </H3>  </CAPTION> \n";
     
     # this is for the alignment of the vertical Original Class label
     $temp_value = ($NUM_CLASSES + 2); 
     print "<tr> \n <td valign=\"middle\" align=\"center\"  rowspan=\"$temp_value\" bgcolor=\"lime\" > \n";
     print "<b>T<br>R<br>U<br>E<br><br>C<br>L<br>A<br>S<br>S<br>E<br>S </b> </td> \n";
     
     # this is for the horizontal Recognized class
     ++$temp_value; 
        print "<td align=\"center\"  colspan=\"$temp_value\" bgcolor=\"yellow\" > \n";
     print "<b>RECOGNIZED CLASSES </b> \n </td> \n </tr> \n";    
     
     my($row,$col); # loop variable for each row and col
     my($reverse_map_row,$reverse_map_col);  # variable to store the mapped class_id's
     
     # writing the heading for the table
     print "<tr ALIGN=\"center\" BGCOLOR=\"fuschia\" >\n";
     print "<td>";
     print " <B> CL/CL </B> ";
     print "</td> \n";
     
     my @sortedkeys = sort { $a <=> $b } keys %HASHTABLE_CLASSMAP;  
     # Sorting classes to display in order, the result file contains them out of order sometimes
     
     
     my($rowindex,$colindex); 
     for($row=0; $row < $NUM_CLASSES; ++$row)
     {
          # first Row having the class id's
          print "<td> \n";
          print " <a onclick=\"window.open";
          print "(Samples_array[$sortedkeys[$row]]);\"> ";
          print "<B>".$sortedkeys[$row]."</B>";        
          print "</a> \n";
          print "</td>\n";    
     }
     
     # this is for the last column of the 1st Row which is not classified as anything 
     print "<td BGCOLOR=\"silver\" >";
     print "<B> -1 </B>";
     print "</td> \n";
     
     print "</tr> \n";
     # Heading Complete
     
     # printing the elements of the table

     for($row=0; $row < $NUM_CLASSES; ++$row)
     {
          $rowindex = $HASHTABLE_CLASSMAP{$sortedkeys[$row]};
          # this is for the 1st column having the class_id's
     
          print "<tr align=\"center\" > \n";
          print "<td BGCOLOR=\"fuschia\" > \n";
          print " <a onclick=\"window.open";
          print "(Samples_array[$sortedkeys[$row]]);\"> ";
          print "<B>".$sortedkeys[$row]."</B>";
          print "</a> \n";
          print "</td> \n";
          
          for($col=0; $col < ($NUM_CLASSES + 1); ++$col)
          {
               if( $col == $NUM_CLASSES)
               {    
                    $sortedkeys[$col] = -1;
                    $colindex = $NUM_CLASSES;
               }
               else
               {
                    $colindex = $HASHTABLE_CLASSMAP{$sortedkeys[$col]};
               }
               
               
               $html_source = "HTML/$sortedkeys[$row]/".$sortedkeys[$row]."_".$sortedkeys[$col].".html?"; 
               # stores the link to the corr. html file
               if($col == $NUM_CLASSES) # -1 Column (i.e. not classified at all)
               {
                    print "<td BGCOLOR=\"silver\" > \n";

                    # if its a +ve value
                    if($CONFUSION_MATRIX[$row][$col] != 0) 
                    {
                         $temp_value = $CONFUSION_MATRIX_PERCENTAGE[$rowindex][$colindex];
                                   
                         # Creating the pop up link and the tool tip 
                         print "<script>link=\"<a title=";
                         printf "%5.2f",$temp_value;
                         print "%;($sortedkeys[$row],-1)";
                         print " href=$html_source\" + str + \" onmouseover=changeImage($HASHTABLE_CLASSMAP{$sortedkeys[$row]},-1); style=text-decoration:none>";    
                         print "<b>$CONFUSION_MATRIX[$rowindex][$colindex]</b>";
                         print "</a>\";\n";
                         print"document.write(link);</script>";
                    }
                    else
                    {       # has zero as the value
                         # just printing the tool tip without pop up !
                         print "<a title=\"($sortedkeys[$row],-1)\">";
                         print " &nbsp;";
                         print "</a> \n";
                    }    
               }
               else
               {    # if its not the -1 column 
                    # if its on the diagonal (i.e exact matches) 
                    if($rowindex == $colindex)
                    {
                         print "<td BGCOLOR=\"fuschia\" > \n";                       
                         # if it has +ve value
                         if($CONFUSION_MATRIX[$rowindex][$colindex] != 0) 
                         {
                              $temp_value = $CONFUSION_MATRIX_PERCENTAGE[$rowindex][$colindex];
                              $nonzero_confusions[$temp]=$rowindex*($NUM_CLASSES+1)+$colindex;
                              $temp=$temp+1;
                                                            
                              # creating the link for the pop up and the tool tip
                              print "<script>link=\"<a title=";
                              printf "%5.2f",$temp_value;
                              print "%;($sortedkeys[$row],$sortedkeys[$col])";
                              print " href=$html_source\" +str+ \" onmouseover=changeImage($HASHTABLE_CLASSMAP{$sortedkeys[$row]},$HASHTABLE_CLASSMAP{$sortedkeys[$col]}); style=text-decoration:none>";     
                              print "<b>$CONFUSION_MATRIX[$rowindex][$colindex]</b>";
                              print "</a>\";\n";
                              print"document.write(link);</script>";
                              print "</a> \n";
                         }
                         else
                         {    # has zero as the value
                              # just printing the tool tip without pop up !
                              print "<a title=\"($sortedkeys[$row],$sortedkeys[$col])\" > ";
                              print " &nbsp; ";
                              print "</a> \n";
                         }    
                    }
                    else
                    {    # not the diagonal element                  
                         
                         print "<td>";
                         # if its +ve value
                         if($CONFUSION_MATRIX[$rowindex][$colindex] != 0) 
                         {
                         #print "hash = $row,$col:";
                         #print "hash == $HASHTABLE_CLASSMAP{$sortedkeys[$row]},$HASHTABLE_CLASSMAP{$sortedkeys[$col]}--";                  
                         #print "total = $CONFUSION_MATRIX[$rowindex][$colindex]++";
                              $temp_value = $CONFUSION_MATRIX_PERCENTAGE[$rowindex][$colindex];
                              $nonzero_confusions[$temp]=$rowindex*($NUM_CLASSES+1)+$colindex;
                              $temp=$temp+1;
                              
                              
                              print "<script>link=\"<a title=";
                              printf "%5.2f",$temp_value;
                              print "%;($sortedkeys[$row],$sortedkeys[$col])";
                              print " href=$html_source\" +str+ \" onmouseover=changeImage($HASHTABLE_CLASSMAP{$sortedkeys[$row]},$HASHTABLE_CLASSMAP{$sortedkeys[$col]}); style=text-decoration:none>";     
                              print "<b>$CONFUSION_MATRIX[$rowindex][$colindex]</b>";
                              print "</a>\";\n";
                              print"document.write(link);</script>";
                              print "</a> \n";
                         }
                         else
                         {
                              # has zero as the value
                              # just printing the tool tip without pop up !
                              print "<a title=\" ($sortedkeys[$row],$sortedkeys[$col]) \" > ";
                              print " &nbsp; ";
                              print "</a> \n";
                         }
                    }    
               }    
               
               print " </td> \n";
          }
          print "</tr> \n";
     }
     
     print "</table> \n";
     
     # end of the second column of the main table
     # third and the final column of the main table for the Recognized Class image
     print "</td> \n";


     print "<td> \n";
     print "<img src=\"\" name=\"recognisedImage\"/> <br><b><font color=\"red\" > \n";
     print " &nbsp;&nbsp;&nbsp;&nbsp; \n";
     print "RECOGNIZED CLASS</font></b> \n"; 
     print "</td> \n";
     print "</tr> \n";
     print "</table> \n";

     # end of the main table
     
     # adding the combo box (or drop down menu) for selecting the original and recognized class
     
     print "<br> \n <br> \n";
     
     #having a table to have these combo boxes
     print " <table align=\"center\" > \n ";
     print "<tr> \n <td> \n <b> True Class </b> \n";
     
     # combo box for the Original Class
     print "<select align=\"center\" name=\"seloriginalclass\" > \n";
     for($row=0; $row < $NUM_CLASSES; ++$row)
     {
          print "<option value=\"$sortedkeys[$row]\" > $sortedkeys[$row]</option> \n";  
     }    
          print "</select> \n </td> \n";
          
     # combo box for the Recognized Class
     print "<td> \n <b> Recognized Class </b> \n ";
     print "<select align=\"center\" name=\"selrecognizedclass\" > \n";
     for($row=0; $row < $NUM_CLASSES; ++$row)
     {
          print "<option value=\"$sortedkeys[$row]\" > $sortedkeys[$row]</option> \n";  
     }    
     print "<option value=\"-1\" >-1</option> \n";  
          print "</select> \n";
     
     #button for the pop up of the corr. html file
     print "<input type=\"button\" value=\"Show Samples\" onclick=\"showSamples()\"/> \n";
     
     # closing this table
     print "</td> \n </tr> \n </table> \n <br> \n ";
     
     # adding a new table for the print and the performance analysis buttons
     print "<table align=\"center\"> \n <tr> \n <td> \n";
     
     # printing the print matrix and Performance analysis buttons
     print "<input type=\"button\" value=\"    Print Matrix    \" \n";
        print "onclick=\'window.print();'\" /> \n";
     print "<input type=\"button\" value=\"Performance Analysis\" onclick=\"window.open";
     $html_source = $perf_file;
     print "('$html_source');\" /> \n";
     
     # closing the tags for the table 
     print "</td> \n </tr> \n </table> \n";
     
     #closing the html tags
     print "</form> \n";
        print "<script language=\"javascript\"> \n";
     print "var nonZeroConfusions=new Array($temp); \n";
     print "var numClasses=$NUM_CLASSES+1; \n"; #+1 required to account for reject class type -1
     
     for(my $index=0;$index<$temp;++$index)
     {
          print "nonZeroConfusions[$index]=$nonzero_confusions[$index]; \n";
     }
     
     print "</script> \n";
     print "</body> \n";
     print "</html> \n";
     
     select STDOUT;  # redirecting to the standard o/p stream 
     close WRITE;     # closing the file

} # end of the print_confusion_matrix subroutine  


# Populates the arrays PERFORMANCE_PERCENTAGE and AVERAGE_PERFORMANCE
sub compute_performance_analysis
{
     $TOT_NUM_SAMPLES=0;
     for(my $row=0;$row<$NUM_CLASSES;$row++)
     {
          $TOT_NUM_SAMPLES+=$NUMSAMPLES_PER_CLASS[$row];
          for(my $col=0;$col<$TOPCHOICES;$col++)
          {
               if($NUMSAMPLES_PER_CLASS[$row] !=0)
               {
                    $PERFORMANCE_PERCENTAGE[$row][$col]=((($CORRECTCLASS_MATRIX[$row][$col])* 100) / ($NUMSAMPLES_PER_CLASS[$row]));
               }
          }    
     }
     for(my $row=0;$row<$NUM_CLASSES;$row++)
     {
          for(my $col=0;$col<$TOPCHOICES;$col++)
          {
               $AVERAGE_PERFORMANCE[$col]+=($CORRECTCLASS_MATRIX[$row][$col]*100)/$TOT_NUM_SAMPLES;
          }
     }    
}


# this subroutine prints the performance/class matrix
# input parameter $output_file_name_performance_analysis
sub print_performance_analysis_per_class
{
     if(@_ != 1)
     {
          print " There has to be 1 argument target filename \n";
          die;
     }
     
     my $target = shift @_; # destination file to store the preformance analysis matrix
     
     print "-Performance Analysis stored at: $target\n\nPlease use resultviewer.html to display the Confusion Matrix.\n";
     
     unless(open WRITE, ">$target")
     {
          die " Cannot open $target for writing \n";
     }
     
     my ($row,$col,$flag,$rowindex);
     my @sortedkeys = sort { $a <=> $b } keys %HASHTABLE_CLASSMAP;

     select WRITE; # changing the default o/p to the file $target
     
     # printing the html and the table headers
     print " <html>";
     print " <head> \n";
     print " <LINK REL=StyleSheet HREF=\"HTML/lipitk.css\" TITLE=\"Lipi\">\n";
     print " <TITLE>LipiToolkit Evaluation Tool</TITLE> \n";
     print " </head> \n";

     print " <table><tr><td>\n";
     print " <table BORDER=\"1\" CELLSPACING=\"4\" RULES=\"ALL\" > \n";
     print " <CAPTION> <H3>Performance Analysis</H3> \n" ;
     print " <CAPTION> \n";
     
     # printing the first Row (i.e Heading )
     print " <tr ALIGN=\"center\" BGCOLOR=\"fuschia\" > \n <td> <B> ClassID </B> </td> \n";
          
     for($row = 1; $row <= $TOPCHOICES; ++$row)
     {
          print " <td> <B> TOP$row </B> </td> \n"; 
     }    
     print " <td> <B> NumberOfSamples</B> </td> </tr> \n";
     
     
     # printing the elements of the confusion matrix
     for($row = 0 ; $row < $NUM_CLASSES; ++$row)
     {
          # first column with the class id's
          print "<tr ALIGN=\"center\" > \n";
          print "<td BGCOLOR=\"fuschia\" > \n";
          print "<B> $sortedkeys[$row] </B> \n";
          print "</td> \n";
          $rowindex = $HASHTABLE_CLASSMAP{$sortedkeys[$row]};
          
          $flag = 0; # clearing the flag
          
          for($col =0; $col < $TOPCHOICES; ++$col)
          {
               if($flag) # if already 100% is achieved then other cols of that row are left blank
               {
                    print "<td> &nbsp; </td> \n";
                    next; # go to the next iteration of the for loop
               }
               # if there are no samples for any class then percentage cant be calculated
               if($NUMSAMPLES_PER_CLASS[$rowindex] == 0)
               {
                    print "<td>";
                    print "NA";
                    print "</td> \n";
               }
               else
               {
                    my $value = $PERFORMANCE_PERCENTAGE[$rowindex][$col];
                    # checking for the 100% accuracy for any class if yes then flag is set 
                    if($value == 100)
                    {
                         $flag = 1;
                    }    
                    print "<td>";
                    printf " %5.2f ", $value;
               }    print "</td> \n";
                    
          }
          # last column having the number of samples of that class
          print "<td>";
          print " $NUMSAMPLES_PER_CLASS[$rowindex]";
          print "</td> \n";
          
          print "</tr> \n";
     }
     
     # printing last row containing the averages
     print "<tr align=\"center\" > \n";
     print "<td BGCOLOR=\"fuschia\" > ";
     print "<B> AVG </B>";
     print "</td> \n";
     
     for($col = 0; $col < $TOPCHOICES; ++$col)
     {
          # printing the averages (i.e the last row )
          print "\<td BGCOLOR = \"fuschia\" \>";
          printf " \<B\> %5.2f \<\/B\>",$AVERAGE_PERFORMANCE[$col];   
          print "\<\/td\> \n";
          
     }
          # printing the last col of last row
          print "\<td BGCOLOR = \"fuschia\"\>";
          print "\<B\> $TOT_NUM_SAMPLES(Total)  \<\/B\>";   
          print "\<\/td\> \n";
     
     print "\<\/tr\>";
     print "\<\/table\></td><tr>";
     print_top_errors($TOPERRORS);
     # closing the tags
     print "</tr>\<\/table\>";
     print "\<p align\=\"center\"\>\<input type\=\"button\" value\=\"Print Matrix\" onclick\=\'window.print()\'\)\"\/\> ";
     print "</p>\n";
     print "\<\/html\> \n";
     
     select STDOUT; # reverting back to the standard o/p
     
     close WRITE;
     
}  # end of subroutine print_performance_analysis_class



sub performance_analysis_per_class
{    
    my($HTMLflagStatus);
     
     $HTMLflagStatus = shift @_;   # result file
     
     my ($row,$col,$flag,$rowindex);
     my @sortedkeys = sort { $a <=> $b } keys %HASHTABLE_CLASSMAP;

  if( $HTMLflagStatus == 0)
     {
    print "\n----------------------------------------";          
    print "\nClassID\t  TOP1\t\t NumberOfSamples\n";
    print "----------------------------------------\n";     
     
    # printing the elements of the confusion matrix
    for($row = 0 ; $row < $NUM_CLASSES; ++$row)
    {
        print " $sortedkeys[$row] \t ";
          
        $rowindex = $HASHTABLE_CLASSMAP{$sortedkeys[$row]};      
      
        $flag = 0; # clearing the flag
      
        for($col =0; $col < $TOPCHOICES; ++$col)
        {
          if($flag) # if already 100% is achieved then other cols of that row are left blank
          {                   
            next; # go to the next iteration of the for loop
          }
          # if there are no samples for any class then percentage cant be calculated
          if($NUMSAMPLES_PER_CLASS[$rowindex] == 0)
          {
            print " NA    \t";
          }
          else
          {
            my $value = $PERFORMANCE_PERCENTAGE[$rowindex][$col];
            # checking for the 100% accuracy for any class if yes then flag is set 
            if($value == 100)
            {
              $flag = 1;
            }  
            printf " %5.2f\t", $value;                 
          }        
          print " \t$NUMSAMPLES_PER_CLASS[$rowindex]";                               
          print " \n";                  
        }
     }
     }    
     print "\n----------------------------------------\n";
     # printing the last col of last row
     print"\nAverage Samples = ";
          
     print " $TOT_NUM_SAMPLES (Total)"; 
     print "\n";    
     
}  # end of subroutine print_performance_analysis_class

# this subroutine makes the directory and creates the html files in it !!
# input parameter 
#    1. $row of confusion_matrix
#    2. $col of confusion_matrix
# 
# output
#    is the new html file in its corresponding folder  

sub create_html_file
{

     if(@_ != 2)
     {
          print "Number of arguments should be 2: row , col of the confusion_matrix \n";
          die;
     }
     my $target;         # path of the html file to be created
     my ($row,$col);
     my $temp_path;      # temporary variable to store the directory.
     my $image_source;   # variable to store the path of the image file
     my $counter;        # variable used for the for loop
     
     $row = shift @_;    # row of the confusion_matrix
     $col = shift @_;    # col of the confusion_matrix
          
     $target = $PATH."HTML/$row/".$row."\_".$col."\.html";  # complete path of the html file   
          
     unless(open WRITE, ">$target")
     {
          die " Cannot open $target \n";
     }    
     
     $CREATED_HTML_FILES[$HTMLCNT++]=$row."/".$row."_".$col.".html";
     select WRITE; # changing the default o/p to the file $target
     
     # printing the header and the table tags 
     print "\<html\>\n<!-- Generated by LipiTk Evaluation Tool Ver. $Major_Version.$Minor_Version -->\n\<head\>";
     print "<LINK REL=StyleSheet HREF=\"../lipitk.css\" TITLE=\"Lipi\">\n";
     print "<TITLE>LipiToolkit Evaluation Tool</TITLE> \n";
     print "\<script\>\n";
     print"var str=location.search.split(\"?\")[1],link;
     var path1=str.split(\"\$\")[0];
     var path2=str.split(\"\$\")[1];";
     print "\</script\>\</head\>";
     
     # linking the images for the original and recognized image
     
     print "<table align=\"center\" width=\"100%\" border=\"0\" > \n";

     # if its a diagonal element ( i.e of correctly classified samples )
     if($row == $col)
     {
          print " \<CAPTION\> <H3> ";
          print " Sample(s) of Class: $row Classified Correctly. ";
          print " </H3>" ;
          print " \<CAPTION\> \n";
          
     }
     else
     {    # if not a correctly classified cell

          # if its a unclassified cell (i.e. it belongs to -1 column )
          if( $col == $NUM_CLASSES )
          {
               print " \<CAPTION\><H3>";
               print " Sample(s) of Class: $row  NOT classified at ALL !!! ";
               print " </H3>" ;
               print " \<CAPTION\> \n";
          }
          else
          {    # normal misclassified cell 
               print " \<CAPTION\> <H3> ";
               print " Sample(s) of Class: $row   Misclassified as Class: $col ";
               print " </H3>" ;
               print " \<CAPTION\> \n";      
          }
     }


     
     print "<tr align=\"center\"> \n ";
     
     # image for Original Class
     print "<td align=\"center\"> \n "; 
     print "<script>document.write(\"<img src=\\\"\"+path2+\"$Sample_images[$HASHTABLE_CLASSMAP{$row}]\\\" border=\\\"0\\\" />\");</script>";
     print "<br> <font color=\"blue\"> <b> TRUE CLASS </b> </font> </td> \n";
     
     # adding blank cols inbetween original and recognized class
     for($counter = 1; $counter < ($NUM_COL_PER_ROW - 1) ; ++$counter)
     {
          print "<td CLASS=noBorder> &nbsp; </td> \n";
     }    
     
     # image for Recognized Class
     print "<td align=\"center\"> \n";
     print "<script>document.write(\"<img src=\\\"\"+path2+\"$Sample_images[$HASHTABLE_CLASSMAP{$col}]\\\" border=\\\"0\\\" />\");</script>";
     print "<br> <font color=\"red\"> <b> RECOGNIZED CLASS </b> </font> </td> \n";
     
     print "</tr> \n ";
     
     print "</table> <br> \n";
     
     # printing the inner table
     
     print " \<table ALIGN = \"center\" BORDER = \"1\" CELLSPACING = \"2\" WIDTH = \"100%\" ";
     print " RULES =\"ALL\" \> \n";     
     
     select STDOUT; # redirecting to the standard o/p
     close $target;
     
} # end of subroutine create_html_file


# This subroutine closes the tags of the opened html files
# input
#    doesnt take any input (but uses the @CREATED_HTML_FILES array)
#         this array has the file names of the html with the path
# output 
#    appends the closing tags of the html files
# 
          
sub close_html_tags
{
     my $target;                        # variable to store the path of the target file 
     
     my $html_home = $PATH.$conf_file;       # path of the confusion_matrix file !! 
     my ($row,$col);
     
     # for each of the opened html file
     for($row = 0 ; $row < $#CREATED_HTML_FILES; ++$row)
     {
               # path of the filename
               $target = $PATH."HTML/".$CREATED_HTML_FILES[$row];
               
               # opening the file in append mode            
               unless(open WRITE, ">>$target")
               {
                    die " Cannot open $target \n";
               }    

               select WRITE; # changing the default o/p to the file $target
               
               # check for the % criterian to close the row <tr> tag 
               if( ($NUM_COL_PER_ROW  !=0 ) && ($NUMBER_IMAGES_MATRIX[$row][$col] % $NUM_COL_PER_ROW ) != 0 )
               {
                    print "</tr> \n";
               }
               
               # closing the html tags
               print "\<\/table\> \n";
               print "\<\/html\> \n";
     
               select STDOUT; # reverting back to the standard o/p
     
               close WRITE;
     }    
               
}    # end of subroutine close_html_tags


# this subroutine calls the MarchTool.exe and appends the cell contating the image of the file to the corr. file
#    and add the hyperlink to its source
# input
#    1. $image_source or the file from which the image has to be generated
#    2. $image_destination or the target file name where u want to store the image
#    3. $html_source to which html file should we add this image 
#    4. $image_index what is the image number of the file ( images are number by the cell name (row_col_imageindex.png)
#    5. $confidence_level is what the algorithms says that the given file belongs to this class
# output 
#    image is created in the $image_destination and then the tags are added the $html_source          


sub add_image
{
     if(@_ != 5)
     {
          print "the number of arguments should be 5: imagefile_source, imagefile_dest, htmlfile, image number ";
          print ", confidence level \n";
          die;
     }
     
     my $image_source = shift @_;       # souce from which image has to be generated
     my $image_destination = shift @_;  # destination - where image has to be stored
     my $html_source = shift @_;        # file where the generated images has to be added      
     my $image_index = shift @_;        # counter for the image_no for that cell of the confusion_matrix
     my $confidence_level = shift @_;   # variable to store the confidence level
     
     unless(open WRITE, ">>$html_source")
     {
          die " Cannot open $html_source \n";
     }    

     select WRITE; # changing the default o/p to the file $target
     
     # checking whether its a new row if yes adding the new row tag
     if(($image_index % $NUM_COL_PER_ROW) == 1)
     {
          print "<tr> \n";
     }
     
     # adding the element in the row

     print "<td align=\"center\" width=\"100\" height=\"150\">";
     print "<script>link=\"<a href=\\\"\"+unescape(path1)+\"$image_source\\\"><img src=\\\"\"+unescape(path2)+\"$image_destination\\\" border=0></a>\";";
     print "document.write(link);</script>";
     printf "<br>( %5.2f )",$confidence_level;
     print "</td> \n";
     
     # if its the last element of the row then closing the row tag
     if(($image_index % $NUM_COL_PER_ROW) == 0)
     {
          print "</tr> \n";
     }

     select STDOUT;      # redirecting to the standard output
     close WRITE;
}    # end of subroutine add_image



# this subroutine computes the cell wise percentages within each class that would be displayed as a tool tip 
# input
#    nothing (actually @CONFUSION_MATRIX )
# output
#    @CONFUSION_MATRIX_PERCENTAGE has the percentage value.
     
sub compute_confusion_matrix_percentages
{
     my ($row,$col,$sum);
     
     # cacluation percentage value for each row
     for($row = 0; $row < $NUM_CLASSES ; ++$row)
     {
          $sum = 0;
          # finding the sum of the values of each row
          for($col = 0; $col < $NUM_CLASSES + 1; ++$col)
          {
               $sum += $CONFUSION_MATRIX[$row][$col];
          }
          
          # dividing each value of the row by the sum and multiply it with 100 to find the percentages
          for($col=0; $col < $NUM_CLASSES + 1; ++$col)
          {
               # check for the zero elements !!
               if( $CONFUSION_MATRIX[$row][$col] != 0 )
               {
                    $CONFUSION_MATRIX_PERCENTAGE[$row][$col] = (($CONFUSION_MATRIX[$row][$col] * 100 ) / $sum ) 
               }
          }
     }
}


# Takes the number of errors to be printed as input, prints the table into file specified by $perf_file, called from
# print_performance_analysis_class subroutine.
sub print_top_errors
{
     my @sortedkeys = sort { $a <=> $b } keys %HASHTABLE_CLASSMAP;
          
     my $top = shift @_;
     my @nonzero_confusions;
     my($row,$col); # loop variable for each row and col
     my($reverse_map_row,$reverse_map_col);  # variable to store the mapped class_id's

     my($rowindex,$colindex);      

     my $CONFUSION_MATRIX_PERCENTAGE_Size = $#CONFUSION_MATRIX_PERCENTAGE;
     if($top > $CONFUSION_MATRIX_PERCENTAGE_Size)
     {
          print "------Top error choice $top given exceeds size of Confusion Matrix ie $CONFUSION_MATRIX_PERCENTAGE_Size";
     }
     
     my ($row,$col,@sortarray,@final,$i,$j);
     
     print "<td valign=top><table border=1>\n";
     print "<caption><H3>Top Errors</H3></caption>\n";
     print "<tr bgcolor=fuschia><td>True Class</td>";
     print "<td>Recognised as</td>";
     print "<td>Percentage</td></tr>";

     my $count = 0;
     for($row=0; $row < $NUM_CLASSES; ++$row)
     {
          $rowindex = $HASHTABLE_CLASSMAP{$sortedkeys[$row]};         
          for($col=0; $col < ($NUM_CLASSES + 1); ++$col)
          {         
               my $struct;
               $colindex = $HASHTABLE_CLASSMAP{$sortedkeys[$col]};                        
               if($rowindex != $colindex)
               {
                    if($CONFUSION_MATRIX[$rowindex][$colindex] != 0) 
                    {
                    $struct->{row}=$row;
                    $struct->{col}=$col;
                    $struct->{percent}=$CONFUSION_MATRIX_PERCENTAGE[$rowindex][$colindex];
                    $sortarray[$count++]=$struct;
                    
                    }
               }              
          }
     }
     
     for($i=$count-1; $i >= 0; $i--)  # Sorting the array of structures
     {
          for ($j = 1; $j <= $i; $j++)
          {
               if ($sortarray[$j-1]->{percent} < $sortarray[$j]->{percent})
                    {
                       my $temp = $sortarray[$j-1];
                       $sortarray[$j-1] = $sortarray[$j];
                       $sortarray[$j] = $temp;
                    }
          }
     }
     for($i=0;$i<$top;$i++)
     {
     if($sortarray[$i]->{percent} >0)
     {
          print "<td>".$sortarray[$i]->{row}."</td>";
          print "<td>".$sortarray[$i]->{col}."</td>";
          printf "<td>%5.2f</td>", $sortarray[$i]->{percent};
          print "</tr>\n";
     }
                    
     }
     
     
     print "</table></td></tr>\n";
}

# intializing the global variables and calling the main fn confusion_matrix

{
     #use Switch;
     my ($configfile,$resultfile);

     # Setting Default values
     $TOPNCHOICES=1; # Number of top choices to look for before calculating accuracy;
     $TOPCHOICES=1;
     $IMAGE_EXTN=".bmp";
     $TOPERRORS = 5;
     $NUM_COL_PER_ROW = 10;
     $ENABLE_HTML = 1;
     
     if($#ARGV == -1)
     {
          display_usage();
          exit;
     }

     # Command Line Options, see case -help for details
     GetOptions('ver|v'   => \$version,
             'help'    => \$helpRequired,
             'f=s'     => \$configfile,
             'input=s' => \$resultfile,
             'output=s'=> \$out_file,
             'lipiroot=s' => \$lipiPath,
             'topError=i'     => \$TOPERRORS,
             'imgExt=s'    => \$IMAGE_EXTN,
           'accuracy=i'     => \$TOPCHOICES,
           'generateHTML=i'     => \$ENABLE_HTML,
             'images=i'    => \$NUM_COL_PER_ROW);
          
     if( $version )
     {
          print "LipiTk Evaluation Tool Version : $Major_Version.$Minor_Version.$Bugfix_Version\n";
          exit;
     }
     if($helpRequired)
     {
          display_usage();
          exit;
     }
     if($configfile ne "")
     {
          unless(open RFILE, "<$configfile")
          {
               die "cannot open $configfile \n";
          }         
          while(<RFILE>)
          {
               chomp;
               my @line = split(/\=/);
               if(($line[0]=~/^#|^\s+#/) or ($line[0] eq ""))
               {
                    next;
               }
               if($#line != 1)
               {
                    print "line in the config file is in wrong format @line\n";
                    print "the format should be\n";   
                    print "\t\toption      = <input file name>\n";
                    exit;
               }
               $line[0] =~ s/^\s+|\s+$//g;
               $line[1] =~ s/^\s+|\s+$//g;
                    
               if($line[0] eq "input") {$resultfile=$line[1]}
               elsif($line[0] eq "top_errors") {$TOPERRORS=$line[1]}
               elsif($line[0] eq  "image_extension") {$IMAGE_EXTN=$line[1]}
               # elsif($line[0] eq  "confusion_matrix_choices") {$TOPNCHOICES=$line[1]}
               elsif($line[0] eq  "performance_choices") {$TOPCHOICES=$line[1]}
               elsif($line[0] eq  "number_images_row") {$NUM_COL_PER_ROW=$line[1]}
               elsif($line[0] eq  "output") {$out_file=$line[1]}
               elsif($line[0] eq  "lipiroot") {$lipiPath=$line[1]}         
               elsif($line[0] eq  "generateHTML") {$ENABLE_HTML=$line[1]}            
               else 
               {
                    print "unrecognised option $line[0]";
                    exit;
               }
          }
          close(RFILE);
     }
     #       moving to default there is some error from this is >1
     #         elsif($ARGV[$i] eq "-c" ){$TOPNCHOICES=$ARGV[$i+1]} 
     

     if($lipiPath eq "")
     {
          $lipiPath = $ENV{'LIPI_ROOT'};
          if($lipiPath eq "")
          {
               print "Error: LIPI_ROOT env variable is not set";
               exit;
          }
     }

     if($resultfile eq "")
     {
          print "specify a result file\n";
          exit;
     }
     if ($ENABLE_HTML < 0 ||  $ENABLE_HTML>1 )
     {
    print "\nError: genereateHTML value out of bound, Continuing with Default\n";
    $ENABLE_HTML = 1;
     }

     # Replacing \ seperator with / seperator in the path name of result file
     $resultfile =~ s/\\/\//g;
     #Find path of the result file.
     {
          $PATH = $resultfile;
          if ($PATH =~ m/\//){
                $PATH =~ s/\/([a-z,0-9,_, ,A-Z,.])+$/\//g;
          }
          else{$PATH = ""}
     }
     
     my ($accuracy, $i);
     my $print_to_stdout = 0;

     find_number_results($resultfile);
     if($resultsinfile < $TOPNCHOICES)
     {
          print "Warning : Only $resultsinfile choices in results file, confidence matrix with only $resultsinfile choices\n";
          $TOPNCHOICES = $resultsinfile;
     }
     if($resultsinfile < $TOPCHOICES)
     {
          print "Warning : Only $resultsinfile choices in results file, printing performance analysis with only $resultsinfile choices\n";
          $TOPCHOICES = $resultsinfile;
     }
     find_num_classes($resultfile);
     initialize();
     confusion_matrix($resultfile,$ENABLE_HTML);

     unless (open RESULTS, ">>".$out_file)
     {
          #warn "Warning : Output file not specified writing to stdout";
          $print_to_stdout = 1;
     }

     $i = 1;
     
     foreach $accuracy (@AVERAGE_PERFORMANCE )
     {
          my $out_str = sprintf "TOP %d Accuracy  =  %5.2f%% ", $i, $accuracy;
          $i++;
          if($print_to_stdout == 0)
          {
               print RESULTS $out_str."\n";
          }
          else
          {
               print $out_str."\n";
          }
     }
     print "\n----------------------------------------\n";
}


sub display_usage
{
     print "\n\nUsage : eval.pl\n";
     print "\neval.pl\n";
     print "\n\t-f <config file> : Config file containing list of options\n";
     print "\n\t\tConfig file format\n\n";
     print "\t\tinput      = <input file name>\n";
     print "\t\toutput     = <output file name>\n";
     print "\t\tlipiroot   = <lipi root path>\n";
     print "\t\tgenerateHTML = <1 to enable HTML output; 0 to suppress HTML output (default 1)>\n";
     print "\t\ttop_errors = <#top confusions required(default 5)>\n";
     print "\t\timage_extension    = <.bmp,.png (default .bmp)>\n";
     print "\t\tnumber_images_row  = <#images displayed in a row(default 10)>\n";
     print "\t\tperformance_choices= <#top N accuracies required(default 1)>\n";
     print "\nOR\n";
     print "\neval.pl\n";
     print "\n\t-input <input file>\n";
     print "\n\t[-output <output file>]\n";
     print "\n\t[-lipiroot <lipi root path>]\n";
     print "\n\t[-generateHTML <1 to enable HTML output; 0 to suppress HTML output (default 1)>]\n";
     print "\n\t[-topError <#top confusions required (default 5)>]  \n";
     print "\n\t[-imgExt <image extn .bmp .png etc. include the \".\" also (default .bmp)>]\n";
     print "\n\t[-accuracy <#top N accuracies required(default 1)>]\n"; 
     print "\n\t[-images <#images displayed in a row (default 10)>]\n"; 
     print "\n\t[-ver or -v (Displays Version of the Tool)]\n";
     print "\n";
}
