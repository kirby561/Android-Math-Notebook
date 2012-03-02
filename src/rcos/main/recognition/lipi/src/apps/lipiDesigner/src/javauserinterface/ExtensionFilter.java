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
 * FILE DESCR	: ExtensionFilter  is a class which filters the files to display in the project ChooseDialog box (.cfg by default)
 * CONTENTS		:
 *			accept
 *			getDescription
 *			getExtension
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.io.File;
import javax.swing.filechooser.FileFilter;

public class ExtensionFilter extends FileFilter
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: ExtensionFilter
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: string s - extension name
	 * RETURNS		: None
	 * NOTES		: None
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public ExtensionFilter(String s)
	{
		extensionName = s;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: accept
	 * DESCRIPTION	: 
	 * ARGUMENTS	: file
	 * RETURNS		: boolean - true if directory, else false
	 * NOTES		: None
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public boolean accept(File file)
	{
		if(file.isDirectory())
			return true;
		String s = getExtension(file);
		if(s != null)
			return s.equals(extensionName);
		else
			return false;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getDescription
	 * DESCRIPTION	: 
	 * ARGUMENTS	: None
	 * RETURNS		: path of project config file
	 * NOTES		: 
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String getDescription()
	{
		return "." + extensionName;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getExtension
	 * DESCRIPTION	: 
	 * ARGUMENTS	: file
	 * RETURNS		: String - extension of the project config file
	 * NOTES		: None
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static String getExtension(File file)
	{
		String s = null;
		String s1 = file.getName();
		int i = s1.lastIndexOf('.');
		if(i > 0 && i < s1.length() - 1)
			s = s1.substring(i + 1).toLowerCase();
		return s;
	}

	String extensionName;
}