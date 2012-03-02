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
 * $LastChangedDate: 2011-04-12 17:52:54 +0530 (Tue, 12 Apr 2011) $
 * $Revision: 846 $
 * $Author: mnab $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR	: LipiTkActionClass  is a class which handles all the actions
 *			   corresponding to the buttons present in the main UI
 *
 * CONTENTS		:
 *			actionPerformed
 *			helpFunction
 *			aboutFunction
 *			createFunction
 *			closeFunction
 *			loadFunction
 *			createPackage
 *			removeExtension
 *			exitFunction
 *			addSampleFunction
 *			addClassFunction
 *			checkClassExists
 *			recognizeFunction
 *          packageExists
 *			doAction
 *			train
 *			removeClass
 *			removeSample
 *			readProjectCFG
 *			clearFunction
 *			getDataDirectory
 *
 * AUTHOR		: Ravi Kiran
 * DATE			:  August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
//import java.io.InputStreamReader;

public class LipiTkActionClass implements ActionListener
{
	
	
	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: LipiTkActionClass
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: lipicontrl - the refernce value of LIPIUIController object
	 * RETURNS		: None
	 * NOTES		: Reads the reference variable of LipiTkController object into local temporary variable
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public LipiTkActionClass(LIPIUIController lipicontrl)
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::Constructor Function");

		try
		{
			lipicontroller = lipicontrl;
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception from LipiTkActionClass Constructor"+ ex.toString());
		}

		LipiTkUI.DEBUG("Exit From LipiTKAction Class::Constructor Function");
	}
	
	 

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: actionPerformed
	 * DESCRIPTION	: This method will get invoked whenever the user made any action on control present in the user-interface
	 * ARGUMENTS	: actionevent - contains the reference to the ActionEvent type
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public void actionPerformed(ActionEvent event_type)
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::ActionPerformed Function");
		
		
		
		try
		{
			String control_name = (String)event_type.getActionCommand();

			if (control_name.equals(ACTION_NEW))
			{
				createFunction();
				bNewOrLoad = true;
			}
			else if(control_name.equals(ACTION_CLOSE))
			{
				//Added by Murali on 11/12/2006 for warning a message dialog box for closing the project
				//Request Id: 1613106
				//Added by Jitender on 25/08/2011 for warning a message dialog box to save before closing the project
				int nOption = JOptionPane.showConfirmDialog(LipiTkUI.popupcontainer,"Do you want to close current project?","Confirmation",JOptionPane.YES_NO_OPTION);
				
				if(JOptionPane.YES_OPTION == nOption)
				{
					closeFunction();
					//exitFunction();
					bNewOrLoad = false;
				}
			}
			else if(control_name.equals(ACTION_LOAD))
			{
				loadFunction();
				bNewOrLoad = false;
			}
			else if(control_name.equals(ACTION_PACKAGE))
			{
				createPackage();
			}
			else if(control_name.equals(ACTION_SAVE))
			{
				System.out.println("ACTION_SAVE");
				updateLipiengineConfig();
				//closeFunction();
			}
			else if(control_name.equals(ACTION_DELETE))
			{
				int nOption = JOptionPane.showConfirmDialog(LipiTkUI.popupcontainer,"Do you want to remove current project from projects directory?","Confirmation",JOptionPane.YES_NO_OPTION);
				
				if(JOptionPane.YES_OPTION == nOption)
				{				
					
					String removeDir = LIPIUIController.Environment_variable + "/projects/" + project_name;
					System.out.println("initial project to be deleted :" + removeDir);
					
					if(false == removeDirectory(removeDir))
						System.out.println("failed to delete : \n" + removeDir);
					
					System.out.println("directory deleted delete : \n ");
					
					deleteProjectLipiengineConfig();
					System.out.println("lipiengine updated: \n");
					closeFunction();
					System.out.println("close : \n");
				}
				
			}
			else if(control_name.equals(ACTION_EXIT))
			{
				//Added by Murali on 07/12/2006 for warning a message dialog box for exit the application
				//Request Id: 1608969
				
				//Added & Removed by  Jitender 07/12/2011 :) for saving project before exiting.
		
				int nOption = JOptionPane.showConfirmDialog(LipiTkUI.popupcontainer,"Do you want to exit?","Confirmation",JOptionPane.YES_NO_OPTION);
			
				if(JOptionPane.YES_OPTION == nOption)
				{
					closeFunction();
					exitFunction();
					bNewOrLoad = false;
				}
			}
			else if(control_name.equals(ACTION_HELP))
			{
				helpFunction();
			}
			else if(control_name.equals(ACTION_ABOUT))
			{
				aboutFunction();
			}
			else if(control_name.equals(ACTION_ADDSAMPLE))
			{
				addSampleFunction();
			}
			else if(control_name.equals(ACTION_ADDCLASS))
			{
				addClassFunction();
			}
			else if(control_name.equals(ACTION_RECOGNIZE))
			{
				recognizeFunction();
			}
			else if(control_name.equals(ACTION_TRAIN))
			{
				train();
			}
			else if(control_name.equals(ACTION_DELETESAMPLE))
			{
				classes = LetterListClass.getClassList();
				int classID = classes.getSelectedIndex();
				
				if (classID < 0)
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please select a sample to delete.",ACTION_DELETESAMPLE, JOptionPane.OK_OPTION);
				//JOptionPane.
				else
				{
				//Added by Murali on 06/12/2006 for warning a message dialog in case of delete a class or sample
				//Request Id: 1608969
				int n = JOptionPane.showConfirmDialog(LipiTkUI.popupcontainer,"Do you want to delete sample?","Confirmation",JOptionPane.YES_NO_OPTION);
				if(n==0)
				{
					removeSample();
				}
				}
			}
			else if(control_name.equals(ACTION_DELETECLASS))
			{
				classes = LetterListClass.getClassList();
				int classID = classes.getSelectedIndex();
				
				if (classID < 0)
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please select a class to delete.",ACTION_DELETECLASS, JOptionPane.OK_OPTION);
				//JOptionPane.
				else
				{
				//Added by Murali on 06/12/2006 for warning a message dialog in case of delete a class or sample
				//Request Id: 1608969
				int n = JOptionPane.showConfirmDialog(LipiTkUI.popupcontainer,"Do you want to delete class?","Confirmation",JOptionPane.YES_NO_OPTION);
				if(n==0)
				{
					removeClass();
				}
				}
			}
			else if(control_name.equals(ACTION_CLEAR))
			{
				clearFunction();
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception from LipiTkActionClass action Performed Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::ActionPerformed Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: helpFunction
	 * DESCRIPTION	: Displays a Help LipiTK dialog UI
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void helpFunction()
	{
		try
		{
			System.out.println("Inside HelpTopics");
			LipiTKUtil.openDocument(LIPIUIController.Environment_variable + USERMANUAL_FILE);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception from LipiTkActionClass action Performed Function"+ ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: aboutFunction
	 * DESCRIPTION	: Displays a About LipiTK dialog UI
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void aboutFunction()
	{
		try
		{
			addDialog = new JDialog(new JFrame(), true);
			addDialog.setResizable(false);
			addDialog.setTitle("About Lipi Designer");
			((Frame)addDialog.getOwner()).setIconImage(LipiTKUtil.showImage("images/lipitk_ICO.jpg"));
			addDialog.setSize(450, 275);
			
			
			Container container = addDialog.getContentPane();
			addDialog.setLocationRelativeTo(container);
			String s1 = System.getProperty("os.name").toLowerCase();
			String s;
			if(s1.startsWith("windows"))
				s = "#E0DFE3";
			else
				s = "#EEEEEE";
			String s2 = (new StringBuilder()).append("<html><table bgcolor=").append(s).append("><tr><td><table><tr> <td><FONT size=5>Lipi Designer" +
					"</FONT></td></tr><tr><td><FONT size=2>Version 4.0</FONT></td></tr>" +
					"<tr><td><FONT size=2>Copyright (c) Hewlett Packard Development Company L.P." +
					"</FONT></td></tr><tr><td><FONT size=2><b>Credits</b></FONT></td></tr><tr><td>" +
					"<FONT size=2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nidhi Sharma</FONT></td></tr>" +
					"<tr><td><FONT size=2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Srinivasa Vithal Charakana</FONT></td>" +
					"</tr><tr><td><FONT size=2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Hewlett-Packard GDIC)" +
					"</FONT></td></tr><tr><td><FONT size=2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;BA Hari Krishna</FONT>" +
					"</td></tr><tr><td><FONT size=2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Hewlett-Packard Labs India)" +
			"</FONT></td></tr></table></html>").toString();
			JLabel jlabel = new JLabel(s2);
			jlabel.setFont(new Font("arial", 0, 5));
			jlabel.setHorizontalTextPosition(10);
			JLabel jlabel1 = new JLabel("");
			ImageIcon imageicon = new ImageIcon(getClass().getResource("images/lipitk_logo.jpg"));
			jlabel1.setIcon(imageicon);
			jlabel1.setHorizontalAlignment(0);
			JPanel jpanel = new JPanel();
			jpanel.setLayout(new BoxLayout(jpanel, 1));
			JPanel jpanel1 = new JPanel();
			jpanel1.setLayout(new BoxLayout(jpanel1, 0));
			jpanel1.add(jlabel);
			jpanel1.add(jlabel1);
			JButton jbutton = new JButton("Close");
			jbutton.setAlignmentY(1.0F);
			jbutton.setAlignmentX(0.0F);
			jbutton.setAlignmentX(0.7F);
			jpanel.add(jpanel1);
			jpanel.add(jbutton);
			container.add(jpanel);

			ActionListener actionlistener = new ActionListener()
			{
				public void actionPerformed(ActionEvent actionevent)
				{
					LipiTkUI.DEBUG("Action performed called");
					
					addDialog.setVisible(false);
				}
			};
			jbutton.addActionListener(actionlistener);
			addDialog.show();
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Create Function"+ ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: createFunction
	 * DESCRIPTION	: A new project is created
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void createFunction()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::Create Function");
		try
		{
			// If condition modified by Murali to fix the issue - 1611497 on 11/12/2006
			if(project_name == null || project_name.length() == 0)
			{
				//Changed the dialog box as modal dialog
				addDialog = new JDialog(new JFrame(), true);
				addDialog.setTitle("Create new project");
				addDialog.setSize(300, 80);
				Container container = addDialog.getContentPane();
				addDialog.setLocationRelativeTo(container);				
				
				JLabel jlabel = new JLabel("Enter the name of Project:  ");
				jlabel.setBackground(Color.WHITE);
				jlabel.setHorizontalAlignment(0);

				JPanel jp = new JPanel();
				jp.setLayout(new BoxLayout(jp, BoxLayout.Y_AXIS));

				JPanel jpanel = new JPanel();
				jpanel.setLayout(new BoxLayout(jpanel, BoxLayout.X_AXIS));

				jpanel.add(jlabel);
				writeproject_name = new JTextField();
				JButton okey = new JButton("OK");
				ActionListener actionlistener = new ActionListener()
				{
					public void actionPerformed(ActionEvent actionevent)
					{
						project_name  = writeproject_name.getText().trim();
						//Valdiating for empty project name
						if(project_name == null || project_name.equals(""))
						{
							// Modified by Murali - null value replaced by LipiTkUI.popupcontainer
							JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please enter project name","Type project name",JOptionPane.INFORMATION_MESSAGE);
							writeproject_name.requestFocusInWindow();
						} //Validating project name for white spaces

						/**To validate the name of a project
						 *characters like '>', '<', '?' have
						 *been checked for in the project name
						 */

						else if(project_name.indexOf(" ") != -1 || project_name.indexOf(',') != -1 || project_name.indexOf('>') != -1
								|| project_name.indexOf('<') != -1 ||project_name.indexOf('?') != -1 ||project_name.indexOf('\"') != -1
								|| project_name.indexOf(':') != -1 ||project_name.indexOf('|') != -1 ||project_name.indexOf('\\') != -1
								|| project_name.indexOf(';') != -1 ||project_name.indexOf('/') != -1 || project_name.indexOf('.') == (project_name.length()-1))
						{
							// Modified by Murali - null value replaced by LipiTkUI.popupcontainer
							JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please enter valid project name","Invalid project name",JOptionPane.INFORMATION_MESSAGE);
							project_name = null;
							writeproject_name.requestFocusInWindow();
						}
						else
						{
							if( (lipicontroller.checkProjectExists(project_name)))
							{
								addDialog.setVisible(false);
								lipicontroller.createNewProject(project_name);
								project_New = true;
								project_Load = false;
								LipiTkUI.setTitleName(project_name);
								project_dir = LIPIUIController.Environment_variable+"/projects/"+project_name+"/data/";
								project_config_dir = LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/default/";
								samples = LetterListSample.getSampleList();
								LetterListSample.setRoot(project_dir);
								train_happened = false;
								BottomPanel.setButtonStatus(true,false);
								LipiTkUI.setMenuItemStatus(true, true); //to disable the package menu item for a new project
							}
							else
							{
								// Modified by Murali - null value replaced by LipiTkUI.popupcontainer
								JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Project already exists with this name. Specify a different project name","Project already exists",JOptionPane.ERROR_MESSAGE);
								project_name = null;
								writeproject_name.requestFocusInWindow();
							}
						} //End of else
					}
				};
				okey.addActionListener(actionlistener);
				//Added action listener for text field.
				writeproject_name.addActionListener(actionlistener);
				jpanel.add(writeproject_name);
				jp.add(jpanel);
				jp.add(okey);
				container.add(jp);
				addDialog.show();
			}
			else
			{
				// Modified by Murali - null value replaced by LipiTkUI.popupcontainer
				JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Close current project before creating a new project","Close existing project",JOptionPane.ERROR_MESSAGE);
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Create Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::Create Function");
	}
	
	/**********************************************************************************
	 * AUTHOR		: Jitender Singh
	 * DATE			: 07-12-11
	 * NAME			: updateLipiengineConfig
	 * DESCRIPTION	: update lipiengine configuration, if user wants to save the project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	
	public static void updateLipiengineConfig()
	{
		FileInputStream configSTream = null;
		InputStreamReader inputStream = null;
        BufferedReader readBuffer = null;
		StringBuffer sKeyValue = new StringBuffer();
		File lipiengineConfig = new File(LIPIUIController.Environment_variable+"/projects/" + "lipiengine.cfg");
		System.out.println("lipiengine.cfg : "+ lipiengineConfig);
		
		String sCapsProjectName = new String(project_name.toUpperCase());
		String sTextBuffer = new String("SHAPEREC_" + sCapsProjectName + "=" + project_name + "(default)");;
		System.out.println("entry in lipiengine.cfg : "+ sCapsProjectName);
		String textinLine;
		
		try {
		
				configSTream = new FileInputStream(lipiengineConfig);		
				inputStream = new InputStreamReader(configSTream);
				readBuffer = new BufferedReader(inputStream);
		
		       while( null != (textinLine=readBuffer.readLine()))
		       {
		    	   if( textinLine.contains(sTextBuffer))
				      {
				    	  System.out.println("logical name in lipiengine.cfg : "+ sTextBuffer);
				    	  JOptionPane.showMessageDialog(LipiTkUI.popupcontainer, "Logical name already exists in lipiengine.cfg, Project files saved.");
				    	  //showMessageDialog(LipiTkUI.popupcontainer,"Logical name already exists.",JOptionPane.);
				    	  //for(int nCount =0; nCount < 256; nCount++)
				    		  //sTextBuffer += "_" + nCount;
				      }
		          
		           if(0 == textinLine.length())
		               break;
		           
		           sKeyValue.append(textinLine);
		           sKeyValue.append("\n");
		       }		
				
		      
		    	  
		       //sKeyValue.append("\n");
		       System.out.println("appending logical name in lipiengine.cfg : "+ sTextBuffer);
		       sKeyValue.append(sTextBuffer);
		       
		       configSTream.close();
		       inputStream.close();
		       readBuffer.close();
		       
			} 
		catch (FileNotFoundException e) 
		{
	         e.printStackTrace();
	         System.out.println("appending logical name in lipiengine.cfg : FileNotFoundException");
	    } 
		catch (IOException e) 
	    {
	        e.printStackTrace();
	        System.out.println("appending logical name in lipiengine.cfg : IOException");
	    }
		
		try
		{
		   FileWriter fileWrite = new FileWriter(lipiengineConfig);
	       BufferedWriter writeBuffer = new BufferedWriter(fileWrite);
	       System.out.println("writing logical name in lipiengine.cfg : "+ sKeyValue.toString());
	       writeBuffer.write(sKeyValue.toString());
	       writeBuffer.close();
		}
		catch (Exception e)
		{
			System.out.println("appending logical name in lipiengine.cfg : FileWriter");
        	System.err.println("Error: " + e.getMessage());
        }
		
	}
	
	/**********************************************************************************
	 * AUTHOR		: Jitender Singh
	 * DATE			: 09-30-11
	 * NAME			: deleteProjectLipiengineConfig
	 * DESCRIPTION	: remove project name from lipiengine configuration, if user wants to delete the project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	
	public static void deleteProjectLipiengineConfig()
	{
		FileInputStream configSTream = null;
		InputStreamReader inputStream = null;
        BufferedReader readBuffer = null;
		StringBuffer sKeyValue = new StringBuffer();
		File lipiengineConfig = new File(LIPIUIController.Environment_variable+"/projects/" + "lipiengine.cfg");
		
		String sCapsProjectName = new String(project_name.toUpperCase());
		String sTextBuffer = new String("SHAPEREC_" + sCapsProjectName);
		
		String textinLine = "lipiengine.cfg";
		
		try {
		
				configSTream = new FileInputStream(lipiengineConfig);		
				inputStream = new InputStreamReader(configSTream);
				readBuffer = new BufferedReader(inputStream);
		
		       while(true)
		       {
		           textinLine=readBuffer.readLine();
		           
		           if(null == textinLine)
		        	   break;
		           
		           
		           if( textinLine.contains(sTextBuffer))
		               continue;
		           
		           sKeyValue.append(textinLine);
		           sKeyValue.append("\n");
		       }
		       
		       configSTream.close();
		       inputStream.close();
		       readBuffer.close();
		       
			} 
		catch (FileNotFoundException e) 
		{
	         e.printStackTrace();
	    } 
		catch (IOException e) 
	    {
	        e.printStackTrace();
	    }
		
		try
		{
		   FileWriter fileWrite = new FileWriter(lipiengineConfig);
	       BufferedWriter writeBuffer = new BufferedWriter(fileWrite);
	       writeBuffer.write(sKeyValue.toString());
	       writeBuffer.close();
		}
		catch (Exception e)
		{
        	System.err.println("Error: " + e.getMessage());
        }
		
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: closeFunction
	 * DESCRIPTION	: Closes the current project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public void closeFunction()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::Close Function");
		try
		{
			lipicontroller.closeProject();
			LipiTkUI.setTitleName("");
			classes = LetterListClass.getClassList();
			samples = LetterListSample.getSampleList();
			resultID = RecognizedResults.getResultListID();
			String [] empty_list ={};
			classes.setListData(empty_list);
			samples.setListData(empty_list);
			resultID.setListData(empty_list);
			TopPanel.getDisplay().clean();
			(BottomPanel.getDrawEntry()).clean();
			project_name = null;
			project_dir = null;
			project_config_dir= null;
			train_happened = false;
			project_New = false;
			project_Load = false;
			
			//loadFile.close();
			LipiTkUI.setMenuItemStatus(true, true); // to disable close menuitem
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:: Exception in LipiTkAction class from Close Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::Close Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: loadFunction
	 * DESCRIPTION	: Loads an already existing project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void loadFunction()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::Load Function");

		try
		{
			// If condition modified by Murali to fix the issue - 1611497 on 11/12/2006
			if(project_name == null || project_name.length() == 0)
			{
				ChooseDialog cdg = new ChooseDialog();
				String selected_file =  cdg.choose(); 
				
				if(selected_file == LIPIUIController.Environment_variable + "\\projects\\")
					JOptionPane.showMessageDialog(
									LipiTkUI.popupcontainer,
									"Please select a project to load.",
									"Lipi Designer 4.0",
									JOptionPane.ERROR_MESSAGE);
				
				if(selected_file == null || selected_file.length() == 0)
				return;
				
				
				selected_file += "/config/project.cfg";
				System.out.println(selected_file);
				File loadFile = new File(selected_file);
				if(!loadFile.exists()) {
					JOptionPane.showMessageDialog(
									LipiTkUI.popupcontainer,
									"Project does not exist.",
									"Lipi Designer 4.0",
									JOptionPane.ERROR_MESSAGE);
				} else {
					if(selected_file != "") {
						String [] split_text = selected_file.split(CONFIG); //[C:\Lipi\projects\demonumerals\, \project.cfg]
						selected_project = split_text[0]+"data"; //C:\Lipi\projects\demonumerals\data
	
						if(null ==(project_name = readProjectCFG(selected_file)))
							{
								JOptionPane.showMessageDialog(LipiTkUI.popupcontainer, "Selected project is not supported by Lipi Designer, Please choose another project.");
							 	closeFunction();
							 	return;
							//(LipiTkUI.popupcontainer,"Please enter project name","Type project name",JOptionPane.INFORMATION_MESSAGE);
							}
							
						LipiTkUI.DEBUG("Project Name"+project_name);
						project_dir = LIPIUIController.Environment_variable+"/projects/"+project_name+"/data/";
						project_config_dir=LIPIUIController.Environment_variable+"/projects/"+project_name+"/config/default/";
	
						classes = LetterListClass.getClassList();
						samples = LetterListSample.getSampleList();
						LetterListSample.setRoot(project_dir);
						String [] classIDs = null;
						classIDs = lipicontroller.loadProject(project_name);
						project_Load = true;
						project_New = false;
						LipiTkUI.setTitleName(project_name);
						train_happened = false;
						LipiTkUI.setMenuItemStatus(true, true); //to enable/disable the package menu item
						if(StrokeFileManager.checkModalDataExists(project_name))
						{
							train_happened = true;
							BottomPanel.setButtonStatus(true,true);
						}
						else
						{
							train_happened = false;
							BottomPanel.setButtonStatus(true,false);
						}
						if( (classIDs != null) && (classIDs.length != 0))
						{
							classes.setListData(classIDs);
							classes.setSelectedIndex(0);
							LipiTkUI.DEBUG("Path for Setting SampleListbox**************"+project_dir+classIDs[0]);
							samples.setListData(removeExtension(lipicontroller.readInkDataFromFileSystem(project_dir+classIDs[0])));
							samples.setSelectedIndex(0);
						}
					} 
				}
			} else
			{
				// Modified by Murali - null value replaced by LipiTkUI.popupcontainer
				JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Close current project before loading a new project","Close current project",JOptionPane.ERROR_MESSAGE);
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from LoadFunction"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::Load Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: removeExtension
	 * DESCRIPTION	: Extension is removed
	 * ARGUMENTS	: samples - array of the samples before the removal of extension
	 * RETURNS		: samples - array of the samples after the removal of extension
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private String[] removeExtension(String samples[])
	{
		int length = samples.length;
		LipiTkUI.DEBUG("Length:"+length);

		for(int i=0;i<length;i++)
		{
			LipiTkUI.DEBUG(samples[i]);
			samples[i] = samples[i].substring(0,(samples[i].lastIndexOf(".")));

			LipiTkUI.DEBUG("*******Seperated File:"+ samples[i]);
		}

		return samples;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: exitFunction
	 * DESCRIPTION	: Exits the LIPITK DESIGNER
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public void exitFunction()
	{
		lipicontroller.exitProject();
		System.exit(0);
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: addSampleFunction
	 * DESCRIPTION	: Adds new sample into project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void addSampleFunction()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::AddSample Function");

		try
		{
			// Checking for the existance of project
			if(project_name == null || project_name.equals(""))
			{
				// Modified by Murali - null value replaced by LipiTkUI.popupcontainer
				JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please create a project before adding sample","Create project",JOptionPane.ERROR_MESSAGE);
			}
			else
			{
				strokes = DataEntry.getLipiTKStroke();
				if(strokes.isEmpty())
				{
					// Modified by Murali on 06/12/2006 - null value replaced by LipiTkUI.popupcontainer
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please draw sample shape in writing area","Add sample",JOptionPane.INFORMATION_MESSAGE);
				}
				else
				{

					classes = LetterListClass.getClassList();
					if(classes.getModel().getSize () != 0)
					{
						int classID = classes.getSelectedIndex();
						String cName = (String)classes.getModel().getElementAt(classID);
						String path = project_dir+cName;
						if(BottomPanel.getTrainMode())
						{
							LipiTkUI.DEBUG("*******Train Mode is Auto***********");
						}
						else
						{
							LipiTkUI.DEBUG("*******Train Mode is Manual***********");
						}
						lipicontroller.addStroke(project_name,strokes,path,cName,"Add Sample",BottomPanel.getTrainMode());

						if(BottomPanel.getTrainMode())
							train_happened = true;
						else
							train_happened = false;
						LipiTkUI.setMenuItemStatus(false, true);
						BottomPanel.setButtonStatus(false,true);
						samples = LetterListSample.getSampleList();
						samples.setListData(removeExtension(lipicontroller.readInkDataFromFileSystem(path)));
						samples.setSelectedIndex(samples.getModel().getSize () - 1);

					} //End of class check
					else
					{
						// Modified by Murali on 06/12/2006 - null value replaced by LipiTkUI.popupcontainer
						JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please add class before adding sample","Add class",JOptionPane.INFORMATION_MESSAGE);
					}
				} // End of empty signal check else
			} //End of project check else
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from AddSample Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::AddSample Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: addClassFunction
	 * DESCRIPTION	: Adds new class into project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void addClassFunction()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::AddClass Function");
		try
		{
			// Checking for the existance of project
			if(project_name == null || project_name.equals(""))
			{
				// Modified by Murali on 06/12/2006 - null value replaced by LipiTkUI.popupcontainer
				JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please create project before adding class","Create project",JOptionPane.ERROR_MESSAGE);
			}
			else
			{
				classes = LetterListClass.getClassList();
				samples = LetterListSample.getSampleList();
				//Modified the Jdialog box as modal
				addDialog = new JDialog(new JFrame(), true);
				addDialog.setTitle("Enter new class name");
				addDialog.setSize(320, 80);
				Container container = addDialog.getContentPane();
				addDialog.setLocationRelativeTo(container);
				JLabel jlabel = new JLabel("Specify a name for the class:  ");
				jlabel.setBackground(Color.WHITE);
				jlabel.setHorizontalAlignment(0);
				JPanel jp = new JPanel();
				jp.setLayout(new BoxLayout(jp, BoxLayout.Y_AXIS));
				JPanel jpanel = new JPanel();
				jpanel.setLayout(new BoxLayout(jpanel, BoxLayout.X_AXIS));
				jpanel.add(jlabel);
				writeclassName = new JTextField();
				writeclassName.setMaximumSize(new Dimension(120,20));
				JButton okey = new JButton("OK");

				strokes = DataEntry.getLipiTKStroke();
				ActionListener actionlistener = null;
				//Checking for the existance of signal
				if(strokes.isEmpty())
				{
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please draw sample shape in writing area","Add sample",JOptionPane.INFORMATION_MESSAGE);
				}
				else
				{
					actionlistener = new ActionListener()
					{
						public void actionPerformed(ActionEvent actionevent)
						{
							LipiTkUI.DEBUG("Enter From OKAction Function");

							class_name = writeclassName.getText().trim();

							/**To validate the name of a class
							 *characters like '>', '<', '?' have
							 *been checked for in the class name
							 */

							if(class_name == null || class_name.equals("") || class_name.indexOf(',') != -1 || class_name.indexOf('>') != -1 || class_name.indexOf('<') != -1 ||class_name.indexOf('?') != -1 ||class_name.indexOf('\"') != -1 || class_name.indexOf(':') != -1 ||class_name.indexOf('|') != -1 ||class_name.indexOf('\\') != -1 ||class_name.indexOf('/') != -1 || class_name.indexOf(' ') !=-1 || class_name.indexOf('.') !=-1)
							{
								// Modified by Murali on 06/12/2006 - null value replaced by LipiTkUI.popupcontainer
								JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please enter valid class name","Invalid class name",JOptionPane.INFORMATION_MESSAGE);
								writeclassName.requestFocusInWindow();
							}
							else if(checkClassExists(class_name))
							{
								// Modified by Murali on 06/12/2006 - null value replaced by LipiTkUI.popupcontainer
								JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Entered class name already exists. Specify a different class name","Class name already exists",JOptionPane.INFORMATION_MESSAGE);
								writeclassName.requestFocusInWindow();
							}
							else
							{
								String pathTo_Write = project_dir+class_name;
								String classList_dir = project_dir;
								lipicontroller.addStroke(project_name,strokes,pathTo_Write,class_name,"Add Class",BottomPanel.getTrainMode());
								if(BottomPanel.getTrainMode())
									train_happened = true;
								else
									train_happened = false;
								LipiTkUI.setMenuItemStatus(false, true);
								BottomPanel.setButtonStatus(false,true);
								addclass_flag = true;
								addDialog.setVisible(false);

								classes.setListData(lipicontroller.readInkDataFromFileSystem(classList_dir));
								classes.setSelectedIndex(lipicontroller.getSelectedClassIndex(classList_dir,class_name));
								int classID = classes.getSelectedIndex();
								String cName = (String)classes.getModel().getElementAt(classID);
								samples.setListData(removeExtension(lipicontroller.readInkDataFromFileSystem(classList_dir+cName)));
								samples.setSelectedIndex(samples.getModel().getSize () - 1);
							}
							LipiTkUI.DEBUG("Exit From OKAction Function");

						}

					};
					okey.addActionListener(actionlistener);
					//Added action listener for text field.
					writeclassName.addActionListener(actionlistener);
					jpanel.add(writeclassName);
					jp.add(jpanel);

					jp.add(okey);
					container.add(jp);
					addDialog.show();
				} //End of else part
			} // End of project check else part
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from AddClass Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::AddClass Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE		  	: 18-08-06
	 * NAME			: checkClassExists
	 * DESCRIPTION	:
	 * ARGUMENTS	: className - name of the class to be checked
	 * RETURNS		: true if class exists & false if not
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private boolean checkClassExists(String className)
	{
		classes = LetterListClass.getClassList();
		boolean flag = false;
		for(int i = 0;i<classes.getModel().getSize ();i++)
		{
			if(className.equals(classes.getModel().getElementAt(i)))

			{
				flag = true;
				break;
			}
		}

		return flag;
	}

	/**********************************************************************************
	 * AUTHOR		: Srinivasa Vithal, Hari Krishna
	 * DATE			: 19-03-08
	 * NAME			: packageExists
	 * DESCRIPTION	:
	 * ARGUMENTS	: Package Name - name of the Package to be checked
	 * RETURNS		: true if Package exists & false if not
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private boolean packageExists(String pkgName)
	{
		boolean flag = false;
		File f = new File(LIPIUIController.Environment_variable + "/package");
		
		//boolean bExists = ;
		if(!f.exists())
			f.mkdir();
			
		File[] fList = f.listFiles(); //list of all files in the Package directory

		String s = System.getProperty(PROPERTY_NAME);
		if (s.startsWith(WINDOWS))
		{
			pkgName = pkgName + WINDOWS_PKGEXTENSION;
			System.out.println("\n Package name : " + pkgName);
		}
		else
		{
			pkgName = pkgName + LINUX_PKGEXTENSION;
		}

		for(int i = 0; i < fList.length ; i++)
		{
			//System.out.println(fList[i].getName() + '-' + pkgName);
			if(fList[i].getName().toString().equals(pkgName))
			{
				//System.out.println(pkgName + "found");
				flag = true;
				break;
			}
		}
		return flag;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: recognizeFunction
	 * DESCRIPTION	: When recognize button is pressed, this part of the code executes and calls the LIPITK
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void recognizeFunction()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::Recognize Function");
		String class_name;
		classes = LetterListClass.getClassList();
		samples = LetterListSample.getSampleList();

		try
		{
			// Checking for the existance of project
			if(project_name == null || project_name.equals(""))
			{
			}
			else
			{
				if((classes.getModel().getSize () != 0) && (samples.getModel().getSize ()) != 0)
				{
					strokes = DataEntry.getLipiTKStroke();
					//Checking for the existance of signal
					if(strokes.isEmpty())
					{
						// Modified by Murali on 07/12/2006 - null value replaced by LipiTkUI.popupcontainer
						JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"No sample data to recognize. Draw sample data on writing area","Draw sample",JOptionPane.INFORMATION_MESSAGE);
					}
					else
					{
						if(train_happened)
							doAction(strokes);
						else
						{
							if(lipicontroller.checkModalDataExists(project_name))
								doAction(strokes);
							else
							{
								// Modified by Murali on 07/12/2006 - null value replaced by LipiTkUI.popupcontainer
								JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Trained modal data doesn't exits. Click on 'Train' button to train the data","Trained modal mata doesn't exsits",JOptionPane.ERROR_MESSAGE);
							}
						}
					} // End of else - LipiTKStroke check
				}
				else
				{
					// Modified by Murali on 07/12/2006 - null value replaced by LipiTkUI.popupcontainer
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"No trained data samples. Add data samples and train","Empty data samples",JOptionPane.ERROR_MESSAGE);
				}
			} //End of project check else part
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from RecognizeFunction"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::Recognize Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: doAction
	 * DESCRIPTION	: Displays the results, recognized by LIPITK, in the UI
	 * ARGUMENTS	: strokes - stroke that is to be recognized
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void doAction(LipiTKStroke strokes)
	{
		String class_name;
		lipitkresult = lipicontroller.recognize(strokes);

		/**No of results to be displayed on the screen has been changed to a
		 *the size of the array - previously the value '5' was used
		 */

		String results [] = new String[lipitkresult.length];
		for(int i = 0;i< lipitkresult.length;i++)
		{
			results [i] = new String();

			int id = lipitkresult[i].getId();

			//Rounding float value to 2 decimal
			float confidence = LipiTKUtil.round(lipitkresult[i].getConfidence(),2);
			if( id != -1)
			{
				class_name = lipicontroller.getSymbolName(id,project_config_dir);
				results[i] = "ID:  " + class_name + "   Confidence:"+ confidence;
			}

			LipiTkUI.DEBUG("id:"+id+"confidence:"+confidence);
		}
		resultID = RecognizedResults.getResultListID();
		String[] empty_list = { };
		resultID.setListData(empty_list);
		resultID.setListData(results);
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: train
	 * DESCRIPTION	:
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void train()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::Train Function");
		try
		{
			//Checking for the existance of project
			if(project_name == null || project_name.equals(""))
			{
				// Modified by Murali on 07/12/2006 - null value replaced by LipiTkUI.popupcontainer
				JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please create project to train sample data","Create project",JOptionPane.ERROR_MESSAGE);
			}
			else
			{
				classes = LetterListClass.getClassList();
				samples = LetterListSample.getSampleList();
				if((classes.getModel().getSize () != 0) && (samples.getModel().getSize ()) != 0)
				{
					lipicontroller.train(project_name);
					train_happened = true;
					LipiTkUI.setMenuItemStatus(false, true); //to enable/disable the package menu item
					BottomPanel.setButtonStatus(false,true);

				}
				else
				{
					// Modified by Murali on 07/12/2006 - null value replaced by LipiTkUI.popupcontainer
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"No trained data samples. Add data samples and train","Empty data samples",JOptionPane.INFORMATION_MESSAGE);
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Train Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::Train Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: removeClass
	 * DESCRIPTION	: Removes selected class from the project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void removeClass()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::RemoveClass Function");
		try
		{
			classes = LetterListClass.getClassList();
			samples = LetterListSample.getSampleList();

			if((classes.getModel().getSize ()) != 0)
			{
				String [] list_data;
				int classID= classes.getSelectedIndex();
				String cName = (String)classes.getModel().getElementAt(classID);
				String path = project_dir+cName;
				lipicontroller.deleteStroke(project_name,cName,path,null,"Delete Class",BottomPanel.getTrainMode());
				if(BottomPanel.getTrainMode())
					train_happened = true;
				else
					train_happened = false;
				LipiTkUI.setMenuItemStatus(false, true);
				BottomPanel.setButtonStatus(false,true);
				list_data = lipicontroller.readInkDataFromFileSystem(project_dir);
				if(( classID != 0) || ((classes.getModel().getSize () - 1) != 0))
				{
					if(( classID == classes.getModel().getSize () -1))
					{
						classes.setListData(list_data);
						classes.setSelectedIndex(classID-1);
						cName = (String)classes.getModel().getElementAt(classID-1);
					}
					else
					{
						classes.setListData(list_data);
						classes.setSelectedIndex(classID);
						cName = (String)classes.getModel().getElementAt(classID);
					}

					samples.setListData(removeExtension(lipicontroller.readInkDataFromFileSystem(project_dir+cName)));
					samples.setSelectedIndex(0);
				}
				else
				{
					String [] empty_list ={};
					classes.setListData(empty_list);
					samples.setListData(empty_list);
					//Added to clean the visualization window
					TopPanel.getDisplay().clean();
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Remove Class Function:" + ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::RemoveClass Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: removeSample
	 * DESCRIPTION	: Removes selected sample from the project
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void removeSample()
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::RemoveSample Function");

		try
		{
			classes = LetterListClass.getClassList();
			
			samples = LetterListSample.getSampleList();

			if((classes.getModel().getSize () != 0) && (samples.getModel().getSize ()) != 0)
			{
				int classID = classes.getSelectedIndex();
				String cName = (String)classes.getModel().getElementAt(classID);

				int sampleID = samples.getSelectedIndex();
				if(sampleID == -1)
				{
					// Modified by Murali on 07/12/2006 - null value replaced by LipiTkUI.popupcontainer
					JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please select the sample id to remove","Select sample",JOptionPane.INFORMATION_MESSAGE);
				}
				else
				{
					String sName = (String)samples.getModel().getElementAt(sampleID);
					String path = project_dir+cName;
					lipicontroller.deleteStroke(project_name,cName,path,sName,"Delete Sample",BottomPanel.getTrainMode());
					if(BottomPanel.getTrainMode())
						train_happened = true;
					else
						train_happened = false;
					LipiTkUI.setMenuItemStatus(false,true);
					BottomPanel.setButtonStatus(false,true);
					samples.setListData(removeExtension(lipicontroller.readInkDataFromFileSystem(project_dir+cName)));
					int list_length = (samples.getModel().getSize ());
					if(sampleID == list_length)
						samples.setSelectedIndex(sampleID - 1);
					else
						samples.setSelectedIndex(sampleID);
					//Deleting the class if corresponding sample ids are deleted
					if(list_length == 0)
					{
						File file = new File(project_dir+cName);
						file.delete();
						classes.setListData(lipicontroller.readInkDataFromFileSystem(project_dir));
						samples.setSelectedIndex(0);
						TopPanel.getDisplay().clean();
					}
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Remove Sample Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::RemoveSample Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: readProjectCFG
	 * DESCRIPTION	: Reads the project configuration file and returns the name of the project
	 * ARGUMENTS	: file_path - path of the project config file
	 * RETURNS		: name of the project
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private String readProjectCFG(String file_path)
	{
		LipiTkUI.DEBUG("Enter Into LipiTKAction Class::ReadProjectCFG Function");
		String proj_name = null;
		try
		{
			File fp = new File(file_path);
			BufferedReader br = new BufferedReader(new FileReader(fp));
			String line = null;

			while((line=br.readLine())!=null)
			{
				String []split_part = line.split("=");
				if((split_part[0].compareTo("ProjectName ")) == 0)
				{
					int start,end;
					String temp = split_part[1];
					start = temp.indexOf("\"");
					end = temp.lastIndexOf("\"");
					proj_name = split_part[1].substring(start+1,end);
				}
			}
			br.close(); //added by Jitender, to delete project if user chooses not to save. 25/8/2011
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from ReadProjectCFG"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From LipiTKAction Class::ReadProjectCFG Function");
		return proj_name;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: clearFunction
	 * DESCRIPTION	: The Writing area & Results field of the UI are cleared
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void clearFunction()
	{
		try
		{
			(BottomPanel.getDrawEntry()).clean();
			String [] empty_list = {};
			resultID = RecognizedResults.getResultListID();
			resultID.setListData(empty_list);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Clear Function"+ ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Srinivasa Vithal, Hari Krishna
	 * DATE			: 19-03-08
	 * NAME			: createPackage
	 * DESCRIPTION	: Packages the PreBuild Recognizer
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	private void createPackage()
	{
		try
		{
			if(project_name == null || project_name.equals(""))
			{
				JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,
						"Please create project and train with sample data or load project to Package","Package Error",
						JOptionPane.ERROR_MESSAGE);
			}
			else
			{
				addDialog = new JDialog(new JFrame(), true);
				addDialog.setTitle("Enter Package name");
				addDialog.setSize(320, 80);
				Container container = addDialog.getContentPane();
				addDialog.setLocationRelativeTo(container);
				JLabel jlabel = new JLabel("Specify a name for the package:  ");
				jlabel.setBackground(Color.WHITE);
				jlabel.setHorizontalAlignment(0);
				JPanel jp = new JPanel();
				jp.setLayout(new BoxLayout(jp, BoxLayout.Y_AXIS));
				JPanel jpanel = new JPanel();
				jpanel.setLayout(new BoxLayout(jpanel, BoxLayout.X_AXIS));
				jpanel.add(jlabel);
				writePackageName = new JTextField();
				writePackageName.setMaximumSize(new Dimension(120,20));
				writePackageName.setText("user_lipi_reco_");
				JButton okey = new JButton("OK");
				
				
				

				ActionListener actionlistener = null;

				actionlistener = new ActionListener()
				{
					public void actionPerformed(ActionEvent actionevent)
					{

						packageName = writePackageName.getText().trim().toUpperCase();
						//packageName = "USER_" + packageName.toUpperCase();

						/**To validate the name of a class
						 *characters like '>', '<', '?' have
						 *been checked for in the class name
						 */

						if(packageName == null || packageName.equals("") || packageName.indexOf(' ') !=-1 || packageName.indexOf(',') != -1 || packageName.indexOf('>') != -1 ||
								packageName.indexOf('<') != -1 ||packageName.indexOf('?') != -1 ||packageName.indexOf('\"') != -1 || packageName.indexOf(':') != -1 || packageName.indexOf(';') != -1 ||
								packageName.indexOf('|') != -1 ||packageName.indexOf('\\') != -1 ||packageName.indexOf('/') != -1 || packageName.indexOf('.') !=-1 || packageName.indexOf('-') !=-1)
						{
							// Modified by Murali on 06/12/2006 - null value replaced by LipiTkUI.popupcontainer
							JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please enter valid Package name","Invalid Package name",JOptionPane.INFORMATION_MESSAGE);
							packageName = null;
							writePackageName.requestFocusInWindow();
						}
						else if(packageExists(packageName))
						{
							JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Package name already exists. Specify a different name","Package name already exists",JOptionPane.INFORMATION_MESSAGE);
							packageName = null;
							writePackageName.requestFocusInWindow();
						}
						else
						{
							Process process = null;
							BufferedReader bufferReader = null;
							String line = "";
							boolean flag = true;
							try
							{
								addDialog.setVisible(false);
								System.out.println("Packaging the Prebuilt Recognizer with perl script.....");
								String s = System.getProperty(PROPERTY_NAME);
								if (s.startsWith(WINDOWS))
								{
									//process = Runtime.getRuntime().exec("cmd.exe /c cabarc")
									//String perlScript = "cmd /c " + LIPIUIController.Environment_variable + "/scripts/createrecoaddon.pl " + "-project " + project_name + " -logicalname " + packageName;
									String batFile = "cmd /c " + LIPIUIController.Environment_variable + "/scripts/createAddon.bat " + project_name + " " + packageName;
									System.out.println(batFile);
									try 
									{
										process = Runtime.getRuntime().exec(batFile);
									} catch (Exception e) 
									{
										
										//ioe.printStackTrace();
										System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Package test  Function"+ e.toString());
									}
									bufferReader = new BufferedReader( new InputStreamReader( process.getInputStream() ) );
									//line = bufferReader.readLine();
									while ((line = bufferReader.readLine()) != null) {
						                System.out.println(line);
										}
									/*if(line == null)
									{
										JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"Please install CABSDK to create a package","CABSDK is not installed",JOptionPane.ERROR_MESSAGE);
										flag = false;
									}*/
								}
								else
								{
									//String scriptFile = "perl " + LIPIUIController.Environment_variable + "/scripts/createrecoaddon.pl " + "-project " + project_name + " -logicalname " + packageName;

									String scriptFile = "sh " + LIPIUIController.Environment_variable + "/scripts/createAddon.sh " + project_name + " " + packageName;
									System.out.println(scriptFile);
									try 
									{
										process = Runtime.getRuntime().exec(scriptFile);
									} catch (Exception e) 
									{
										
										//ioe.printStackTrace();
										System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Package test  Function"+ e.toString());
									}
									bufferReader = new BufferedReader( new InputStreamReader( process.getInputStream() ) );
									//line = bufferReader.readLine();
									while ((line = bufferReader.readLine()) != null) 
									{
						                System.out.println(line);
									}
								}
									

								if(flag)
								{
									if(packageExists(packageName))
									{
										System.out.println("Completed successfully");
										JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"The package is available under "+LIPIUIController.Environment_variable + "/package directory","Package created successfully",JOptionPane.INFORMATION_MESSAGE);
									}
									else
										JOptionPane.showMessageDialog(LipiTkUI.popupcontainer,"The package could not be created.","Error in creating package",JOptionPane.ERROR_MESSAGE);
								}
							}
							catch (Exception e)
							{
								System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Package Function"+ e.toString());
							}

						}
					}

				};
				okey.addActionListener(actionlistener);
				//Added action listener for text field.
				writePackageName.addActionListener(actionlistener);
				jpanel.add(writePackageName);
				jp.add(jpanel);

				jp.add(okey);
				container.add(jp);
				addDialog.show();
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface:Exception in LipiTkActionClass from Package Function"+ ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getDataDirectory
	 * DESCRIPTION	:
	 * ARGUMENTS	: None
	 * RETURNS		: path to the data directory of the project
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static String getDataDirectory()
	{
		return project_dir;
	}
	
	
	/**********************************************************************************
	 * AUTHOR		: Jitender Singh
	 * DATE			: 24-08-2011
	 * NAME			: removeDirectory
	 * DESCRIPTION	:
	 * ARGUMENTS	: path of the firectory
	 * RETURNS		: success/failure
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Description : remove project if user selects not to save the project
	 *************************************************************************************/
	
	public static boolean removeDirectory(String projectName) 
	{			
		 File directory = new File(projectName);		  
		  
		  if (!directory.exists())
		    return true;
		  
		  if (!directory.isDirectory())
		    return false;
		  
		  System.out.println("removeDirectory " + directory);

		  String[] list = directory.list();

		  // Some JVMs return null for File.list() when the
		  // directory is empty.
		  if (list != null) 
		  {
		    for (int i = 0; i < list.length; i++) 
		    {
		      File entry = new File(directory, list[i]);

		      //System.out.println("\tremoving entry " + entry);
		      

		      if (entry.isDirectory())
		      {
		        if (!removeDirectory(entry.getAbsolutePath()))
		          return false;
		      }
		      else
		      {
		        if (!entry.delete())
		          return false;
		      }
		    }
		  }

		  return directory.delete();
	}
	

	private JTextField writeclassName,writeproject_name,writePackageName;
	private JDialog addDialog;
	private LipiTKStroke strokes;
	private static String project_dir,project_config_dir;
	public static String project_name;
	//public static File loadFile;
	
	private String selected_project;
	private JList classes,samples,resultID;
	public static boolean addclass_flag;
	public static boolean train_happened = true;
	private LipitkResult lipitkresult[] = null;
	private boolean project_Load,project_New;
	private String class_name = null, packageName = null;

	private final String ACTION_NEW = "New";
	private final String ACTION_LOAD = "Load";
	private final String ACTION_CLOSE = "Close";
	// Added by Jitender 09/30/11
	private final String ACTION_SAVE = "Save";
	private final String ACTION_DELETE = "Delete";
	private final String ACTION_EXIT = "Exit";
	private final String ACTION_PACKAGE = "Package";
	private final String ACTION_HELP = "Help Topics";
	private final String ACTION_ABOUT = "About Lipi Designer";
	private final String ACTION_ADDCLASS = "Add Class";
	private final String ACTION_ADDSAMPLE = "Add Sample";
	private final String ACTION_DELETESAMPLE = "Delete Sample";
	private final String ACTION_DELETECLASS = "Delete Class";
	private final String ACTION_RECOGNIZE = "Recognize";
	private final String ACTION_TRAIN = "Train";
	private final String ACTION_CLEAR = "Clear";

	private final String USERMANUAL_FILE = "/doc/pdf/lipi-ide_2_1_User_Manual.pdf";
	private final String CONFIG = "config";
	private final String WINDOWS_PKGEXTENSION = ".zip";
	private final String LINUX_PKGEXTENSION = ".zip";
	private final String PROPERTY_NAME = "os.name";
	private final String WINDOWS = "Windows";
	
	public static boolean bNewOrLoad = false; // true if new, false if loaded Added by Jitender 7/12/2011
	public static LIPIUIController lipicontroller;

	
}

