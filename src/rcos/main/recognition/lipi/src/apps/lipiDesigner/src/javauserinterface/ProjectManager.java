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
 * FILE DESCR	: ProjectManager  is a class which manages the project directory structure
 * CONTENTS  	:
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.io.File;

public class ProjectManager
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: ProjectManager
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: project_name - Specifies the name of the project
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public ProjectManager(String project_name)
	{
		try
		{
			File data_dir = new File(LIPIUIController.Environment_variable+"/projects/"+project_name+"/data");
			data_dir.mkdirs();

			File config_dir = new File(LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/default");
			config_dir.mkdirs();

			File project_cfg = new File(LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/project.cfg");
			project_cfg.createNewFile();

			File profile_cfg = new File(LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/default/profile.cfg");
			profile_cfg.createNewFile();

			File trainlist_file = new File(LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/default/trainlist.txt");
			trainlist_file.createNewFile();
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in ProjectManager from Project Manager Constructor"+ ex.toString());
		}
	}
}