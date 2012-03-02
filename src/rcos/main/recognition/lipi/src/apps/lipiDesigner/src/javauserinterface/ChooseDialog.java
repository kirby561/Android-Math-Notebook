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
 * $LastChangedDate: 2011-08-23 13:44:15 +0530 (Tue, 23 Aug 2011) $
 * $Revision: 882 $
 * $Author: jitender $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR	: ChooseDialog  is a class which displays the project list dialog box to display the projects avialable
 * CONTENTS		:
 *			choose
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.io.File;
import javax.swing.JFileChooser;

public class ChooseDialog extends JFileChooser
{
	private String fileNameChoose;

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: ChooseDialog
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: creates the Choose project dialog box and displays it
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public ChooseDialog()
	{
		LipiTkUI.DEBUG("Enter Into ChooseDialog Class::Constructor Function");

		try
		{
			String s2 = LIPIUIController.Environment_variable+"/projects/" + System.getProperty("file.separator");

			File file;
			ExtensionFilter extensionfilter1 = new ExtensionFilter("");
			setFileFilter(extensionfilter1);
			file = new File(s2);
			setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			setCurrentDirectory(file);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in ChooiseDialog class from constructor function");
		}
		LipiTkUI.DEBUG("Exit From ChooseDialog Class::Constructor Function");


	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: choose
	 * DESCRIPTION	: This method will return the name of the project choosen from the project list dialog box
	 * ARGUMENTS	: None
	 * RETURNS		: String
	 * NOTES		: return fileNameChoose indicates the name of the project choosen from the dialog box
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String choose()
	{
		LipiTkUI.DEBUG("Enter Into ChooseDialog Class::Choose Function");

		try
		{
			int i = showDialog(this, "Select");
			if(i == 0)
			{
				fileNameChoose = getSelectedFile().getPath();
				LipiTkUI.DEBUG(fileNameChoose);
			} 
			else
				fileNameChoose = "";
			
			if(fileNameChoose.endsWith(".cfg"))
				LipiTkUI.DEBUG("The File " + fileNameChoose + " is loaded properly");
			else if(fileNameChoose != "")
				LipiTkUI.DEBUG("The File " + fileNameChoose + " can not be load (not ended by .cfg)");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in ChooseDialog class from Choose function");
		}

		LipiTkUI.DEBUG("Exit From ChooseDialog Class::Choose Function");

		return fileNameChoose;
	}
}
