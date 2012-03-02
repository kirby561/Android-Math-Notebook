README for LipiTk Evaluation Tool Version 2.0
---------------------------------------------

1. Command Line Tool with the following options

-v, 	Displays Version of the Tool
-f, 	Specify the file containing the list of options. Format of file:
     	option=value
     	option=value etc.
     	Names of options: result_file, top_errors, image_extension
     	confusion_matrix_choices,performance_choices,number_row.
-help, 	Displays this list
-r, 	Full path of result file
-e, 	N for Top N errors, default 5
-ie, 	Image extension, include a . as well .png,.bmp etc., default .png
-c, 	Number of choices for populating the confusion matrix, default 1
-p, 	Number of choices for displaying the performance analysis table, default 5
-nc, 	Number of images per row for displaying, default 10

Example usage: perl eval.pl -r pcaresults.txt -c 1 -e 10 -nc 10 -p 5 -ie .png

2. Config file specified with the -f option

The format is:
option=value
option=value etc.

The options are :
result_file=_______ 		for specifying result file
top_errors=________ 		Specifying N for printing top N errors
image_extension=_____ 		Specifying the image extension
confusion_matrix_choices=_____ 	Specify the number of choices for populating the CF matrix
performance_choices=_________	Specify the number of choices for printing performance 						analysis
number_row=_______________	Number of images to be displayed per row in the HTML file

The default values mentioned in 1. are assigned if the option is not given a value.

Example Config file:
# input.txt
result_file=pcaresults.txt
top_errors=10
image_extension=.png
confusion_matrix_choices=1
performance_choices=5
number_row=8

# acts as a comment.		

3. For commenting lines in the config file for specifying command line options or for commenting lines in the input results file, use a # at the beginning of the line.

4. After generating the confusion_matrix.html. One will have to use view.html to    display the confusion_matrix.html. Using the view.html, one can also move the    generated confusion_matrix.html along with the HTML to a different system.   
