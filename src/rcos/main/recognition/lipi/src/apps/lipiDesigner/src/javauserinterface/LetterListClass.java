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
 * FILE DESCR	: LetterListClass  is a class which handles the events generated in the ClassList box.
 * CONTENTS		:
 *			valueChanged
 *			updateList
 *			getClassList
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.border.*;
import java.awt.Dimension;
import java.io.File;
import java.io.FilenameFilter;

@SuppressWarnings("serial")
public class LetterListClass extends JPanel
implements ListSelectionListener
{

	private static String rootdir= "";
	LetterListSample jsampleID = null;
	static LetterListClass llistclass = null;

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: LetterListClass
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: lists - contains the reference to LetterListSample list box 
	 *			      root - contains the string to the path of root directory 
	 *			      label - contains the name of the LetterListSample
	 * RETURNS		: None
	 * NOTES		: Constructs the ClassListBox object
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public LetterListClass(LetterListSample lists,String root,String label)
	{
		LipiTkUI.DEBUG("Enter Into LetterListClass Class::Constructor Function");

		try
		{
			jsampleID = lists;
			rootdir = root;
			String [] empty_list = {};
			jletterList = new JList(empty_list);
			classIDs = jletterList;
			Border etched = BorderFactory.createEtchedBorder();
			jletterList.setBorder(BorderFactory.createTitledBorder(etched,label));
			jletterList.addListSelectionListener(this);
			jletterList.setVisibleRowCount(6);
			JScrollPane jscrollpane = new JScrollPane(jletterList);
			jscrollpane.setPreferredSize(new Dimension(100,200));
			add(jscrollpane);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LetterListClas class from constructor function"+ ex.toString());
		}

		LipiTkUI.DEBUG("Exit From LetterListClass Class::Constructor Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: valueChanged
	 * DESCRIPTION	: This method will get called when ever the contents of ClassListBox get changed
	 * ARGUMENTS	: listselectionevent - contains the reference to the ListSelectionEvent			  
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	@SuppressWarnings("null")
	public void valueChanged(ListSelectionEvent listselectionevent)
	{
		LipiTkUI.DEBUG("Enter Into LetterListClass Class::valueChanged Function");

		try
		{
			if(!listselectionevent.getValueIsAdjusting() && (jletterList.getModel().getSize ()) != 0)
			{
				rootdir = LipiTkActionClass.getDataDirectory();

				if(jletterList.getSelectedIndex() == -1)
				{
					jletterList.setSelectedIndex((jletterList.getModel().getSize ()) - 1);
				}

				int i = jletterList.getSelectedIndex();
				String elem = (String)jletterList.getModel().getElementAt(i);

				if(elem!= null || !elem.equals(""))
				{
					String newdir =rootdir+elem+"/";
					jsampleID.updateList(newdir);
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LetterListClass class from Value Changed action event"+ ex.toString());
		}

		LipiTkUI.DEBUG("Exit From LetterListClass Class::ValueChanged Function");
	}


	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: updateList
	 * DESCRIPTION	: This method will set the new list of classes into the ListBox
	 * ARGUMENTS	: dir String - variable which contains the path to the directory where the files are present			  
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void updateList(String dir)
	{
		LipiTkUI.DEBUG("Enter Into LetterListClass Class::updateList Function");
		try
		{
			File f2 = new File(dir);
			FilenameFilter filter = new LipiTKUtil();
			String [] samples = f2.list(filter);

			if((jletterList !=null) && (samples != null))
				jletterList.setListData(samples);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LetterListClas in UpdateList function"+ex.toString());
		}

		LipiTkUI.DEBUG("Exit From LetterListClass Class::updateList Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getClassList
	 * DESCRIPTION	: This method will set the new list of classes into the ListBox
	 * ARGUMENTS	: None			  
	 * RETURNS		: ClassIDs - list of classes
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static JList getClassList()
	{
		return classIDs;
	}

	private static JList classIDs;
	private JList jletterList;
}
