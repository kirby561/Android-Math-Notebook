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
 * $LastChangedDate: 2011-03-31 12:23:24 +0530 (Thu, 31 Mar 2011) $
 * $Revision: 841 $
 * $Author: mnab $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR	: LipiTkUI  is a class which creates the main UI frame
 * CONTENTS		:
 *			itemStateChanged
 *			main
 *			getLipiTkActionRef
 *			setTitleName
 *			DEBUG
 *          setMenuItemStatus
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JSplitPane;
import javax.swing.UIManager;

public class LipiTkUI extends JFrame implements ItemListener, WindowListener
{

	static LipiTkActionClass lipiac;
	//Added by Murali on 06/12/2006 for warning a message dialog in case of delete a class or sample
	//Request Id: 1608969
	static Container popupcontainer;

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: LipiTkUI
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Creates the controls for UI
	 *  			  Creates the instance of LipiTkController
	 *  			  Creates the instance of LipiTKAction class
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public LipiTkUI()
	{
		try
		{
			setResizable(false);
			JMenuBar jmenubar = new JMenuBar();
			JMenu project = new JMenu("Project");
			JMenu help = new JMenu("Help");
			JCheckBoxMenuItem debuginfo = new JCheckBoxMenuItem("Debug Information");
			JMenuItem helptopics = new JMenuItem("Help Topics");
			JMenuItem about = new JMenuItem("About Lipi Designer");
			newp = new JMenuItem("New");
			closep = new JMenuItem("Close");
			// added by Jitender 09/30/11 to add new menu items
			save = new JMenuItem("Save");
			delete = new JMenuItem("Delete");
			exit = new JMenuItem("Exit");
			load = new JMenuItem("Load");
			createPackage = new JMenuItem("Package");

			debug_status = false;

			LIPIUIController lipicontroller = new LIPIUIController();
			LipiTkActionClass lipiaction = new LipiTkActionClass(lipicontroller);

			lipiac = lipiaction;

			newp.addActionListener(lipiac);
			closep.addActionListener(lipiac);
			load.addActionListener(lipiac);
			createPackage.addActionListener(lipiac);
			// added by Jitender 09/30/11 to add new menu items
			exit.addActionListener(lipiac);
			save.addActionListener(lipiac);
			delete.addActionListener(lipiac);
			about.addActionListener(lipiac);
			helptopics.addActionListener(lipiac);
			debuginfo.addItemListener(this);

			project.add(newp);
			project.add(load);
			project.addSeparator();
			project.add(createPackage);			
			project.add(save);
			project.add(delete);
			project.addSeparator();
			project.add(closep);			
			project.add(exit);

			jmenubar.add(project);

			help.add(helptopics);
			help.add(debuginfo);
			help.addSeparator();
			help.add(about);

			jmenubar.add(help);
			setJMenuBar(jmenubar);

			Container container = getContentPane();
			TopPanel  topP = new TopPanel();
			BottomPanel botP= new BottomPanel();
			container.setLayout(new GridLayout(1,1));
			JSplitPane jsplitpane = new JSplitPane(0, topP, botP);
			container.add(jsplitpane);
			//Added by Murali on 06/12/2006 for warning a message dialog in case of delete a class or sample
			//Request Id: 1608969
			popupcontainer = container;
			if(lipicontroller.Environment_variable == null)
			{
				JOptionPane.showMessageDialog(container,"LIPI_ROOT Enviromental variable is not set. Check the Lipi-Designer user manual for further information.","Environment variable not set",JOptionPane.ERROR_MESSAGE);
				System.exit(0);
			}
			closep.setEnabled(false);
			save.setEnabled(false);
			delete.setEnabled(false);
			createPackage.setEnabled(false);
			
			addWindowListener(this);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LipiTkUI class from LipiTk Constructor:"+ ex.toString());
		}
	}


	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: itemStateChanged
	 * DESCRIPTION	: This function is used to change the debug status information from the UI
	 * ARGUMENTS	: None
	 * RETURNS		: ItemEvent - contains the reference to event generated when the item is selected
	 * NOTES		: Set the degug-status equal to true if the option is selected
	 *			  	  Set the degug-status equal to false if the option is deselected
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public void itemStateChanged(ItemEvent e)
	{
		if (e.getStateChange() == ItemEvent.DESELECTED)
		{
			debug_status = false;
		}
		else
		{
			debug_status = true;
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: main
	 * DESCRIPTION	: This is the main method which gets invoked when the program starts running.
	 *			      Creates the object instance of main class LipiTkUI
	 *			      Calls the constructor function of LipiTkUI class
	 * ARGUMENTS	: args - contains any command line arguments
	 * RETURNS		: None
	 * NOTES		: None
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static void main(String args[])
	{
		try
		{
			String s = System.getProperty("os.name");
			if(s.startsWith("Windows"))
			try
			{
					UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
			}
			catch(Exception exception)
			{
				System.out.println("not able to set Windowslook and Feel:" + exception.toString());
			}
			LipiTkUI lipimain = new LipiTkUI();
			lipiRefer = lipimain;
			lipimain.setTitle("Lipi Designer 4.0");

			lipimain.setDefaultCloseOperation(3);
			lipimain.setDefaultLookAndFeelDecorated(true);

			lipimain.setSize(750,600);
			//Displaying lipitk logo in UI
			lipimain.setIconImage(LipiTKUtil.showImage("images/lipitk_logo.jpg"));
			lipimain.show();
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LipiTkUI class from Main:"+ ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getLipiTkActionRef
	 * DESCRIPTION	: This method will return the reference to LipiTkAction class object
	 * ARGUMENTS	: None
	 * RETURNS		: lipiac - contains the reference to LipiTkAction class Instance
	 * NOTES		: None
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static LipiTkActionClass getLipiTkActionRef()
	{
		return lipiac;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: setTitleName
	 * DESCRIPTION	: This method will set the title name to the main dialog box
	 * ARGUMENTS	: titleName - contains the name of the Title to be set
	 * RETURNS		: None
	 * NOTES		: None
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static void setTitleName(String titleName)
	{
		if((titleName != null) && (titleName != ""))
			lipiRefer.setTitle("Lipi Designer 4.0: " + titleName +" Project");
		else
			lipiRefer.setTitle("Lipi Designer 4.0");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: DEBUG
	 * DESCRIPTION	: This method will display the Debug messages
	 * ARGUMENTS	: str - debug message
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static void DEBUG(String str)
	{
		if(debug_status)
			System.out.println(str);
	}

	/**********************************************************************************
	 * AUTHOR		: Srinivasa Vithal, Hari Krishna
	 * DATE			: 19-03-08
	 * NAME			: setMenuItemStatus
	 * DESCRIPTION	: This method will enable/disable the package menu item
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static void setMenuItemStatus(boolean close_menuitem_status, boolean pkg_menuitem_status)
	{
		try
		{
			File file=new File(LIPIUIController.Environment_variable+"/projects/"+LipiTkActionClass.project_name+"/config/default/nn.mdt");
			
			//to enable/disable close menuItem
			if (close_menuitem_status)
			{
				if (LipiTkActionClass.project_name == null)
				{
					closep.setEnabled(false);
					save.setEnabled(false);
					delete.setEnabled(false);
					load.setEnabled(true);
					newp.setEnabled(true);
					createPackage.setEnabled(false);
				}
				else
				{
					closep.setEnabled(true);
					save.setEnabled(true);
					delete.setEnabled(true);
					newp.setEnabled(false);
					load.setEnabled(false);
					createPackage.setEnabled(true);
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in Lipi UI class from SetMenuItemStatus function" + ex.toString());
		}
	}

	public static boolean debug_status;
	private static LipiTkUI lipiRefer;
	private static JMenuItem closep;
	private static JMenuItem load;
	private static JMenuItem newp;
	// added by Jitender 09/30/11 to add new menu items
	private static JMenuItem exit;
	private static JMenuItem save;
	private static JMenuItem delete;
	private static JMenuItem createPackage;
	
	@Override
	public void windowClosed(WindowEvent arg0) {
	}

	// Added by Jitender 21 July, 2011, to override system exit.
	@Override
	public void windowClosing(WindowEvent arg0) 
	{		
		System.out.println("closing..");
		
		int nOption = JOptionPane.showConfirmDialog(LipiTkUI.popupcontainer,"Do you want to exit ?","Confirmation",JOptionPane.YES_NO_OPTION);
					
		if(JOptionPane.YES_OPTION == nOption)
		{
			lipiac.closeFunction();
			lipiac.exitFunction();
			System.exit(0);
		}	
	}



	@Override
	public void windowActivated(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowDeactivated(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowDeiconified(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowIconified(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowOpened(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}

}
