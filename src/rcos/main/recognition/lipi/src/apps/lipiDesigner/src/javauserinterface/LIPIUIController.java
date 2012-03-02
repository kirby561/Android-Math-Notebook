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
 * FILE DESCR	: BottomPanel  is a class which displays all the controls present in the Bottom Panel of UI
 * CONTENTS		:
 *			addStroke
 *			deleteStroke 
 *			readInkDataFromFileSystem
 *			recognize
 *			checkModalDataExists
 *			getSymbolName
 *			initialize
 *			train
 *			createNewProject
 *			loadProject
 *			getExtension
 *			closeProject
 *			exitProject
 *			writeCFGFile
 *			copyFiles
 *			checkProjectExists
 *			getSelectedClassIndex
 *
 * AUTHOR		: Lenin Fernandes
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.io.File;

public class LIPIUIController
{ 
	/**********************************************************************************
	 * To Communicate with the C++ code(JNI)
	 *************************************************************************************/

	public native void InitializeLipiEngine();
	public native void NewProjectJNI(String projectName);
	public native void LoadProjectJNI(String projectName);
	public native void AddStrokeJNI(String file_path,String classID,boolean trainmode );
	public native void DeleteStrokeJNI(String classId,String file_path,String sampleID, String sampleclassaction, boolean autoTrain );
	public native LipitkResult[] RecognizeJNI(LipiTKStroke inkData);
	public native void TrainJNI();
	public native void CloseProjectJNI();
	public native void ExitJNI();

	static
	{
		System.load(PathFinder.getEnvironment() + "/src/apps/lipiDesigner/lib/" + System.mapLibraryName("lipijniinterface"));
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: LIPIUIController
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Loads the LIPITKJNIInterface dll in the memory
	 * 		  	      Calls the method of LIPITKJNIInterface(InitializeLipiEngine)
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public LIPIUIController()
	{
		LipiTkUI.DEBUG("Enter Into LIPIUIController Class::Constructor Function");

		try
		{
			if(Environment_variable != null)
			{
				initialize();
				sfm = new StrokeFileManager();
			}
		}
		catch(Exception ex)
		{
			System.out.println("Environmental Variable has not set properly:"+ ex.toString());
		}

		LipiTkUI.DEBUG("Exit From LIPIUIController Class::Constructor Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE		    : 18-08-06
	 * NAME		    : addStroke 
	 * DESCRIPTION	: Calls a method of StrokeManager(AddStroke) to add the stroke to the file system
	 *  			  Calls a method of LIPITKJNIInterface (AddStrokeJNI)
	 *  			  LIPITKJNIInterface calls a method of ListFileManager(AddEntryInTrainedListFile()) to add classid/sample to the list file
	 *  	    		  LIPITKJNIInterface calls a method of LIPITK (UnloadModelData)
	 *   			  LIPITKJNIInterface calls a method(Train) of LIPITK to create the trained dat file
	 *  	   		  LIPITKJNIInterface calls a method of LIPITK (LoadModelData)
	 *
	 * ARGUMENTS	: inkData - stroke data to be added to file system
	 * 			  	  classID - class id to which to add the stroke
	 * 			      sampleclassaction - add a sample to existing class  or add new class along with the stroke data
	 * 			      autoTrain - automatic training or manual
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void addStroke(String projectName, LipiTKStroke inkData,String data_dir, String classID, String sampleclassaction, boolean trainmode)
	{
		LipiTkUI.DEBUG("Enter Into LIPIUIController Class::AddStroke Function");
		try
		{
			String file_path = sfm.addStroke(inkData,data_dir,classID,sampleclassaction);

			file_path = file_path + LIST_EXTENSION;

			AddStrokeJNI(file_path,classID,trainmode );
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LIPIUIController class from Add Stroke function"+ ex.toString());
		}

		LipiTkUI.DEBUG("Exit From LIPIUIController Class::AddStroke Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: deleteStroke
	 * DESCRIPTION	: Calls a method of StrokeManager(DeleteStroke) to add the stroke to the file system
	 *  			  Calls a method of LIPITKJNIInterface (DeleteStrokeJNI)
	 *  			  LIPITKJNIInterface calls a method of ListFileManager(DeleteEntryFromListFile()) to delete classid/sample to the list file
	 *  	    		  LIPITKJNIInterface calls a method of LIPITK (UnloadModelData)
	 *   			  LIPITKJNIInterface calls a method(Train) of LIPITK to create the trained dat file
	 *  	   		  LIPITKJNIInterface calls a method of LIPITK (LoadModelData)
	 *
	 * ARGUMENTS	: classID - class id from which to delete the stroke
	 *			  	  sampleID - sample id of the stroke to be deleted
	 * 			      sampleclassaction - delete a sample from this class  or delete the class
	 * 			      autoTrain - automatic training or manual
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void deleteStroke(String projectName,String classID,String data_dir,String sampleID, String sampleclassaction, boolean trainmode )
	{
		String file_path;

		try
		{
			file_path = sfm.deleteStroke(data_dir,sampleID,sampleclassaction);
			DeleteStrokeJNI(classID,file_path,sampleID,sampleclassaction,trainmode );
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from DeleteStroke function:"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE		    : 18-08-06
	 * NAME		    : readInkDataFromFileSystem
	 * DESCRIPTION	: This method calls a method of StrokeManager (ReadInkDataFromFileSystem) load the Ink data in the memory
	 * ARGUMENTS	: classID - class id for which to load the samples
	 * RETURNS		: String[]- List of samples in this class
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String[] readInkDataFromFileSystem(String classId)
	{
		String [] DataFiles = null;
		try
		{
			DataFiles = sfm.getStringList(classId);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from ReadInkDataFromFileSystem function:"+ex.toString());
		}
		return DataFiles;
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: recognize
	 * DESCRIPTION	: This method calls a method of LIPITKJNIInterface(Recognize) to  recognize the stroke drawn by the user
	 * ARGUMENTS	: inkData - stroke data to be recognized
	 * RETURNS		: LipitkResults[] - array of class ids along with the confidence
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public LipitkResult[] recognize(LipiTKStroke inkData)
	{
		LipitkResult lipitkresult[]=null;
		try
		{
			LipiTkUI.DEBUG("INSIDE Recognize:");

			lipitkresult = RecognizeJNI(inkData);
			/**
			 *'5' used for the number of results is changed to 
			 *lipitkresult.length 
			 */
			for(int i = 0;i<lipitkresult.length;i++)
			{
				int id = lipitkresult[i].getId();
				float confidence = lipitkresult[i].getConfidence();
				LipiTkUI.DEBUG("id:"+id+"confidence:"+confidence);
			}
			LipiTkUI.DEBUG("AFTER JNI:");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from Recognize function:"+ex.toString());
		}
		return lipitkresult;
	}

	public boolean checkModalDataExists(String project_name)
	{
		if(sfm.checkModalDataExists(project_name))
		{
			return true;
		}
		else
			return false;
	}

	public String getSymbolName(int id, String project_config_dir)
	{
		return(sfm.getSymbolName(id,project_config_dir));
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: initialize
	 * DESCRIPTION	: Loads the LIPITKJNIInterface dll in the memory
	 *			  	  Calls the method of LIPITKJNIInterface(InitializeLipiEngine)
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void initialize( )
	{
		try
		{
			LipiTkUI.DEBUG("**************Enter Into Initialize Function*************");

			InitializeLipiEngine();

			LipiTkUI.DEBUG("**************Exit From Initialize Function*************");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from Initialize function:"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: train
	 * DESCRIPTION	: Calls the method of LIPIJNIInterface(Train)
	 *			      LIPIJNIInterface calls the method of LIPITK (UnLoadModelData)
	 *			      LIPIJNIInterface calls the method of LIPITK (Train)
	 *			      LIPIJNIInterface calls the method of LIPITK (LoadModelData)
	 *			  
	 * ARGUMENTS	: projectName - project to be trained(path of the project name will be passed)
	 * RETURNS		: None
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void train(String projectName)
	{
		LipiTkUI.DEBUG("**************Train Method Called************");
		try
		{
			LipiTkUI.DEBUG("Before: Train:");
			TrainJNI();
			System.out.println("After: Train:");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from Train function:"+ex.toString());
		}
		LipiTkUI.DEBUG("************Train Method Called***********");
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: createNewProject
	 * DESCRIPTION	: Calls the  method of ProjectManager(CreateNewProject) to create the directory
	 *			      on the file system
	 * ARGUMENTS	: projectName - name of the project
	 * RETURNS		: None
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void createNewProject(String projectName)
	{
		try
		{
			LipiTkUI.DEBUG("Enter Into Create Directory Structure");

			pmr =  new ProjectManager(projectName);

			writeCFGFile(Environment_variable + PROJECTS_PATH + projectName + CONFIG_FILE, projectName);
			copyFiles(Environment_variable+DEFAULTCONFIG_PATH, Environment_variable + PROJECTS_PATH + projectName + DEFAULT_PATH);
			NewProjectJNI(projectName);

			LipiTkUI.DEBUG("Exit From Create Directory Structure");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from CreateNewProject function:"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: loadProject
	 * DESCRIPTION	: Calls the method of StrokeFileManager(LoadProject)
	 *			  	  Calls the method  of LIPIJNIInterface (LoadProject)to load the project in memory
	 *			  	  LIPIJNIInterface calls the method (LoadModelData) of LIPITK
	 *			  
	 * ARGUMENTS	: projectName - name of the project
	 * RETURNS		: String[]- class ids
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String[] loadProject(String projectName)
	{
		String [] classIDs = null;
		String [] temp = null;
		try
		{
			LipiTkUI.DEBUG("Inside Load Project" +projectName);
			classIDs = sfm.getStringList(Environment_variable + PROJECTS_PATH + projectName + DATA_PATH);
			if( (classIDs != null) && (classIDs.length != 0))
			{
				temp = sfm.getStringList(Environment_variable + PROJECTS_PATH + projectName + DATA_PATH + SEPARATOR + classIDs[0]);
			}
			if(temp != null)
			{
				UniPenWriter.extention_string = getExtension(temp[0]);
			}
			else
			{
				UniPenWriter.extention_string = LIST_EXTENSION;
			}

			LipiTkUI.DEBUG("***********ExtenStionString"+ UniPenWriter.extention_string);
			System.out.println("in java projectName :" + projectName);
			LoadProjectJNI(projectName);
			LipiTkUI.DEBUG("Inside Load Project");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from LoadProject function:"+ex.toString());
		}
		return classIDs;
	}

	private String getExtension(String file_name)
	{
		return (file_name.substring(file_name.lastIndexOf("."),file_name.length()));
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: closeProject
	 * DESCRIPTION	: Calls the method  of LIPIJNIInterface(CloseProject) to close the the project.
	 *			      LIPIJNIInterface calls the method (UnLoadModelData) of LIPITK
	 *			      Unloads the LIPIJNIInterface dll from memory
	 * ARGUMENTS	: projectName - name of the project
	 * RETURNS		: None
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void closeProject()
	{
		try
		{
			LipiTkUI.DEBUG("Before Closing the application");
			CloseProjectJNI();
			LipiTkUI.DEBUG("After Closing the application");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from CloseProject function:"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: exitProject
	 * DESCRIPTION	: Calls the method  of LIPIJNIInterface(Exit).			  
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void exitProject()
	{
		try
		{
			ExitJNI();
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from Exit Project function:"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: writeCFGFile
	 * DESCRIPTION	: Writes the project configuration file			  
	 * ARGUMENTS	: path - path of the project
	 *			  	  project_name - name of the project
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private void writeCFGFile(String path,String project_name)
	{
		try
		{
			sfm.writeConfigFile(path,project_name);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from WriteCFGFile function:"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: copyFiles
	 * DESCRIPTION	: Copies the default configurations files from the configuration folder			  
	 * ARGUMENTS	: src - path of the default configuration folder
	 *			      des - path of the project configuration folder
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private void copyFiles(String src, String dest)
	{
		try
		{
			LipiTkUI.DEBUG("Source dir"+src);
			LipiTkUI.DEBUG("Desti dir"+dest);
			File scr_file = new File(src);
			File dest_file = new File(dest);
			sfm.copy(scr_file,dest_file,src,dest);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from CopyFiles function:"+ex.toString());
		}
	}

	public boolean checkProjectExists(String project_name)
	{
		return (sfm.checkProjectExists(project_name));
	}

	/**********************************************************************************
	 * AUTHOR		: Lenin Fernandes
	 * DATE			: 18-08-06
	 * NAME			: getSelectedClassIndex
	 * DESCRIPTION	: returns the index of the class selected (in the Classes JList)			  
	 * ARGUMENTS	: classPathDir - path of the data directory in the project
	 *			      classID - class id
	 * RETURNS		: classIndex - the index of the class selected 
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public int getSelectedClassIndex(String classPathDir, String classID)
	{
		String [] DataFiles = null;
		int classIndex = -1;
		try
		{
			DataFiles = sfm.getStringList(classPathDir);
			for(int i = 0; i < DataFiles.length;i++)
			{
				if(DataFiles[i]. equals(classID))
				{
					classIndex = i;
					break;
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside LIPIUIController class from ReadInkDataFromFileSystem function:"+ex.toString());
		}
		LipiTkUI.DEBUG("*********Identified Index:"+classIndex);
		return classIndex;
	}

	private StrokeFileManager sfm;
	private ProjectManager pmr;
	public static String Environment_variable = PathFinder.getEnvironment();
	private final String LIST_EXTENSION = ".txt";
	private final String PROJECTS_PATH = "/projects/";
	private final String CONFIG_FILE = "/config/project.cfg";
	private final String DEFAULTCONFIG_PATH = "/projects/default_config";
	private final String DEFAULT_PATH = "/config/default";
	private final String DATA_PATH = "/data";
	private final String SEPARATOR = "/";
}
