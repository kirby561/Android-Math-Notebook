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
 * FILE DESCR	: LipiTKUtil provides few utilities like rounding off of variables, opening documents etc.,
 * CONTENTS		:
 *			round
 *			ShowImage
 *			openDocument
 *          accept
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.awt.Image;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.File;
import java.io.FilenameFilter;
import javax.swing.ImageIcon;

public class LipiTKUtil implements FilenameFilter
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: round
	 * DESCRIPTION	: Round a double value to a specified number of decimal places
	 * ARGUMENTS	: val - the value to be rounded
	 *			      places - the number of decimal places to round to
	 * RETURNS		: val - rounded to places decimal places
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private static double round(double val, int places)
	{
		long factor = 0;
		long tmp = 0;
		try
		{
			factor = (long)Math.pow(10,places);

			// Shift the decimal the correct number of places
			// to the right.
			val = val * factor;

			// Round to the nearest integer.
			tmp = Math.round(val);
		}
		catch(Exception exception)
		{
			System.err.println("JAVA UI Interface: Exception occured in LipiTKUtil while triming float value :"+ exception.toString());
		}

		// Shift the decimal the correct number of places
		// back to the left.
		return (double)tmp / factor;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: round
	 * DESCRIPTION	: Round a float value to a specified number of decimal places
	 * ARGUMENTS	: val - the value to be rounded
	 *			      places - the number of decimal places to round to
	 * RETURNS		: val - rounded to places decimal places
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static float round(float val, int places)
	{
		return (float)round((double)val, places);
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: showImage
	 * DESCRIPTION	: Reads the image from the specified image path
	 * ARGUMENTS	: imagePath - the path of the image
	 * RETURNS		: image
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static Image showImage(String imagePath)
	{
		Image image = null;
		try
		{
			java.net.URL imgURL = LipiTkUI.class.getResource(imagePath);
			if (imgURL != null)
			{
				image = new ImageIcon(imgURL).getImage();
				return image;
			}
		}
		catch (Exception exception)
		{
			System.err.println("JAVA UI Interface: Exception occured in LipiTKUtil while getting image :"+ exception.toString());
		}
		return image;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: openDocument
	 * DESCRIPTION	: The pdf document is opened using this function
	 * ARGUMENTS	: filePath - the path of the pdf file
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static void openDocument(String filePath)
	{
		try
		{
			String operatingSystem = System.getProperty("os.name").toLowerCase();
			if(operatingSystem.startsWith("windows"))
			{
				Runtime.getRuntime().exec("cmd /c "+filePath);
			}
			else
			{
				Runtime.getRuntime().exec("xpdf "+filePath);
			}
		}
		catch (FileNotFoundException fileNotFoundException)
		{
			System.err.println("JAVA UI Interface: The document is not found:"+ fileNotFoundException.toString());
		}
		catch (IOException ioException)
		{
			System.err.println("JAVA UI Interface: An IOException occured in LipiTKUtil while opening document :"+ ioException.toString());
		} catch (Exception exception)
		{
			System.err.println("JAVA UI Interface: Exception occured in LipiTKUtil while opening document :"+ exception.toString());
		}
	}


	/**********************************************************************************
	 * AUTHOR		: Hari Krishna, BA
	 * DATE			: 15-04-08
	 * NAME			: accept
	 * DESCRIPTION	: Filters the files with .txt extension and folders with out "." character
	 * ARGUMENTS	: File - directory on which the list() is called, String - directory/file to be checked
	 * RETURNS		: boolean
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public boolean accept(File dir, String name) 
	{ 
		if(name.endsWith(".txt") || name.indexOf(".") == -1) 
			return true;
		else
			return false; 
	}

}
