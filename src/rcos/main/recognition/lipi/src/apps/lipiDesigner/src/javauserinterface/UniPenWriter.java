/*****************************************************************************************
 * Copyright (c) 2009 Hewlett-Packard Development Company, L.P.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************************/
/******************************************************************************
 * SVN MACROS
 *
 * $LastChangedDate: 2009-03-20 16:33:02 +0530 (Fri, 20 Mar 2009) $
 * $Revision: 743 $
 * $Author: mnab $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR	: Recognize  is a class which stores the data file into the file system in UNIPEN format
 * CONTENTS		:
 *		write
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.io.BufferedWriter;
import java.io.FileWriter;

public class UniPenWriter
{

	/**********************************************************************************
	* AUTHOR		: Ravi Kiran
	* DATE		    : 18-08-06
	* NAME		    : UniPenWriter
	* DESCRIPTION	: constructor
	* ARGUMENTS		: strokes - Specifies the strokes of a peritcular symbol of the project
	*			      filename - Specifies the name of the file name to store data in uni-pen format
	* RETURNS		: None
	* NOTES		    : This Function will store the pen-points in unipen format into the file system
	* CHANGE HISTROY
	* Author			Date				Description 
	*************************************************************************************/
	public UniPenWriter(LipiTKStroke strokes,String filename)
    {
         extention_string = ".txt";
         write(strokes,filename);
	 }

	/**********************************************************************************
	* AUTHOR		: Ravi Kiran
	* DATE			: 18-08-06
	* NAME			: write
	* DESCRIPTION	: Writes the pen-points in uni-pen format into file system
	* ARGUMENTS		: strokes - Specifies the strokes of a paritcular symbol of the project
	*			  	  filename - Specifies the name of the file name to store data in uni-pen format
	* RETURNS		: None
	* NOTES			: 
	* CHANGE HISTROY
	* Author			Date				Description 
	*************************************************************************************/
	public void write(LipiTKStroke lipiTKStroke, String filename)
	{
		LipiTkUI.DEBUG("Enter Into Writing Storke to File System");
		try
		{
			String str= "";
			FileWriter writer = new FileWriter(filename+extention_string);
			BufferedWriter bufWriter = new BufferedWriter(writer);
			bufWriter.write(".VERSION 1.0");
			bufWriter.newLine();
			bufWriter.write(".HIERARCHY CHARACTER");
			bufWriter.newLine();
			bufWriter.write(".COORD X Y T");
			bufWriter.newLine();
			bufWriter.write(".SEGMENT CHARACTER");
			bufWriter.newLine();
			bufWriter.write(".X_POINTS_PER_INCH 100");
			bufWriter.newLine();
			bufWriter.write(".Y_POINTS_PER_INCH 100");
			bufWriter.newLine();
			bufWriter.write(".POINTS_PER_SECOND 75");
			bufWriter.newLine();
			bufWriter.write(".H_LINE 2760 3280");
			bufWriter.newLine();
			bufWriter.write(".V_LINE 2320 2840");
		
			int strokes = lipiTKStroke.getStrokeSize();
			Stroke stroke[] = lipiTKStroke.getStroke();
			for(int j = 0; j < strokes; j++)
			{
				int strokeLength = stroke[j].getStrokeLength();
				bufWriter.newLine();
				bufWriter.write(".PEN_DOWN");
				bufWriter.newLine();
				LipiTkUI.DEBUG("Stroke Number:"+j);
				Point xy[] = stroke[j].getPoints();
				for(int k=0;k< strokeLength;k++)
				{
					int x = xy[k].getXPoint();
					int y = xy[k].getYPoint();
					int z = 0;
				
					LipiTkUI.DEBUG("X,Y:"+x+","+y);
					str =+x+" "+y+" "+z;
				
					bufWriter.write(str);
					bufWriter.newLine();
				}
		        bufWriter.write(".PEN_UP");
		   }
		  bufWriter.close();
		}
		catch(Exception ex)
	   	{
	   		System.out.println("EXCEPTION WHILE PRINTING STROKE :"+ex.toString());
	   	}
	   	LipiTkUI.DEBUG("Exit from Writing Storke to File System");
	}

	public static String extention_string;
}