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
 * FILE DESCR: ParseFile  is a class which parse the sample data file and draws the pen-point trace.
 *
 * CONTENTS:
 *			getStrokeData
 *
 * AUTHOR:     Ravi Kiran
 *
 * DATE:       August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;


public class ParseFile
{
	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: ParseFile
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: path - specifies the path to the file which contains the pen-points
	 * RETURNS		: None
	 * NOTES		: This function will parse the pen-points file and add the X & Y co-ordinates to the Strokes
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public ParseFile(String path)
	{
		try
		{
			File file = new File(path+UniPenWriter.extention_string);
			LipiTkUI.DEBUG("Paht to Parse"+path+UniPenWriter.extention_string);

			if( file != null)
			{
				BufferedReader br = new BufferedReader(new FileReader(file));
				if(br != null)
				{
					LipiTkUI.DEBUG(path);

					disp = TopPanel.getDisplay();
					disp.clean();

					String line = null;
					int i= 0;
					int Xpoints[] = new int[500];
					int Ypoints[] = new int[500];
					disp.repaint();

					while((line=br.readLine())!=null)
					{
						if( line.compareTo(".PEN_DOWN") == 0)
						{
							i = 0;
							while(((line=br.readLine()).compareTo(".PEN_UP")) != 0)
							{
								String [] points = line.split(" ");

								Xpoints[i] = Integer.parseInt(points[0]);

								Ypoints[i] = Integer.parseInt(points[1]);
								if( i == 0)
									disp.addPoints(Xpoints[i],Ypoints[i],true);
								else
									disp.addPoints(Xpoints[i],Ypoints[i],false);

								i++;
							}
						}
					}
					br.close();
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in ParseFile class from Constructor function");
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getStrokeData
	 * DESCRIPTION	: This function will return the reference to StrokeDisplay object which is created with the parse pen-points file
	 * ARGUMENTS	: None
	 * RETURNS		: disp - reference to the StrokeDisplay object
	 * NOTES		: ActionEvent contains the reference to the actionevent generated
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public StrokeDisplay getStrokeData()
	{
		return disp;
	}

	private static StrokeDisplay disp;

}