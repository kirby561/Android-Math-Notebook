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
 * FILE DESCR	: StrokeFileManager  is a class which manages the file name formats and actions related to file system.
 * CONTENTS		:
 *			addStroke
 *			parseFileName
 *          manupulateFileNum
 *			deleteStroke
 *			deleteDir
 *			getStringList
 *			writeConfigFile
 *			copy
 *			getSymbolName
 *			checkProjectExists
 *			checkModalDataExists
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FilenameFilter;

public class StrokeFileManager
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: addStroke
	 * DESCRIPTION	: This method Adds the stroke points in uni-pen format into the file system.
	 * ARGUMENTS	: inkData - contains the pen-points of a stroke
	 *			      path - contains the path of the file to store
	 *  			  classID - contains the string format of classID
	 *			      sampleclassAction - contains the action in string format - 'Add Class' specifies 'Adding a classs'.
	 * 													 'Add Sample' specifies 'Adding a sample'.
	 *
	 * RETURNS		: path - specifies the path of class
	 * NOTES		: ActionEvent contains the reference to the actionevent generated
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String addStroke(LipiTKStroke  inkData, String path,String classID, String sampleclassAction )
	{
		try
		{
			File fp = new File(path);
			if(sampleclassAction == "Add Class")
			{
				/**********Creating a new class***********/
				LipiTkUI.DEBUG("Control reached inside Controller");

				file_count = 0;
				fp.mkdir();
				LipiTkUI.DEBUG("Directory to create:"+path);
				path = manupulateFileNum(path,classID);
				new UniPenWriter(inkData,path);
				// To update the filepaths in listfile.txt with lipi root
				path = "$LIPI_ROOT/projects/" + LipiTkActionClass.project_name + "/data/" + classID + "/" + classID + "_" + file_count;
			}
			else
			{
				/**************Adding a new sample to the existing class*****/
				FilenameFilter filter = new LipiTKUtil();
				String [] strtemp = fp.list(filter);
				int num_Files = strtemp.length;

				LipiTkUI.DEBUG("Number of files in selected Item:"+num_Files);
				int max = 0;
				int i = 0;
				while(i<strtemp.length)
				{
					if( max < parseFileName(classID,strtemp[i]))
					{
						max =   parseFileName(classID,strtemp[i]);
					}
					i++;
				}
				LipiTkUI.DEBUG("File index number to add"+max);
				path= path+ "/"+classID+"_"+(max+1);
				new UniPenWriter(inkData,path);
				// To update the filepaths in listfile.txt with lipi root
				path = "$LIPI_ROOT/projects/" + LipiTkActionClass.project_name + "/data/" + classID + "/" + classID + "_" + (max+1);	
				LipiTkUI.DEBUG("Path Name:"+ path);
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside StrokFileManager class from AddStroke function:"+ex.toString());
		}
		return path;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: parseFileName
	 * DESCRIPTION	: Parses the file name of a Stroke
	 * ARGUMENTS	: classID - contains the string format of classID
	 *			  	  file_name - file name to be parsed
	 * RETURNS		: a - sample number used in the file
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private int parseFileName(String classID, String file_name)
	{
		String [] split_str = null;
		String num = null;
		try
		{
			LipiTkUI.DEBUG("File number to append"+ file_name);
			split_str = file_name.split(".txt");
			// splits the file path and returns the sample number used in the file
			int temp = split_str[0].lastIndexOf("_"); 
			num = split_str[0].substring(temp+1);
			LipiTkUI.DEBUG("Number to be added:"+ num);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside StrokFileManager class from ParseFileName function:"+ex.toString());
		}

		int a = new Integer(num).intValue();
		return a;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: manupulateFileNum
	 * DESCRIPTION	: Manupulates the file number to be added to Stroke file
	 * ARGUMENTS	: path - path to the dat directory of the project
	 *			      classID - contains the string format of classID			  
	 * RETURNS		: path - file name of the stroke
	 * NOTES		: 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private String manupulateFileNum(String path,String classID)
	{
		path = path+"/"+classID+"_"+file_count;
		return path;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: deleteStroke
	 * DESCRIPTION	: This method Deletes the stroke points in uni-pen format from the file system.
	 * ARGUMENTS	: path - contains the path of the file to delete.
	 *			  	  sampleID - contains the string format of sampleID to delete
	 *			  	  sampleclassAction - contains the action in string format - 'Delete Class' specifies 'Deleting a classs' 
	 *			  										 'Delete Sample' specifies 'Deleting a sample'.
	 * RETURNS		: path - file name of the stroke
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String deleteStroke(String path,String sampleID, String sampleclassAction )
	{
		String return_path = null;

		try
		{
			if(sampleclassAction == "Delete Class")
			{
				return_path = path;
				File fp = new File(path);
				deleteDir(fp);
				LipiTkUI.DEBUG("Deleted");
			}
			else
			{
				int temp = sampleID.lastIndexOf("_");
				String classID = sampleID.substring(0,temp);
				return_path = "$LIPI_ROOT/projects/"+LipiTkActionClass.project_name+"/data/" +classID+ "/"+ sampleID+UniPenWriter.extention_string; 
				File fp = new File(path+"/"+sampleID+UniPenWriter.extention_string);

				fp.delete();
				LipiTkUI.DEBUG("Delete");
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception inside StrokFileManager class from DeleteStroke function:"+ex.toString());
		}
		return return_path;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: deleteDir
	 * DESCRIPTION	: This method Deletes the class directory from the data directory of the project if SampleClassAction is 'Delete Class'.
	 * ARGUMENTS	: dir - contains the path of the class to delete.			  
	 * RETURNS		: true - if directory is deleted, else false
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private  boolean deleteDir(File dir)
	{
		if (dir.isDirectory())
		{
			String[] children = dir.list();
			for (int i=0; i<children.length; i++)
			{
				boolean success = deleteDir(new File(dir, children[i]));
				if (!success)
				{
					return false;
				}
			}
		}

		// The directory is now empty so delete it
		return dir.delete();
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getStringList
	 * DESCRIPTION	: This method returns the list of files & sub-folders present in the given file path
	 * ARGUMENTS	: path - contains the string path to search			  
	 * RETURNS		: String[] - contains the list of files and sub-folders
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String[] getStringList(String path)
	{
		String [] samples = null;
		try
		{
			File Fp = new File(path);

			LipiTkUI.DEBUG("Path to check:"+path);
			FilenameFilter filter = new LipiTKUtil();
			samples = Fp.list(filter);
			int length = samples.length;

			LipiTkUI.DEBUG("Length:"+length);
			for(int i=0;i<length;i++)
			{
				LipiTkUI.DEBUG("**************SAMPLES"+samples[i]+"**************");
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in StrokeFileManager class From GetStringList Function" + ex.toString());
		}
		return samples;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: writeConfigFile
	 * DESCRIPTION	: This method writes the profile.cfg configuaration file into the file system
	 * ARGUMENTS	: path - contains the string path of file system
	 *			      project_name - contains the project name			  
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void writeConfigFile(String path,String project_name)
	{
		try
		{
			FileWriter writer = new FileWriter(path);
			BufferedWriter bufWriter = new BufferedWriter(writer);
			bufWriter.write(PROJECT_TYPE);
			bufWriter.write(EQUALS);
			bufWriter.write(SHAPE_RECOGNIZER);
			bufWriter.newLine();
			bufWriter.write(PROJECT_NAME);
			bufWriter.write(EQUALS);
			bufWriter.write(QUOTATION);
			bufWriter.write(project_name);
			bufWriter.write(QUOTATION);
			bufWriter.newLine();
			/**
			 *Writes to the configuration file to set the 
			 *number of shapes used in a project
			 */
			bufWriter.write(NUM_SHAPES);
			bufWriter.write(EQUALS);
			bufWriter.write(DYNAMIC);	
			bufWriter.close();
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in StrokeFileManager class From WriteConfigFile Function" +ex.toString() );
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: copy
	 * DESCRIPTION	: This method copies the default configuaration files into the corresponding project config folder
	 * ARGUMENTS	: src - contains the reference to Source file system
	 *			      dst - contains the reference to Destination file system			  
	 *			      source - contains the String to the source file path
	 *			      destin - contains the String to the destination file path
	 * RETURNS		: None
	 * NOTES		: throws Standard exception if any exception occured
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void copy(File src, File dst, String source, String destin) throws IOException
	{
		try
		{
			String [] file_list = src.list();

			for(int i = 0;i< file_list.length;i++)
			{

				if(!((new File(source + "/" + file_list[i])).isDirectory()))
				{
					FileReader temp = new FileReader(source + "/" + file_list[i]);
					FileWriter temp1 = new FileWriter(destin + "/" + file_list[i]);
					BufferedReader in = new BufferedReader(temp);
					BufferedWriter out = new BufferedWriter(temp1);
					String str ;
					while ((str = in.readLine()) != null)
					{
						out.write(str);
						out.newLine();
					}
					in.close();
					out.close();
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in StrokeFileManager class From Copying CFG file Function"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getSymbolName
	 * DESCRIPTION	: This method returns the  string id to the corresponding SRE id
	 * ARGUMENTS	: id - contains the SRE ID
	 *			      project_config_dir - contains the path to the project_config_dir			  
	 * RETURNS		: splited_line - indicates the String class name corresponding to SRE id
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public String getSymbolName(int id,String project_config_dir)
	{
		String line;
		String [] splited_line= null;
		LipiTkUI.DEBUG("******************************Inside getSymbolName function"+project_config_dir+"MapFile.ini");

		try
		{
			File map_file = new File(project_config_dir+"MapFile.ini");
			BufferedReader readIni = new BufferedReader(new FileReader(map_file));
			while((line=readIni.readLine())!=null)
			{
				splited_line = line.split(" ");
				LipiTkUI.DEBUG("Splited Values:"+splited_line[0]+splited_line[1]);
				if(splited_line[1].equals((new Integer(id)).toString()))
				{
					LipiTkUI.DEBUG("ID Found");
					break;
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in StrokeFileManager class From getSymbolName Function"+ex.toString());
		}
		return splited_line[0];
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: checkProjectExists
	 * DESCRIPTION	: This method will check whether the new project created is already existing or not
	 * ARGUMENTS	: project_name - contains the name of the project			  
	 * RETURNS		: true - indicates project doesnt exists & false - indicates project name already exists
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public boolean checkProjectExists(String project_name)
	{
		File file_check = new File(LIPIUIController.Environment_variable+"/projects/"+project_name);
		if(file_check.exists())
		{
			LipiTkUI.DEBUG("Project already Exists");
			return false;
		}
		else
		{
			LipiTkUI.DEBUG("Project doesn't exists");
			return true;
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: checkModalDataExists
	 * DESCRIPTION	: method will check whether the modal data file for project exists or not
	 * ARGUMENTS	: project_name - contains the name of the project		  
	 * RETURNS		: true - indicates modal data exists & false indicates modal data doesn't exists
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static boolean checkModalDataExists(String project_name)
	{
		File file_check = new File(LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/default/nn.mdt");
		if(file_check.exists())
		{
			LipiTkUI.DEBUG("Modal Data Exists");
			return true;
		}
		else
		{
			LipiTkUI.DEBUG("Modal Data doesn't exists");
			return false;
		}
	}

	private final String EQUALS = "=";
	private final String PROJECT_TYPE = "ProjectType";
	private final String SHAPE_RECOGNIZER = "SHAPEREC";
	private final String PROJECT_NAME = "ProjectName ";
	private final String QUOTATION = "\"";
	private final String NUM_SHAPES = "NumShapes";
	private final String DYNAMIC = "Dynamic";
	private static int file_count;
}
